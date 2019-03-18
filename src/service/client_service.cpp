#include <iostream>
#include <string>

using namespace std;


int main()
{
    cout << "Client service is running" << endl;
    cout << "You can use only client commands!" << endl;
    cout << "Type 'exit' to stop service" << endl;
    string cmd;
    while(true)
    {
        cout << "client_service:$ client ";
        getline(cin, cmd);
        if(cmd == "exit")
            break;
        cmd = "./build/client " + cmd;
        system(cmd.c_str());
    }
    return 0;
}