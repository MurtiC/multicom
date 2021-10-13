
#include <signal.h>
#include <stdlib.h>

#include "open62541/open62541.h"

#include "nodes.c"
#include "types.c"

#include "readCsv.c"

static volatile UA_Boolean running = true;
static void stopHandler(int sig)
{
    UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND, "received ctrl-c");
    running = false;
}

int main(void)
{
    printf("\n\nStart Server\n");
    
    // #region Setup
    signal(SIGINT, stopHandler);
    signal(SIGTERM, stopHandler);

    UA_Server *server = UA_Server_new();

    char portText[10];
    readConfigFile(portText, sizeof(portText), "/config/server/port.conf");
    char url[500];
    readConfigFile(url, sizeof(url), "/config/server/ip.conf");

    UA_Int16 port = atoi(portText);
    UA_ServerConfig_setMinimal(UA_Server_getConfig(server), port, 0); //set port number

    UA_String hostname;
    UA_String_init(&hostname);
    hostname.length = strlen(url);
    hostname.data = (UA_Byte *)url;

    UA_ServerConfig_setCustomHostname(UA_Server_getConfig(server), hostname); //set hostname
    // #endregion
    
    // #region Variables
    addVariable(server);
    writeVariable(server);
    writeWrongVariable(server);
    // #endregion

    UA_StatusCode retval = UA_Server_run(server, &running);
    UA_Server_delete(server);
    return retval == UA_STATUSCODE_GOOD ? EXIT_SUCCESS : EXIT_FAILURE;
    //*/

    //  char buffer[1000];
    //  writeConfigFile("4840","/config/server/port.conf");
    //  readConfigFile(buffer,sizeof(buffer),"/config/server/port.conf");

    printf("Stopped Server\n\n\n");
}