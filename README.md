# RESTful
[![Build Status](https://travis-ci.com/p0sixEDfalls/RESTful.svg?branch=master)](https://travis-ci.com/p0sixEDfalls/RESTful)

# What is the project?
This is a service, which implements the API for working with network interfaces in Linux operating system and CLI client for 
interacting with the service.
# How to use
In the client application can be used the commands such as:

	list            Shows which network interfaces use the server
 	                Usage: list [server argument] [port argument]
                    If arguments is not declared then will be used default address <127.0.0.1> and port <8080>
 	                Example: list -server 127.0.0.1 -port 8080

	version         Shows which interface version use the server
	                Usage: version [server option] <server argument> [port argument] <port argument>
	                If arguments is not declared then will be used default address <127.0.0.1> and port <8080>
                    Example: version -server 127.0.0.1 -port 8080

	show            Shows information about network interface of the server
	                Usage:  show [name option] [server option] <server argument> [port option] <port argument>
	                If arguments is not declared then will be used default address <127.0.0.1> and port <8080>
                    Example: show eth0 -server 127.0.0.1 -port 8080
                  
	--help          Shows inforamtion about the client. Can be combained with client commands to get information about it
	--version       Shows the interface version used by client
  
The server application may use these optional options:

	-server         Set the IP-address which should listen
	-port           Set the port which should listen
	                Example -server 127.0.0.1 -port 8080

# How to build
The server and client are designed only for **Linux** based systems, so you should build it only on Linux(for example, Ubuntu).

Open the terminal and use the following commands to build:

Install GNU G++ Compiler:

	$ sudo apt install g++

Check the version of g++. It should be 7.x.x+.

	$ sudo g++ --version

Install cmake:

	$ sudo apt-get install cmake

Install and build *googletest* packages:

	$ sudo apt-get install libgtest-dev
	$ cd /usr/src/gtest
	$ sudo cmake CMakeLists.txt
	$ sudo make
	$ sudo cp *.a /usr/lib
  
Install *libevent* packages:

	$ sudo apt-get install libevent-dev

Install *rapidjson* packages:

	$ sudo apt-get install rapidjson-dev

Now when all dependencies are installed clone the repository:

	$ sudo git clone https://github.com/p0sixEDfalls/RESTful.git

Open the repository folder and build server and client applications:

	$ cd RESTful 
	$ cmake CMakeLists.txt
	$ cmake --build . --target server
	$ cmake --build . --target client
  
Optionally you can build and run server and client tests:

	$ cmake --build . --target build_and_test
  
Now the *build* folder is created and server and client executables inside.

# How to build docker image

Simply you can run builded image from Docker Hub(login before):

	$ sudo docker run -i -t p0sixedfalls/restful
	
Or build it yourself:

Clone the repository and open created folder if you don't do it:

	$ sudo git clone https://github.com/p0sixEDfalls/RESTful.git
	$ cd RESTful 

Now build and run the image:

	$ sudo docker build . -t <your_image_name> .
	$ sudo docker run -i -t <your_image_name>
	
The docker container will start the server as background and execute the wrapper for client, 
where ony client commands are available.
Now you can check all commands of client:

	Client service is running
	You can use only client commands!
	Type 'exit' to stop service
	client_service:$ client --help
	NAME:
		client - client displays network interfaces and info.
	USAGE:
		cli_net [global options] command [command options] <arguments...>
	VERSION:
		1.0.0
	COMMANDS:
		list, version, show
		use --help command name to get more information about command
	GLOBAL OPTIONS
		--help - shows utility information or information about command
		--version - shows version information

Docker Hub link:

	https://cloud.docker.com/repository/docker/p0sixedfalls/restful	

# Built with
  * [googletest](https://github.com/google/googletest)
  * [libevent](https://github.com/libevent/libevent)
  * [rapidjson](https://github.com/Tencent/rapidjson) 

# Any questions?
Contact me:

	xdb7493c6e70@yandex.ru
