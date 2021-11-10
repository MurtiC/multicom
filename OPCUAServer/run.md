## open6541
# Build .h/.c
sudo apt-get update
sudo apt-get install git build-essential gcc pkg-config cmake python3

git clone https://github.com/open62541/open62541
cd open62541
git submodule update --init --recursive

mkdir build
cd build

cmake -DBUILD_SHARED_LIBS=ON -DCMAKE_BUILD_TYPE=RelWithDebInfo -DUA_NAMESPACE_ZERO=FULL -DUA_ENABLE_AMALGAMATION=ON ..
make -j

cd ..
cd ..

# Load Repository
git clone https://github.com/MalteHeringUGlow/TempTag
cd TempTag
git checkout OPC-UA-Server
git fetch

cd OPCUAServer


## Building
# Linux
gcc -c -std=c99 open62541/open62541.c -o open62541/open62541.o
gcc -c server.c -o server.o
gcc open62541/open62541.o server.o -o server
./server

# Windows
gcc -c -std=c99 windows\open62541.c -o windows\open62541.o
gcc -c server.c -o server.o
gcc owindows\open62541.o server.o -o server
./server


## Nodemon
# Installation
sudo apt-get install npm
sudo npm install -g nodemon
# Run Nodemon
nodemon --exec ./server