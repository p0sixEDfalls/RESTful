#include <string>
#include <vector>

#include "httpreq.h"

enum REQ_FLAG {VERSION, LIST, SHOW};

vector<string> ParseParameters(int argc, char* argv[]);
string ProcessParameters(vector<string>& args);
bool ExtractJSON(string& html);