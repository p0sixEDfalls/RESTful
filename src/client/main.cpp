#include <iostream>
#include "util.h"

//global --help --version
//cmd list show version
//args server port
//opt only for show  anystr


int main(int argc, char* argv[])
{
	vector<string> args = ParseParameters(argc, argv);
	//vector<string> args = {"fdgfdg", "list", "-server", "0.0.0.0", "-port" , "8000"};
	cout << ProcessParameters(args) << endl;
    return 0;
}
