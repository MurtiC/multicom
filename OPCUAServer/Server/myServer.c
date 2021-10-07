#include "open62541.h"

#include <signal.h>
#include <stdlib.h>
#include <string.h>

static volatile UA_Boolean running = true;
UA_Double temperature = 20.0;
static void stopHandler(int sig) {
    UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND, "received ctrl-c");
    running = false;
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

static void
afterWriteLicht(UA_Server *server,
               const UA_NodeId *sessionId, void *sessionContext,
               const UA_NodeId *nodeId, void *nodeContext,
               const UA_NumericRange *range, const UA_DataValue *data) {
    UA_Variant value;
    UA_Boolean status;

    UA_Server_readValue(server, UA_NODEID_STRING(2, "R1_Lampe1_Status(on/off)"), &value);
    status = *(UA_Boolean*) value.data;

    if(status == true){
    UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND, "Licht AN");    
    }else{
    UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND, "Licht AUS");    
    }
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

    //Add new namespace to the server
    UA_Int16 ns_room1 = UA_Server_addNamespace(server, "Room1");
    UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND, "New Namespace added: &d", ns_room1);

    //Add new object called Temperatur Sensor to the Server
    UA_NodeId r1_tempsensor_Id; /* get the nodeid assigned by the server */
    UA_ObjectAttributes oAttr = UA_ObjectAttributes_default;
    UA_Server_addObjectNode(server, UA_NODEID_STRING(2, "R1_TS1"),
                            UA_NODEID_NUMERIC(0, UA_NS0ID_OBJECTSFOLDER),
                            UA_NODEID_NUMERIC(0, UA_NS0ID_ORGANIZES),
                            UA_QUALIFIEDNAME(2, "Temperature Sensor"), UA_NODEID_NUMERIC(0, UA_NS0ID_BASEOBJECTTYPE),
                            oAttr, NULL, &r1_tempsensor_Id);

    //Add new variable seriennumer to the server
    UA_VariableAttributes snAttr = UA_VariableAttributes_default;
    UA_Int32 serialNumber = 111111;
    UA_Variant_setScalar(&snAttr.value, &serialNumber, &UA_TYPES[UA_TYPES_INT32]);
    UA_Server_addVariableNode(server, UA_NODEID_STRING(2, "R1_TS1_SerialNumber"), r1_tempsensor_Id,
                              UA_NODEID_NUMERIC(0, UA_NS0ID_HASCOMPONENT),
                              UA_QUALIFIEDNAME(2, "Serial Number"),
                              UA_NODEID_NUMERIC(0, UA_NS0ID_BASEDATAVARIABLETYPE), snAttr, NULL, NULL);
    
    //add new variable temperature to the the server
    UA_VariableAttributes tempAttr = UA_VariableAttributes_default;
    UA_Variant_setScalar(&tempAttr.value, &temperature, &UA_TYPES[UA_TYPES_DOUBLE]);
    UA_Server_addVariableNode(server, UA_NODEID_STRING(2, "R1_TS1_Temperature"), r1_tempsensor_Id,
                              UA_NODEID_NUMERIC(0, UA_NS0ID_HASCOMPONENT),
                              UA_QUALIFIEDNAME(2, "Temperature"),
                              UA_NODEID_NUMERIC(0, UA_NS0ID_BASEDATAVARIABLETYPE), tempAttr, NULL, NULL);

    UA_ValueCallback callback ;
    callback.onRead = beforeReadTemperature;
    callback.onWrite = NULL;
    UA_Server_setVariableNode_valueCallback(server, UA_NODEID_STRING(2, "R1_TS1_Temperature"), callback);

//_______________________________________________LAMPE__________________________________________--
    //Add new object called Lampe to the Server
    UA_NodeId lamp_id; /* get the nodeid assigned by the server */
    UA_ObjectAttributes o_lamp_Attr = UA_ObjectAttributes_default;
    UA_Server_addObjectNode(server, UA_NODEID_STRING(2, "R1_Lampe1"),
                            UA_NODEID_NUMERIC(0, UA_NS0ID_OBJECTSFOLDER),
                            UA_NODEID_NUMERIC(0, UA_NS0ID_ORGANIZES),
                            UA_QUALIFIEDNAME(2, "Lampe"), UA_NODEID_NUMERIC(0, UA_NS0ID_BASEOBJECTTYPE),
                            o_lamp_Attr, NULL, &lamp_id);

    //Add new variable hersteller to the server
    UA_VariableAttributes herstellerAttr = UA_VariableAttributes_default;
    UA_String hersteller = UA_STRING("Philips");
    UA_Variant_setScalar(&herstellerAttr.value, &hersteller, &UA_TYPES[UA_TYPES_STRING]);
    UA_Server_addVariableNode(server, UA_NODEID_STRING(2, "R1_Lampe1_Hersteller"), lamp_id,
                              UA_NODEID_NUMERIC(0, UA_NS0ID_HASCOMPONENT),
                              UA_QUALIFIEDNAME(2, "Hersteller"),
                              UA_NODEID_NUMERIC(0, UA_NS0ID_BASEDATAVARIABLETYPE), herstellerAttr, NULL, NULL);
    
    //add new variable satus to the the server
    UA_VariableAttributes statusAttr = UA_VariableAttributes_default;
    UA_Boolean status = false;
    UA_Variant_setScalar(&statusAttr.value, &status, &UA_TYPES[UA_TYPES_BOOLEAN]);
    statusAttr.accessLevel = UA_ACCESSLEVELMASK_READ | UA_ACCESSLEVELMASK_WRITE;
    UA_Server_addVariableNode(server, UA_NODEID_STRING(2, "R1_Lampe1_Status(on/off)"), lamp_id,
                              UA_NODEID_NUMERIC(0, UA_NS0ID_HASCOMPONENT),
                              UA_QUALIFIEDNAME(2, "Status (On/Off)"),
                              UA_NODEID_NUMERIC(0, UA_NS0ID_BASEDATAVARIABLETYPE), statusAttr, NULL, NULL);

    UA_ValueCallback callback_lamp ;
    callback.onRead = NULL;
    callback.onWrite = afterWriteLicht;
    UA_Server_setVariableNode_valueCallback(server, UA_NODEID_STRING(2, "R1_Lampe1_Status(on/off)"), callback);



    UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND, "starting server....");
    UA_StatusCode retval = UA_Server_run(server, &running);

    UA_Server_delete(server);
    return retval == UA_STATUSCODE_GOOD ? EXIT_SUCCESS : EXIT_FAILURE;
}