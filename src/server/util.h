#include <evhttp.h>

#include <vector>
#include <map>
#include <string>

#include <sstream>
#include <algorithm>


#include <unistd.h>
#include <string.h>
#include <iomanip> 

using namespace std;

inline const string DEFAULT_IP = "127.0.0.1";
inline const string DEFAULT_PORT = "8080";

vector<string> ParseParameters(int argc, char* argv[]);
bool ProcessParameters(vector<string>& args);

static bool GetNetIntNames(vector<string>& vNames);
static bool GetIntInfo(string netName, map<string, string>& mInfo);
static void CheckInfo(map<string, string>& args);
static bool GetInterfaceFunc(vector<string>& parsedURL, string& name);
