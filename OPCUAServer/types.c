#ifndef types_C
    #define types_C

#include <signal.h>
#include <stdlib.h>

#include "readFile.c"

#include "open62541/open62541.h"

static UA_StatusCode readTemperatur(
    UA_Server *server,
    const UA_NodeId *sessionId,
    void *sessionContext,
    const UA_NodeId *nodeId,
    void *nodeContext,
    UA_Boolean sourceTimeStamp,
    const UA_NumericRange *range,
    UA_DataValue *dataValue) {

    //UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND, "The variable was updated");

    char tid[24] = "                        ";
    char *tidd = (nodeId)->identifier.string.data;
    
     strcpy(tid, tidd);

    
    
    UA_Double temperatur = -300.0;
    //"/history/TAGIDcurrent.csv";

    int currentCSVRows = 2;
    int currentCSVColums = 3;
    int currentCSVLength = 256;
    char currentCSV[currentCSVRows][currentCSVColums][currentCSVLength];
    
    char path[500];
    strcpy(path,"/history/");
    strncat(path,tid,24);
    strcat(path,"current.csv");
    

    if (getCSV(path,currentCSVRows,
           currentCSVColums,
           currentCSVLength,
           currentCSV) > 0) 
    {
        int temperaturCol = getCSVColum(currentCSVRows,
           currentCSVColums,
           currentCSVLength,
           currentCSV,"Temperature");
           //UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND, "Find Temperature in CSV: %d",temperaturCol);
        if (temperaturCol >= 0) {
            char *eptr;
            temperatur = strtod(currentCSV[1][temperaturCol],&eptr);
        }
        else {
            //UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND, "Cannot Find Temperature in CSV: %s",temperaturCol);
        }
    }

     UA_Variant_setScalarCopy(&dataValue->value, &temperatur,
                             &UA_TYPES[UA_TYPES_DOUBLE]);

      return UA_STATUSCODE_GOOD;
}

static void manuallyDefineTemptag(
    UA_Server *server,
    char *tagName,
    char *tagId,
    char* Location,
    double upperlimit,
    double lowerlimit)
{

    // Node Name
    UA_NodeId tagTypeId;
    UA_ObjectAttributes oAttr = UA_ObjectAttributes_default;
    oAttr.displayName = UA_LOCALIZEDTEXT("en-US", tagName);
    UA_Server_addObjectNode(
        server, 
        UA_NODEID_NULL,
        UA_NODEID_NUMERIC(0, UA_NS0ID_OBJECTSFOLDER),
        UA_NODEID_NUMERIC(0, UA_NS0ID_ORGANIZES),
        UA_QUALIFIEDNAME(1, tagName), 
        UA_NODEID_NUMERIC(0, UA_NS0ID_BASEOBJECTTYPE),
        oAttr,
        NULL, 
        &tagTypeId);

    // TID
     UA_NodeId tidTypeId;
    UA_VariableAttributes tidAttr = UA_VariableAttributes_default;
    UA_String tagidName = UA_STRING(tagId);
    tidAttr.accessLevel = UA_ACCESSLEVELMASK_READ | UA_ACCESSLEVELMASK_WRITE;
    UA_Variant_setScalar(&tidAttr.value, &tagidName, &UA_TYPES[UA_TYPES_STRING]);
    tidAttr.displayName = UA_LOCALIZEDTEXT("en-US", "TId");
    UA_Server_addVariableNode(
        server, 
        UA_NODEID_NULL,
        tagTypeId,
        UA_NODEID_NUMERIC(0, UA_NS0ID_HASCOMPONENT),
        UA_QUALIFIEDNAME(1, "TId"),
        UA_NODEID_NUMERIC(0, UA_NS0ID_BASEDATAVARIABLETYPE),
        tidAttr,
        NULL, 
        &tidTypeId);


    // Temperatur
  /*  
    UA_VariableAttributes temperaturAttr = UA_VariableAttributes_default;
     temperaturAttr.accessLevel = UA_ACCESSLEVELMASK_READ;
    UA_Double temperatur = -300.0;
    UA_Variant_setScalar(&temperaturAttr.value, &temperatur, &UA_TYPES[UA_TYPES_DOUBLE]);
    temperaturAttr.displayName = UA_LOCALIZEDTEXT("en-US", "Temperatur");
    UA_Server_addVariableNode(
        server,
        UA_NODEID_NULL,
        tagTypeId,
        UA_NODEID_NUMERIC(0, UA_NS0ID_HASCOMPONENT),
        UA_QUALIFIEDNAME(1, "Temperatur"),
        UA_NODEID_NUMERIC(0, UA_NS0ID_BASEDATAVARIABLETYPE),
        temperaturAttr, 
        NULL,
        &temperaturTypeId);*/
    UA_NodeId temperaturTypeId =  UA_NODEID_STRING(1, tagId);
     UA_VariableAttributes temperaturAttr = UA_VariableAttributes_default;
    temperaturAttr.displayName = UA_LOCALIZEDTEXT("en-US", "Temperatur");
    temperaturAttr.accessLevel = UA_ACCESSLEVELMASK_READ;

    UA_QualifiedName currentName = UA_QUALIFIEDNAME(1, "current-time-datasource");
    UA_NodeId parentNodeId = UA_NODEID_NUMERIC(0, UA_NS0ID_OBJECTSFOLDER);
    UA_NodeId parentReferenceNodeId = UA_NODEID_NUMERIC(0, UA_NS0ID_ORGANIZES);
    UA_NodeId variableTypeNodeId = UA_NODEID_NUMERIC(0, UA_NS0ID_BASEDATAVARIABLETYPE);

     UA_DataSource timeDataSource;
    timeDataSource.read = readTemperatur;
    UA_Server_addDataSourceVariableNode(server, temperaturTypeId, tagTypeId,
                                        parentReferenceNodeId, currentName,
                                        variableTypeNodeId, temperaturAttr,
                                        timeDataSource, NULL, NULL);



    /* UA_ValueCallback temperaturcallback;
    temperaturcallback.onRead = beforeTemperaturRead;
    //callback.onWrite = afterWriteTime;
    UA_Server_setVariableNode_valueCallback(server, temperaturTypeId, temperaturcallback);
   */
    // Upper Temperatur limit
    UA_NodeId upperTemperaturTypeId;
    UA_VariableAttributes uppertemperaturAttr = UA_VariableAttributes_default;
    uppertemperaturAttr.accessLevel = UA_ACCESSLEVELMASK_READ | UA_ACCESSLEVELMASK_WRITE;
    UA_Variant_setScalar(&uppertemperaturAttr.value, &upperlimit, &UA_TYPES[UA_TYPES_DOUBLE]);
    uppertemperaturAttr.displayName = UA_LOCALIZEDTEXT("en-US", "Upper Temperatur limit");
    UA_Server_addVariableNode(
        server,
        UA_NODEID_NULL,
        tagTypeId,
        UA_NODEID_NUMERIC(0, UA_NS0ID_HASCOMPONENT),
        UA_QUALIFIEDNAME(1, "Upper Temperatur limit"),
        UA_NODEID_NUMERIC(0, UA_NS0ID_BASEDATAVARIABLETYPE),
        uppertemperaturAttr, 
        NULL,
        &upperTemperaturTypeId);
    // Lower Temperatur limit
    UA_NodeId lowerTemperaturTypeId;
    UA_VariableAttributes lowertemperaturAttr = UA_VariableAttributes_default;
    lowertemperaturAttr.accessLevel = UA_ACCESSLEVELMASK_READ | UA_ACCESSLEVELMASK_WRITE;
    UA_Variant_setScalar(&lowertemperaturAttr.value, &lowerlimit, &UA_TYPES[UA_TYPES_DOUBLE]);
    lowertemperaturAttr.displayName = UA_LOCALIZEDTEXT("en-US", "Lower Temperatur limit");
    UA_Server_addVariableNode(
        server,
        UA_NODEID_NULL,
        tagTypeId,
        UA_NODEID_NUMERIC(0, UA_NS0ID_HASCOMPONENT),
        UA_QUALIFIEDNAME(1, "Lower Temperatur limit"),
        UA_NODEID_NUMERIC(0, UA_NS0ID_BASEDATAVARIABLETYPE),
        lowertemperaturAttr, 
        NULL,
        &lowerTemperaturTypeId);

    /*
    //
    UA_VariableAttributes statusAttr = UA_VariableAttributes_default;
    UA_Boolean status = true;
    UA_Variant_setScalar(&statusAttr.value, &status, &UA_TYPES[UA_TYPES_BOOLEAN]);
    statusAttr.displayName = UA_LOCALIZEDTEXT("en-US", "Status");
    UA_Server_addVariableNode(server, UA_NODEID_NULL, tagTypeId,
                              UA_NODEID_NUMERIC(0, UA_NS0ID_HASCOMPONENT),
                              UA_QUALIFIEDNAME(1, "Status"),
                              UA_NODEID_NUMERIC(0, UA_NS0ID_BASEDATAVARIABLETYPE), statusAttr, NULL, NULL);
    */
  
}



static void updatetemptags(UA_Server *server, void *data) {
/*

    UA_DateTime now = UA_DateTime_now();
    UA_Variant value;
    UA_Variant_setScalar(&value, &now, &UA_TYPES[UA_TYPES_DATETIME]);
    UA_NodeId currentNodeId = UA_NODEID_STRING(1, "current-time-value-callback");
    UA_Server_writeValue(server, currentNodeId, value);
*/
}

#endif


