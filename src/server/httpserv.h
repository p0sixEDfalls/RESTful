#include <evhttp.h>

#include <sstream>
#include <algorithm>
#include <ifaddrs.h>
#include <net/if.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <iomanip> 

#include <rapidjson/document.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>

#include "util.h"

using namespace std;
using namespace rapidjson;

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
    const string MESSAGE = "message";
    const string ERROR = "error";
}

class CreateJSON
{
private:

    Document document;
    string resp;

    string GetJsonString()
    {
        StringBuffer buffer;
        buffer.Clear();
        Writer<StringBuffer> writer(buffer);
        document.Accept(writer);
        return strdup(buffer.GetString());
    }

public:
    CreateJSON() {}

    void CreateShowJSON(map<string, string>& args)
    {
        document.SetObject();
        auto& allocator = document.GetAllocator();
        Value val;
        Value arr(kArrayType);
        Value key;

        key.SetString(JSONOPT::NAME.c_str(), JSONOPT::NAME.size(), allocator);
        val.SetString(args[JSONOPT::NAME].c_str(), args[JSONOPT::NAME].size(), allocator);
        document.AddMember(key, val, allocator);

        key.SetString(JSONOPT::HWADDR.c_str(), JSONOPT::HWADDR.size(), allocator);
        val.SetString(args[JSONOPT::HWADDR].c_str(), args[JSONOPT::HWADDR].size(), allocator);
        document.AddMember(key, val, allocator);

        key.SetString(JSONOPT::INET_ADDR.c_str(), JSONOPT::INET_ADDR.size(), allocator);
        arr.PushBack(Value{}.SetString(args[JSONOPT::IPV4].c_str(), args[JSONOPT::IPV4].length(), allocator), allocator);
        arr.PushBack(Value{}.SetString(args[JSONOPT::IPV6].c_str(), args[JSONOPT::IPV6].length(), allocator), allocator);
        document.AddMember(key, arr, allocator);

        key.SetString(JSONOPT::MASK.c_str(), JSONOPT::MASK.size(), allocator);
        val.SetString(args[JSONOPT::MASK].c_str(), args[JSONOPT::MASK].size(), allocator);
        document.AddMember(key, val, allocator);

        key.SetString(JSONOPT::MTU.c_str(), JSONOPT::MTU.size(), allocator);
        val.SetString(args[JSONOPT::MTU].c_str(), args[JSONOPT::MTU].size(), allocator);
        document.AddMember(key, val, allocator);
        resp = GetJsonString();
    }

    void CreateIterfacesJSON(vector<string>& args)
    {
        document.SetObject();
        auto& allocator = document.GetAllocator();
        
        Value key(JSONOPT::INTERFACES.c_str(), JSONOPT::INTERFACES.size(), allocator);
        Value arr(kArrayType);

        for(string s: args)
            arr.PushBack(Value{}.SetString(s.c_str(), s.length(), allocator), allocator);
        
        
        document.AddMember(key, arr, allocator);
        resp = GetJsonString();
    }

    void CreateMessageJSON(string& error)
    {
        document.SetObject();
        auto& allocator = document.GetAllocator();
        
        Value key(JSONOPT::MESSAGE.c_str(), JSONOPT::MESSAGE.size(), allocator);
        Value val;
        val.SetString(error.c_str(), error.size(), allocator);

        document.AddMember(key, val, allocator);
        resp = GetJsonString();
    }

    void CreateErrorJSON(string& error)
    {
        document.SetObject();
        auto& allocator = document.GetAllocator();
        
        Value key(JSONOPT::ERROR.c_str(), JSONOPT::ERROR.size(), allocator);
        Value val;
        val.SetString(error.c_str(), error.size(), allocator);

        document.AddMember(key, val, allocator);
        resp = GetJsonString();
    }

    string GetJSON()
    {
        return resp;
    }
};

class http_server
{
private:
    string server_ip;
    string port;
    uint16_t port_dec;
    evhttp * server;

    static void CheckReq(evhttp_request *req, void *)
    {
        auto URLParser([](string& url){
            replace(url.begin(), url.end(), '/', ' ');
            vector<string> vPars;
            stringstream ss(url);
            string temp;
            while (ss >> temp)
                vPars.push_back(temp);
            return vPars;
        });

        const char * urlst = evhttp_request_get_uri(req);
        string urlstr = urlst;
        vector<string> vURL = URLParser(urlstr);
        string name;
        if(GetInterfaceFunc(vURL, name))
        {
            evbuffer * outBuf = evhttp_request_get_output_buffer(req);
            if (!outBuf)
            {
                CreateJSON res;
                string err = "Can not process request";
                res.CreateErrorJSON(err);
                evhttp_send_error(req, HTTP_INTERNAL, res.GetJSON().c_str());
                return;
            }
            
            map<string, string> args;
            if(GetIntInfo(name, args))
            {
                CheckInfo(args);
                CreateJSON res;
                res.CreateShowJSON(args);
                evbuffer_add_printf(outBuf, res.GetJSON().c_str());
	            evhttp_send_reply(req, HTTP_OK, "", outBuf);
            }
            else
            {
                CreateJSON res;
                string err = "interface " + name + " was not found";
                res.CreateMessageJSON(err);
                evhttp_send_error(req, HTTP_NOTFOUND, res.GetJSON().c_str());
                return;
            }
        }

        CreateJSON res;
        string err = "Not Found";
        res.CreateMessageJSON(err);
        evhttp_send_error(req, HTTP_NOTFOUND, res.GetJSON().c_str());
    }

    static void version(evhttp_request *req, void *)
    {
        evbuffer * outBuf = evhttp_request_get_output_buffer(req);
        if (!outBuf)
        {
            CreateJSON res;
            string err = "Can not process request";
            res.CreateErrorJSON(err);
            evhttp_send_error(req, HTTP_INTERNAL, res.GetJSON().c_str());
        }
        evbuffer_add_printf(outBuf, "{\"version\":\"v1\"}");
	    evhttp_send_reply(req, HTTP_OK, "", outBuf);
    }

    static void interfaces(evhttp_request *req, void *)
    {
        evbuffer * outBuf = evhttp_request_get_output_buffer(req);
        if (!outBuf)
        {
            CreateJSON res;
            string err = "Can not process request";
            res.CreateErrorJSON(err);
            evhttp_send_error(req, HTTP_INTERNAL, res.GetJSON().c_str());
        }
        vector<string> names;
        if(GetNetIntNames(names))
        {
            CreateJSON res;
            res.CreateIterfacesJSON(names);
            evbuffer_add_printf(outBuf, res.GetJSON().c_str());
	        evhttp_send_reply(req, HTTP_OK, "", outBuf);
        }
        else
        {   
            CreateJSON res;
            string err = "Can not process request";
            res.CreateErrorJSON(err);
            evhttp_send_error(req, HTTP_INTERNAL, res.GetJSON().c_str());      
        }
    }

public:

    http_server(string ip, string p) : server_ip(ip), port(p) {}

    ~http_server() 
    {
        evhttp_free(server);
    }

    bool Start()
    {
        if(port_dec = atoi(port.c_str()); port_dec == 0)
            return false;
        server = evhttp_start(server_ip.c_str(), port_dec);
        if(server == nullptr)
            return false;
        evhttp_set_cb (server, "/service/version", version, 0);
        evhttp_set_cb (server, "/service/v1/interfaces", interfaces, 0);
        evhttp_set_gencb(server, CheckReq, nullptr);
        if(event_dispatch() == -1)
        {
            return false;
        }
    } 
};