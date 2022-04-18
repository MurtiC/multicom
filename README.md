# TempTag



## OPCUAServer Development
In the folder OPCUAServer is the developnet of the OPC-UA server.  
The start is in the server.c

### Setup
Setup Debian on your pc. For windows you can use wsl.

In Debian you need to install
```` bash
$ sudo apt update
$ sudo apt install build-essential
````
To confirm that the GCC compiler is successfully installed type:  
```` bash
$ gcc --version
````
````
Output:  
gcc (Debian 8.3.0-6) 8.3.0
Copyright (C) 2018 Free Software Foundation, Inc.
This is free software; see the source for copying conditions.  There is NO
warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.` 
````
The versions maight have changed.

### WSL
When Windows WSL is used, you need to update the IP to the virtual maschine after every restart. The get the IP of the virtual maschine, open Powershell and paste the following:  
```` powershell
wsl -- ip -o -4 -json addr list eth0 `
| ConvertFrom-Json `
| %{ $_.addr_info.local } `
| ?{ $_ }
````
````
Output:
123.123.123.123
````

This Ip must inserted to the server config:  
[OPCUAServer\files\server\config.csv](OPCUAServer\files\server\config.csv)

```` csv
Ip;Port
192.168.2.1;4841
````


### Start
To Start the server use:  
````
$ make run
````

To start the sever with nodemon use:  
````
$ make watch
````

To clear the port 4840 after a unknown error:  
````
$ make clearport
````

To clear all runtime files use:  
````
$ make clean
````

### Deployment
Copy server.o to Workspace folder.  
Copy folder open62541 to Workspace folder.

