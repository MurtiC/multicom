#include "../open62541/open62541.h"

#include <signal.h>
#include <stdlib.h>
#include <string.h>

static volatile UA_Boolean running = true;
UA_Double temperature = 20.0;
/* predefined identifier for later use */

static void stopHandler(int sig) {
    UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND, "received ctrl-c");
    running = false;
}

//definiert manuell eine Pumpe mit den Variablen 
    //ManufactorName
    //ModelName
    //Status
    //MotorRPM
static void
manuallyDefineTempsensor(UA_Server *server) {
    UA_NodeId tempId; /* get the nodeid assigned by the server */
    UA_ObjectAttributes oAttr = UA_ObjectAttributes_default;
    UA_Server_addObjectNode(server, UA_NODEID_STRING(2, "Sensor1"),
                            UA_NODEID_NUMERIC(0, UA_NS0ID_OBJECTSFOLDER),
                            UA_NODEID_NUMERIC(0, UA_NS0ID_ORGANIZES),
                            UA_QUALIFIEDNAME(1, "Temperatur Sensor"), UA_NODEID_NUMERIC(0, UA_NS0ID_BASEOBJECTTYPE),
                            oAttr, NULL, &tempId);

    UA_VariableAttributes mnAttr = UA_VariableAttributes_default;
    UA_String manufacturerName = UA_STRING("Pump King Ltd.");
    UA_Variant_setScalar(&mnAttr.value, &manufacturerName, &UA_TYPES[UA_TYPES_STRING]);
    UA_Server_addVariableNode(server, UA_NODEID_NULL, tempId,
                              UA_NODEID_NUMERIC(0, UA_NS0ID_HASCOMPONENT),
                              UA_QUALIFIEDNAME(1, "ManufacturerName"),
                              UA_NODEID_NUMERIC(0, UA_NS0ID_BASEDATAVARIABLETYPE), mnAttr, NULL, NULL);

    UA_VariableAttributes tempAttr = UA_VariableAttributes_default;
    UA_Double temp = 50.0;
    UA_Variant_setScalar(&tempAttr.value, &temp, &UA_TYPES[UA_TYPES_DOUBLE]);
    UA_Server_addVariableNode(server, UA_NODEID_NULL, tempId,
                              UA_NODEID_NUMERIC(0, UA_NS0ID_HASCOMPONENT),
                              UA_QUALIFIEDNAME(1, "Temperatur"),
                              UA_NODEID_NUMERIC(0, UA_NS0ID_BASEDATAVARIABLETYPE), tempAttr, NULL, NULL);


    UA_VariableAttributes schwelltempAttr = UA_VariableAttributes_default;
    UA_Double schwellwert = 51.0;
    UA_Variant_setScalar(&schwelltempAttr.value, &schwellwert, &UA_TYPES[UA_TYPES_DOUBLE]);
    UA_Server_addVariableNode(server, UA_NODEID_NULL, tempId,
                              UA_NODEID_NUMERIC(0, UA_NS0ID_HASCOMPONENT),
                              UA_QUALIFIEDNAME(1, "Schwellwert"),
                              UA_NODEID_NUMERIC(0, UA_NS0ID_BASEDATAVARIABLETYPE), schwelltempAttr, NULL, NULL);
}

static void
beforeReadTemperature(UA_Server *server,
               const UA_NodeId *sessionId, void *sessionContext,
               const UA_NodeId *nodeid, void *nodeContext,
               const UA_NumericRange *range, const UA_DataValue *data) {
    float tmp = 1.0 * (rand()%100)/100 - 0.5;
	temperature += tmp;

	UA_Variant value;
    UA_Variant_setScalar(&value, &temperature, &UA_TYPES[UA_TYPES_DOUBLE]);		//Copy the Temperature in a variant variable
    UA_Server_writeValue(server, UA_NODEID_STRING(2, "R1_TS1_Temperature"), value);

}



int main(int argc, char * argv[]) {
    signal(SIGINT, stopHandler);
    signal(SIGTERM, stopHandler);

    UA_Server *server = UA_Server_new();
    
    if(argc > 2) {  //check for hostname/ip and port
        UA_Int16 port = atoi(argv[2]);
        UA_ServerConfig_setMinimal(UA_Server_getConfig(server), port, 0); //set port number
    }else{
        UA_ServerConfig_setDefault(UA_Server_getConfig(server));
    }

    if(argc > 1){ //check for hostname/ip
        UA_String hostname;
        UA_String_init(&hostname);
        hostname.length = strlen(argv[1]);
        hostname.data = (UA_Byte *) argv[1];

        UA_ServerConfig_setCustomHostname(UA_Server_getConfig(server), hostname);  //set hostname
    }

    manuallyDefineTempsensor(server);

    UA_ValueCallback callback ;
    callback.onRead = beforeReadTemperature;
    callback.onWrite = NULL;
    UA_Server_setVariableNode_valueCallback(server, UA_NODEID_STRING(2, "Sensor1"), callback);

    UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND, "starting server....");
    UA_StatusCode retval = UA_Server_run(server, &running);

    UA_Server_delete(server);
    return retval == UA_STATUSCODE_GOOD ? EXIT_SUCCESS : EXIT_FAILURE;
}