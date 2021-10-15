
#include <signal.h>
#include <stdlib.h>

#include "open62541/open62541.h"

#include "nodes.c"
#include "types.c"
//#include "typeExample.c"

#include "readFile.c"

static volatile UA_Boolean running = true;
static void stopHandler(int sig)
{
    UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND, "received ctrl-c");
    running = false;
}

int main(void)
{
    printf("\n\nStart Server\n");

    //char files[10][256];
    //getFiles(10,256,files,"/config/server");
    //for (int a=0; a < 10; a++) {
    //    printf("File: %s\n",files[a]);
    //}


    int serverConfigRows = 2;
    int serverConfigColums = 2;
    int serverConfigLength = 256;
    char serverCSV[serverConfigRows][serverConfigColums][serverConfigLength];
    getCSV("/config/server/server.csv",
           serverConfigRows,
           serverConfigColums,
           serverConfigLength,
           serverCSV);

    int ipCol = getCSVColum(
        serverConfigColums,
        serverConfigRows,
        serverConfigLength,
        serverCSV,
        "Ip");

    int portCol = getCSVColum(
        serverConfigColums,
        serverConfigRows,
        serverConfigLength,
        serverCSV,
        "Port");

    char* ipAdresse = serverCSV[1][ipCol];
    char* portText = serverCSV[1][portCol];

    
    // #region Setup
    signal(SIGINT, stopHandler);
    signal(SIGTERM, stopHandler);

    // Create new Server instance
    UA_Server *server = UA_Server_new();

    //set port number
    UA_Int16 port = atoi(portText);
    UA_ServerConfig_setMinimal(
        UA_Server_getConfig(server), 
        port, 
        0); 
    
    //set hostname
    UA_String hostname;
    UA_String_init(&hostname);
    hostname.length = strlen(ipAdresse);
    hostname.data = (UA_Byte *)ipAdresse;
    UA_ServerConfig_setCustomHostname(
        UA_Server_getConfig(server), 
        hostname);
    // #endregion
    
    // Variables
    manuallyDefineTemptag(
        server,
        "Tag 6",
        "E280B0403C0000000C014A97",
        "On Sensor",
        30,
        20);
    manuallyDefineTemptag(
        server,
        "Tag 7",
        "E280B0403C0000000C01511E",
        "On Sensor",
        30,
        20);
    manuallyDefineTemptag(
        server,
        "Tag 8",
        "E280B0403C0000000C014F84",
        "On Sensor",
        30,
        20);
    manuallyDefineTemptag(
        server,
        "Tag 9",
        "E280B0403C0000000C014AE1",
        "On Sensor",
        30,
        20);
    manuallyDefineTemptag(
        server,
        "Tag 10",
        "E280B0403C0000000C014855",
        "On Sensor",
        30,
        20);
    char data[24] = "Test";




    UA_StatusCode retval = UA_Server_run(server, &running);
    UA_Server_delete(server);
    return retval == UA_STATUSCODE_GOOD ? EXIT_SUCCESS : EXIT_FAILURE;
    //*/

    //  char buffer[1000];
    //  writeConfigFile("4840","/config/server/port.conf");
    //  readConfigFile(buffer,sizeof(buffer),"/config/server/port.conf");

    printf("Stopped Server\n\n\n");
}