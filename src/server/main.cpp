#include "httpserv.h"

int main(int argc, char* argv[])
{   
    vector<string> args = ParseParameters(argc, argv);
	if(ProcessParameters(args))
        return -1;
    return 0;
}