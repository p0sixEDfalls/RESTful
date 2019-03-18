sudo apt-get install -y libgtest-dev
sudo apt-get install cmake
cd /usr/src/gtest
sudo cmake CMakeLists.txt
sudo make
sudo cp *.a /usr/lib

sudo apt-get update
sudo apt-get install -y libevent-dev
sudo apt-get install rapidjson-dev
cd -