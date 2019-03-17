#include "util.h"

namespace GLOBAL
{
	const string HELP = "--help";
	const string VERSION = "--version";
}

namespace COMMAND
{
	const string LIST = "list";
	const string VERSION = "version";
	const string SHOW = "show";
}

namespace ARGUMENT
{
	const string SERVER = "-server";
	const string PORT = "-port";
}

namespace URL
{
	const string VERSION = "/service/version";
	const string INTERFACES = "/service/v1/interfaces";
	const string INTERFACE = "/service/v1/interface/";
}

namespace CLIINFO
{
    const string VERSION = "v1.0.0";
    const string HELP_ONLY = "NAME:\n\t"
 		"client - client displays network interfaces and info.\n"
	"USAGE:\n\t"
 		"cli_net [global options] command [command options] <arguments...>\n"
	"VERSION:\n\t"
 		"1.0.0\n"
	"COMMANDS:\n\t"
		"list, version, show\n\t"
 		"use --help command name to get more information about command\n"
	"GLOBAL OPTIONS\n\t"
		"--help - shows utility information or information about command\n\t"
 		"--version - shows version information";

    const string HELP_AND_VERSION = "Description:\n\t"
            "Command \"version\" shows which interface version use server\n"
 	    "Using:\n\t"
        "version [server option] <server argument> [port argument] <port argument>\n\t"
 		"if arguments is not declared then will be used default address <127.0.0.1> and port <8080>\n"
        "Example:\n\t"
        "version -server 127.0.0.1 -port 8080";

    const string HELP_AND_LIST = "Description:\n\t"
            "Command \"list\" shows which network interfaces use server\n"
 	    "Using:\n\t"
        "list [server option] <server argument> [port argument] <port argument>\n\t"
 		"if arguments is not declared then will be used default address <127.0.0.1> and port <8080>\n"
        "Example:\n\t"
        "list -server 127.0.0.1 -port 8080";
    
    const string HELP_AND_SHOW = "Description:\n\t"
            "Command \"show\" shows information about network interface of server\n"
 	    "Using:\n\t"
        "show [name option] <server argument> [port argument] <port argument>\n\t"
 		"if arguments is not declared then will be used default address <127.0.0.1> and port <8080>\n"
        "Example:\n\t"
        "show eth0 -server 127.0.0.1 -port 8080";

    const string NO_OPTIONS = "No commands detected. See --help for more information";

    const string BAD_HELP_CMD = " is not a client command. See --help for more information";

    const string MANY_CMD_HELP = "So many commands for --help. See --help for more information";

    const string MANY_CMD_VERS = "So many commands for --version. See --help for more information";

    const string BAD_VERION = "Bad optins for version. See --help for more information";

    const string BAD_LIST = "Bad optins for list. See --help for more information";

    const string BAD_SHOW = "Bad optins for show. See --help for more information";
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

string ProcessParameters(vector<string>& args)
{
	int vSize = args.size();

	if(vSize == 1)
		return CLIINFO::NO_OPTIONS;

	auto make_request([](string ip, string port, string url, REQ_FLAG flag){
		http_request h_req = http_request(ip, port, url);
		h_req.MakeRequest();
		JSONParser jsonpars;
		if(status == HTTP_OK)
			switch (flag)
			{
				case VERSION:
					if(jsonpars.ParseVersionJSON(response))
						return jsonpars.GetParsedJSON();
				break;

				case LIST:
					if(jsonpars.ParseInterfacesJSON(response))
						return jsonpars.GetParsedJSON();
				break;

				case SHOW:
					if(jsonpars.ParseShowJSON(response))
						return jsonpars.GetParsedJSON();
				break;

				default:
					return string("Error: Can not process request...");
			}
		else if(status == HTTP_NOTFOUND)
		{
			if(ExtractJSON(response))
				if(jsonpars.ParseMessageJSON(response));
					return jsonpars.GetParsedJSON();
		}
		else if(status == HTTP_INTERNAL)
		{
			if(ExtractJSON(response))
				if(jsonpars.ParseErrorJSON(response));
					return jsonpars.GetParsedJSON();
		}
		else if(status == 0)
		{
			return string("Error: Can not process request... Looks like server is not available");
		}
		else
		{
			return string("Server return an error. Status: ") + to_string(status);
		}
		
		return string("Error: Can not process request...");
	});

	if(args[1] == GLOBAL::HELP && vSize == 2)
	{
		return CLIINFO::HELP_ONLY;
	}
	else if(args[1] == GLOBAL::HELP && vSize == 3)
	{
		if(args[2] == COMMAND::LIST)
		{
			return CLIINFO::HELP_AND_LIST;
		}
			
		if(args[2] == COMMAND::SHOW)
		{
			return CLIINFO::HELP_AND_SHOW;
		}
			
		if(args[2] == COMMAND::VERSION)
		{
			return CLIINFO::HELP_AND_VERSION;
		}

		return args[2] + CLIINFO::BAD_HELP_CMD;	
	}
	else if(args[1] == GLOBAL::HELP && vSize > 3)
	{
		return CLIINFO::MANY_CMD_HELP;
	}

	if(args[1] == GLOBAL::VERSION && vSize == 2)
	{
		return CLIINFO::VERSION;
	}
	else if(args[1] == GLOBAL::VERSION && vSize > 2)
	{
		return CLIINFO::MANY_CMD_VERS;
	}

	if(args[1] == COMMAND::LIST && vSize == 2)
	{
		return make_request(DEFAULT_IP, DEFAULT_PORT, URL::INTERFACES, LIST);
	}
	else if(args[1] == COMMAND::LIST && vSize == 6)
	{
		if(args[2] == ARGUMENT::SERVER && args[4] == ARGUMENT::PORT)
		{
			return make_request(args[3], args[5], URL::INTERFACES, LIST);
		}
		return CLIINFO::BAD_LIST;
	}
	else if(args[1] == COMMAND::LIST && vSize > 6)
		return CLIINFO::BAD_LIST;
	else if(args[1] == COMMAND::LIST && vSize < 6 && vSize > 2)
		return CLIINFO::BAD_LIST;
	

	if(args[1] == COMMAND::VERSION && vSize == 2)
	{
		return make_request(DEFAULT_IP, DEFAULT_PORT, URL::VERSION, VERSION);
	}
	else if(args[1] == COMMAND::VERSION && vSize == 6)
	{
		if(args[2] == ARGUMENT::SERVER && args[4] == ARGUMENT::PORT)
		{
			return make_request(args[3], args[5], URL::VERSION, VERSION);
		}
		return CLIINFO::BAD_VERION;
	}
	else if(args[1] == COMMAND::VERSION && vSize > 6)
		return CLIINFO::BAD_VERION;
	else if(args[1] == COMMAND::VERSION && vSize < 6 && vSize > 2)
		return CLIINFO::BAD_VERION;

	if(args[1] == COMMAND::SHOW && vSize == 3)
	{
		return make_request(DEFAULT_IP, DEFAULT_PORT, URL::INTERFACE + args[2], SHOW);
	}
	if(args[1] == COMMAND::SHOW && vSize == 2)
	{
		return CLIINFO::BAD_SHOW;
	}
	else if(args[1] == COMMAND::SHOW && vSize == 7)
	{
		if(args[3] == ARGUMENT::SERVER && args[5] == ARGUMENT::PORT)
		{
			return make_request(args[4], args[6], URL::INTERFACE + args[2], SHOW);
		}
		return CLIINFO::BAD_SHOW;
	}
	else if(args[1] == COMMAND::SHOW && vSize > 7)
		return CLIINFO::BAD_SHOW;
	else if(args[1] == COMMAND::SHOW && vSize < 7 && vSize > 3)
		return CLIINFO::BAD_SHOW;

	return CLIINFO::NO_OPTIONS;
}

bool ExtractJSON(string& html)
{
	int pos1 = html.find('{');
	int pos2 = html.find('}', pos1);
	if(pos2 < pos1)
		return false;
	string tmp = html.substr(pos1, pos2 - pos1 + 1);
	html = tmp;
	return true;
}