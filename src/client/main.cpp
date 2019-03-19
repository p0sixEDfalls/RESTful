#include <iostream>
#include "util.h"

int main(int argc, char* argv[])
{
	vector<string> args = ParseParameters(argc, argv);
	cout << ProcessParameters(args) << endl;
    return 0;
}
