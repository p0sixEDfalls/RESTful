#include <evhttp.h>
#include <memory>
#include <string>
#include <algorithm>
#include <sstream>
#include <rapidjson/document.h>


using namespace rapidjson;
using namespace std;

inline string response;
inline int status = 0;
inline const string DEFAULT_IP = "127.0.0.1";
inline const string DEFAULT_PORT = "8080";

class http_request 
{
private:

    event_base * base = nullptr;
    evhttp_connection * ev_con = nullptr;
    evhttp_request * req = nullptr;
    string server_ip;
    string port;
    uint16_t port_dec;
    string url;
    
    static void http_request_done(evhttp_request * req, void * arg)
    {
        auto print([](evhttp_request * req){
            char buf[1024];
            int s = evbuffer_remove(req->input_buffer, &buf, sizeof(buf) - 1);
            buf[s] = '\0';
            return string(buf);
        });
         
        if(req == nullptr)
        {
            response = "Error: Can not create request...\nTry to check IP-address or port...";
        }
        else
        {
            status = evhttp_request_get_response_code(req);
            if(status == HTTP_OK || status == HTTP_NOTFOUND || status == HTTP_INTERNAL)
            {
                response = print(req);
            }
            else
            {
                response = "Error: something wrong else...\nRequest is bad...";
            }
        }
    
        if(event_base_loopbreak((struct event_base *)arg) == -1)
            throw runtime_error("Error: Can not create request...");
    }

public:

    http_request(string ip, string port, string u)
    {
        server_ip = ip;
        port = port;
        url = u;
        if(port_dec = atoi(port.c_str()); port_dec == 0)
            throw runtime_error("Error: bad port\n");
    }

    bool MakeRequest()
    {
        if(base = event_base_new(); base == nullptr)
            return false;

	    if(ev_con = evhttp_connection_base_new(base, NULL, server_ip.c_str(), port_dec); ev_con == nullptr)
		    return false;
        
        if(req = evhttp_request_new(http_request::http_request_done, base); req == nullptr)
            return false;
        
        string host = server_ip + ":" + port;
	    evkeyvalq *output_headers = evhttp_request_get_output_headers(req);
	    evhttp_add_header(output_headers, "Host", host.c_str());
	    evhttp_add_header(output_headers, "Connection", "close");

        if(req == nullptr)
            return false;
	    if(int r = evhttp_make_request(ev_con, req, EVHTTP_REQ_GET, url.c_str()); r != 0)
		    return false;
        evhttp_connection_set_timeout(req->evcon, 3);
	    if(event_base_dispatch(base) == -1)
            return false;
    }
};

namespace JSONOPT
{
    const string NAME = "name";
    const string HWADDR = "hw_addr";
    const string IPV4 = "IPv4";
    const string IPV6 = "IPv6";
    const string MASK = "mask";
    const string MTU = "MTU";
    
    const string INET_ADDR = "inet_addr";
    const string INTERFACES = "interfaces";
    const string VERSION = "version";
    const string VERSION_NUM = "v1";
    const string MESSAGE = "message";
    const string ERROR = "error";
}

class JSONParser
{
private:

    string resp;
    Document document;

public:

    bool ParseVersionJSON(string& json)
    {
        document.Parse(json.c_str());
        if(!document.IsObject())
            return false;

        if(!document.HasMember(JSONOPT::VERSION.c_str()))
            return false;

        string t1 = document[JSONOPT::VERSION.c_str()].GetString();
        if(t1 == JSONOPT::VERSION_NUM)
        {
            resp = "v1.0.0";
            return true;
        }

        return false;            
    }

    bool ParseInterfacesJSON(string& json)
    {
	    document.Parse(json.c_str());
	    if(document.HasMember(JSONOPT::INTERFACES.c_str()))
	    {
		    const Value& arr = document[JSONOPT::INTERFACES.c_str()];
		    if(arr.IsArray())
		    {
			    for (SizeType i = 0; i < arr.Size(); i++) 
        		    resp = resp + arr[i].GetString() + ",";
		    }
            else
                return false;
	    }
        else
            return false;
        
	    resp.erase(resp.size() - 1);
        return true;
    }

    bool ParseShowJSON(string& json)
    {
        document.Parse(json.c_str());

        if(document.HasMember(JSONOPT::NAME.c_str()))
	    {
		    resp = document[JSONOPT::NAME.c_str()].GetString();
            resp += ":\n\t";
	    }
        else
            return false;

        if(document.HasMember(JSONOPT::HWADDR.c_str()))
	    {
		    resp += JSONOPT::HWADDR + ": " + document[JSONOPT::HWADDR.c_str()].GetString() + "\n\t";
	    }
        else
            return false;

        if(document.HasMember(JSONOPT::INET_ADDR.c_str()))
	    {
		    const Value& arr = document[JSONOPT::INET_ADDR.c_str()];
		    if(arr.IsArray())
		    {
                resp += JSONOPT::IPV4 + ": " + arr[0].GetString() + "\n\t";
			    resp += JSONOPT::IPV6 + ": " + arr[1].GetString() + "\n\t";
		    }
            else
                return false;
	    }
        else
            return false;

        if(document.HasMember(JSONOPT::MASK.c_str()))
	    {
		    resp += JSONOPT::MASK + ": " + document[JSONOPT::MASK.c_str()].GetString() + "\n\t";
	    }
        else
            return false;

        if(document.HasMember(JSONOPT::MTU.c_str()))
	    {
		    resp += JSONOPT::MTU + ": " + document[JSONOPT::MTU.c_str()].GetString();
	    }
        else
            return false;

        return true;
    }

    bool ParseMessageJSON(string& json)
    {
        document.Parse(json.c_str());

        if(document.HasMember(JSONOPT::MESSAGE.c_str()))
	    {
		    resp = document[JSONOPT::MESSAGE.c_str()].GetString();
	    }
        else
            return false;
        
        return true;
    }

    bool ParseErrorJSON(string& json)
    {
        document.Parse(json.c_str());

        if(document.HasMember(JSONOPT::ERROR.c_str()))
	    {
		    resp = document[JSONOPT::ERROR.c_str()].GetString();
	    }
        else
            return false;
        
        return true;
    }

    string GetParsedJSON()
    {
        return resp;
    }
};