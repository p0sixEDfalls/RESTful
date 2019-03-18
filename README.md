# RESTful
[![Build Status](https://travis-ci.com/p0sixEDfalls/RESTful.svg?branch=master)](https://travis-ci.com/p0sixEDfalls/RESTful)

# What is RESTful project?
The RESTful project is a simple restful service with which contract a client.
# How to use
In client application can be used the commands as:

	list            Shows which network interfaces use server
 	                Using: list [server argument] [port argument]
                    If arguments is not declared then will be used default address <127.0.0.1> and port <8080>
 	                Example: list -server 127.0.0.1 -port 8080

	version         Shows which interface version use server
	                Using: version [server option] <server argument> [port argument] <port argument>
	                If arguments is not declared then will be used default address <127.0.0.1> and port <8080>
                    Example: version -server 127.0.0.1 -port 8080

	show            Shows information about network interface of server
	                Using:  show [name option] [server option] <server argument> [port option] <port argument>
	                If arguments is not declared then will be used default address <127.0.0.1> and port <8080>
                    Example: show eth0 -server 127.0.0.1 -port 8080
                  
	--help          Shows inforamtion about the clien. Can be combained with client command to get information about it
	--version       Shows the interface version used by client
  
The server application may use optional arguments:

	-server         Set the IP-addres which should be listened
	-port           Set the port which should be listened
	                Example -server 127.0.0.1 -port 8080

# How to build
Server and client are disegned only for **Linux** based systems, so you should build it only on Linux(Debain).

Open terminal and use the following commands to build:

Instal GNU G++ Compiler:

	$ sudo apt install g++
  
Instal cmake:

	$ sudo apt-get install cmake

Install and build *googletest* packkages:

	$ sudo apt-get install -y libgtest-dev
	$ cd /usr/src/gtest
	$ sudo cmake CMakeLists.txt
	$ sudo make
	$ sudo cp *.a /usr/lib
	$ sudo ln -s /usr/lib/libgtest.a /usr/local/lib/gtest/libgtest.a
	$ sudo ln -s /usr/lib/libgtest_main.a /usr/local/lib/gtest/libgtest_main.a
  
Install *libevent* packages:

	$ sudo apt-get install -y libevent-dev

Install *rapidjson* packages:

	$ sudo apt-get install rapidjson-dev

Now when all dependencies are installed clone the repository:

	$ sudo git clone https://github.com/p0sixEDfalls/RESTful.git

Open the folder and build the server and client:

	$ cd RESTful 
	$ cmake CMakeLists.txt
	$ cmake --build . --target server
	$ cmake --build . --target client
  
Optionally you can build and run server and client tests

	$ cmake --build . --target build_and_test
  
Now the *build* folder is created and server and client executables inside

# Built with
  * [googletest](https://github.com/google/googletest)
  * [libevent](https://github.com/libevent/libevent)
  * [rapidjson](https://github.com/Tencent/rapidjson) 
