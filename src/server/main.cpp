#include <iostream>

#include "httpserv.h"

int main(int argc, char* argv[])
{   
    vector<string> args = ParseParameters(argc, argv);
	if(!ProcessParameters(args))
        cout << "Can not run the server..." << endl;
    return 0;
}