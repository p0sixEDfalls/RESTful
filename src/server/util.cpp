#include <ifaddrs.h>
#include <net/if.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <arpa/inet.h>

#include "httpserv.h"

static bool GetNetIntNames(vector<string>& vNames)
{
    ifaddrs * ifaddr, * ifiter;
    string name;
    if(getifaddrs(&ifaddr) == -1)
        return false;
    for(ifiter = ifaddr; ifiter != nullptr; ifiter = ifiter->ifa_next)
    {
        name = string(ifiter->ifa_name);
        if(find(vNames.begin(), vNames.end(), name) == vNames.end())
            vNames.push_back(name);
    }
    freeifaddrs(ifaddr);
    return true;
}

static bool GetIntInfo(string netName, map<string, string>& mInfo)
{
    mInfo[JSONOPT::NAME] = netName;
    ifaddrs * ifaddr, * ifiter;
    int family, res;
    char address[NI_MAXHOST] = {0};
    string name;
    unsigned char hwaddr[6];
    int mtu;
    int sock = socket(AF_INET, SOCK_DGRAM, 0);
    ifreq ifr;

    if(getifaddrs(&ifaddr) == -1)
        return false;

    for(ifiter = ifaddr; ifiter != nullptr; ifiter = ifiter->ifa_next)
    {
        name = string(ifiter->ifa_name);
        if(name == netName)
        {
            if(ifiter->ifa_addr == nullptr)         
                return false;

            family = ifiter->ifa_addr->sa_family;

            if(family == AF_INET || family == AF_INET6)
            {
                res = getnameinfo(ifiter->ifa_addr, (family == AF_INET) ? sizeof(sockaddr_in) :
                                                                          sizeof(sockaddr_in6),
                                                    address, NI_MAXHOST, nullptr, 0, NI_NUMERICHOST);
                if(res != 0)
                    return false;
                
                if(family == AF_INET)
                    mInfo[JSONOPT::IPV4] = address;
                else
                    mInfo[JSONOPT::IPV6] = address;
            }
        }
    }

    ifr.ifr_addr.sa_family = AF_INET;
    strncpy(ifr.ifr_name , netName.c_str() , IFNAMSIZ-1);
    if(int r = ioctl(sock, SIOCGIFHWADDR, &ifr); r == -1)
        return false;
    
    stringstream ss;
    for(int i = 0; i < 6; i++)
    {
        ss << hex << setfill('0') << setw(2) << (0xFF & ifr.ifr_hwaddr.sa_data[i]) << ":";
    }
    string temp = ss.str();
    temp.erase(temp.size() - 1);
    mInfo[JSONOPT::HWADDR] = temp;

    if(int r = ioctl(sock, SIOCGIFMTU, &ifr); r == -1)
        return false;
    mtu = ifr.ifr_mtu;
    mInfo[JSONOPT::MTU] = to_string(mtu);

    if(int r = ioctl(sock, SIOCGIFNETMASK, &ifr); r == -1)
        return false;
    mInfo[JSONOPT::MASK] = inet_ntoa(((struct sockaddr_in *)&ifr.ifr_addr)->sin_addr);
    freeifaddrs(ifaddr);
    close(sock);
    return true;
}

static void CheckInfo(map<string, string>& args)
{
    if(args[JSONOPT::IPV4].empty())
        args[JSONOPT::IPV4] = "Not used";

    if(args[JSONOPT::IPV6].empty())
        args[JSONOPT::IPV6] = "Not used";

    if(args[JSONOPT::MASK].empty())
        args[JSONOPT::MASK] = "Not used";

    if(args[JSONOPT::MTU].empty())
        args[JSONOPT::MTU] = "Not used";

    if(args[JSONOPT::HWADDR].empty())
        args[JSONOPT::HWADDR] = "Not used";
}

static bool GetInterfaceFunc(vector<string>& parsedURL, string& name)
{
    if(parsedURL.size() != 4)
        return false;
    
    if(parsedURL[0] != "service" && parsedURL[1] != "v1" && parsedURL[2] != "interface")
        return false;
    
    name = parsedURL[3];
    return true;
}

vector<string> ParseParameters(int argc, char* argv[])
{
    vector<string> vArgs;
    for (int i = 0; i < argc; i++)
    {
        vArgs.push_back(string(argv[i]));
    }
    return vArgs;
}

bool ProcessParameters(vector<string>& args)
{
    if (!event_init())
    {
        return false;
    }
    if(args.size() == 1)
    {
        http_server server = http_server(DEFAULT_IP, DEFAULT_PORT);
        return server.Start();
    }
    else if(args.size() == 5 && args[1] == "-server" && args[3] == "-port")
    {
        http_server server = http_server(args[2], args[4]);
        return server.Start();
    }
    else
    {
        return false;
    }
    
}

