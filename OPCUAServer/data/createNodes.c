#ifndef createNodes_c
#define createNodes_c

#include "dataStruct.c"
#include "../open62541/open62541.h"
#include <signal.h>
#include <stdlib.h>

//#region [rgba(255,0,0,0.15)]
/**
 * @brief Adds a variable node with datatype STRING to the parrent node
 * @param server The UA_Server instance
 * @param parrent NodeId of the parrent node
 * @param newId Inserts the resulting nodeId of this node
 * @param name The name of the node
 * @param defaultValue The value of the node
 */ 
static void AddStringNode(
    UA_Server *server,
    UA_NodeId parrent,
    UA_NodeId *newId,
    char *name,
    char *defaultValue,
    bool write)
{
    // Name
    UA_VariableAttributes Attr = UA_VariableAttributes_default;
    if (write)
    {
        Attr.accessLevel = UA_ACCESSLEVELMASK_READ | UA_ACCESSLEVELMASK_WRITE;
    }
    else 
    {
        Attr.accessLevel = UA_ACCESSLEVELMASK_READ;
    }
    Attr.displayName = UA_LOCALIZEDTEXT("en-US", name);
    Attr.dataType = UA_TYPES[UA_TYPES_STRING].typeId;
    UA_String Text = UA_STRING(defaultValue);
    UA_Variant_setScalar(
        &Attr.value,
        &Text,
        &UA_TYPES[UA_TYPES_STRING]);

    UA_Server_addVariableNode(
        server,
        UA_NODEID_NULL,
        parrent,
        UA_NODEID_NUMERIC(0, UA_NS0ID_HASCOMPONENT),
        UA_QUALIFIEDNAME(1, name),
        UA_NODEID_NUMERIC(0, UA_NS0ID_BASEDATAVARIABLETYPE),
        Attr,
        NULL,
        newId);
}
/**
 * @brief Adds a variable node with datatype DATETIME to the parrent node
 * @param server The UA_Server instance
 * @param parrent NodeId of the parrent node
 * @param newId Inserts the resulting nodeId of this node
 * @param name The name of the node
 * @param defaultValue The value of the node
 */ 
static void AddDateNode(
    UA_Server *server,
    UA_NodeId parrent,
    UA_NodeId *newId,
    char *name,
    UA_DateTime defaultValue,
    bool write)
{
    UA_VariableAttributes Attr = UA_VariableAttributes_default;
    if (write)
    {
        Attr.accessLevel = UA_ACCESSLEVELMASK_READ | UA_ACCESSLEVELMASK_WRITE;
    }
    else 
    {
        Attr.accessLevel = UA_ACCESSLEVELMASK_READ;
    }
    Attr.displayName = UA_LOCALIZEDTEXT("en-US", name);
    Attr.dataType = UA_TYPES[UA_TYPES_DATETIME].typeId;
    UA_DateTime Text;
    UA_Variant_setScalar(
        &Attr.value,
        &defaultValue,
        &UA_TYPES[UA_TYPES_DATETIME]);

    UA_Server_addVariableNode(
        server,
        UA_NODEID_NULL,
        parrent,
        UA_NODEID_NUMERIC(0, UA_NS0ID_HASCOMPONENT),
        UA_QUALIFIEDNAME(1, name),
        UA_NODEID_NUMERIC(0, UA_NS0ID_BASEDATAVARIABLETYPE),
        Attr,
        NULL,
        newId);
}
/**
 * @brief Adds a variable node with datatype INT32 to the parrent node
 * @param server The UA_Server instance
 * @param parrent NodeId of the parrent node
 * @param newId Inserts the resulting nodeId of this node
 * @param name The name of the node
 * @param defaultValue The value of the node
 */
static void AddIntNode(
    UA_Server *server,
    UA_NodeId parrent,
    UA_NodeId *newId,
    char *name,
    int defaultValue,
    bool write)
{
    UA_VariableAttributes Attr = UA_VariableAttributes_default;
    if (write)
    {
        Attr.accessLevel = UA_ACCESSLEVELMASK_READ | UA_ACCESSLEVELMASK_WRITE;
    }
    else 
    {
        Attr.accessLevel = UA_ACCESSLEVELMASK_READ;
    }
    Attr.displayName = UA_LOCALIZEDTEXT("en-US", name);
    Attr.dataType = UA_TYPES[UA_TYPES_INT32].typeId;
    UA_Variant_setScalar(
        &Attr.value,
        &defaultValue,
        &UA_TYPES[UA_TYPES_INT32]);

    UA_Server_addVariableNode(
        server,
        UA_NODEID_NULL,
        parrent,
        UA_NODEID_NUMERIC(0, UA_NS0ID_HASCOMPONENT),
        UA_QUALIFIEDNAME(1, name),
        UA_NODEID_NUMERIC(0, UA_NS0ID_BASEDATAVARIABLETYPE),
        Attr,
        NULL,
        newId);
}
/**
 * @brief Adds a variable node with datatype BOOL to the parrent node
 * @param server The UA_Server instance
 * @param parrent NodeId of the parrent node
 * @param newId Inserts the resulting nodeId of this node
 * @param name The name of the node
 * @param defaultValue The value of the node
 */
static void AddBoolNode(
    UA_Server *server,
    UA_NodeId parrent,
    UA_NodeId *newId,
    char *name,
    bool defaultValue,
    bool write)
{
    UA_VariableAttributes Attr = UA_VariableAttributes_default;
    if (write)
    {
        Attr.accessLevel = UA_ACCESSLEVELMASK_READ | UA_ACCESSLEVELMASK_WRITE;
    }
    else 
    {
        Attr.accessLevel = UA_ACCESSLEVELMASK_READ;
    }
    Attr.displayName = UA_LOCALIZEDTEXT("en-US", name);
    Attr.dataType = UA_TYPES[UA_TYPES_BOOLEAN].typeId;
    UA_Variant_setScalar(
        &Attr.value,
        &defaultValue,
        &UA_TYPES[UA_TYPES_BOOLEAN ]);

    UA_Server_addVariableNode(
        server,
        UA_NODEID_NULL,
        parrent,
        UA_NODEID_NUMERIC(0, UA_NS0ID_HASCOMPONENT),
        UA_QUALIFIEDNAME(1, name),
        UA_NODEID_NUMERIC(0, UA_NS0ID_BASEDATAVARIABLETYPE),
        Attr,
        NULL,
        newId);
}

/**
 * @brief Adds a variable node with datatype DOUBLE to the parrent node
 * @param server The UA_Server instance
 * @param parrent NodeId of the parrent node
 * @param newId Inserts the resulting nodeId of this node
 * @param name The name of the node
 * @param defaultValue The value of the node
 */
static void AddDoubleNode(
    UA_Server *server,
    UA_NodeId parrent,
    UA_NodeId *newId,
    char *name,
    double defaultValue,
    bool write)
{
    UA_VariableAttributes Attr = UA_VariableAttributes_default;
    if (write)
    {
        Attr.accessLevel = UA_ACCESSLEVELMASK_READ | UA_ACCESSLEVELMASK_WRITE;
    }
    else 
    {
        Attr.accessLevel = UA_ACCESSLEVELMASK_READ;
    }
    Attr.displayName = UA_LOCALIZEDTEXT("en-US", name);
    Attr.dataType = UA_TYPES[UA_TYPES_DOUBLE].typeId;
    UA_Variant_setScalar(
        &Attr.value,
        &defaultValue,
        &UA_TYPES[UA_TYPES_DOUBLE ]);

    UA_Server_addVariableNode(
        server,
        UA_NODEID_NULL,
        parrent,
        UA_NODEID_NUMERIC(0, UA_NS0ID_HASCOMPONENT),
        UA_QUALIFIEDNAME(1, name),
        UA_NODEID_NUMERIC(0, UA_NS0ID_BASEDATAVARIABLETYPE),
        Attr,
        NULL,
        newId);
}
//#endregion

//#region [rgba(0,255,0,0.15)]
/**
 * @brief Adds variable nodes of Type NodePin to parrent node
 * @param server The UA_Server instance
 * @param parrent NodeId of the parrent node
 * @param types The types definitions struct
 * @param variables The struct for the resulting nodeIds
 * @param name The name of the node
 */
static void AddNodeVariable(
    UA_Server *server,
    UA_NodeId parrent,
    struct nodeTypes *types,
    struct nodeVariable *variables,
    char *name,
    bool write) {

    UA_ObjectAttributes oAttr = UA_ObjectAttributes_default;
    oAttr.displayName = UA_LOCALIZEDTEXT("en-US", name);
    UA_Server_addObjectNode(
        server,
        UA_NODEID_NULL,
        parrent,
        UA_NODEID_NUMERIC(0, UA_NS0ID_ORGANIZES),
        UA_QUALIFIEDNAME(1, name),
        types->nodeVariable.Type,
        oAttr,
        NULL,
        &variables->Folder);

    AddStringNode(
         server,
        variables->Folder,
        &(variables->Unit),
        "Unit",
        "",
        false);
    AddDoubleNode( server,
        variables->Folder,
        &(variables->Value),
        "Value",
        0.00,
        write);


}
//#endregion

//#region [rgba(255,255,0,0.15)]
/**
 * @brief Adds variable nodes of Type NodeTemperaturSensor to parrent node
 * @param server The UA_Server instance
 * @param parrent NodeId of the parrent node
 * @param types The types definitions struct
 * @param variables The struct for the resulting nodeIds
 * @param name The name of the node
 */ 
static void AddNodeTemperaturSensor(
    UA_Server *server,
    UA_NodeId parrent,
    struct nodeTypes *types,
    struct nodeTemperaturSensor *variables,
    char *name) 
{
    UA_ObjectAttributes oAttr = UA_ObjectAttributes_default;
    oAttr.displayName = UA_LOCALIZEDTEXT("en-US", name);
    UA_Server_addObjectNode(
        server,
        UA_NODEID_NULL,
        parrent,
        UA_NODEID_NUMERIC(0, UA_NS0ID_ORGANIZES),
        UA_QUALIFIEDNAME(1, name),
        types->nodeTemperaturSensor.Type,
        oAttr,
        NULL,
        &variables->Folder);

    // Name
    AddStringNode(
        server,
        variables->Folder,
        &(variables->Name),
        "Name",
        "",
        true);

    AddStringNode(
        server,
        variables->Folder,
        &(variables->Description),
        "Description",
        "",
        true);

    AddStringNode(
        server,
        variables->Folder,
        &(variables->Location),
        "Location",
        "",
        true);
    
    AddIntNode(
        server,
        variables->Folder,
        &(variables->Antenna),
        "Antenna",
        0,
        false);
    
     AddNodeVariable(
        server,
        variables->Folder,
        types,
        &(variables->Temperatur),
        "Temperatur",
        false);
    AddNodeVariable(
        server,
        variables->Folder,
        types,
        &(variables->MaxTemperatur),
        "MaxTemperatur",
        true);
     AddNodeVariable(
        server,
        variables->Folder,
        types,
        &(variables->MinTemperatur),
        "MinTemperatur",
        true);

}
//#endregion

//#region [rgba(0,255,255,0.15)]
/**
 * @brief Adds variable nodes of Type NodePin to parrent node
 * @param server The UA_Server instance
 * @param parrent NodeId of the parrent node
 * @param types The types definitions struct
 * @param variables The struct for the resulting nodeIds
 * @param name The name of the node
 */ 
static void AddNodePin(
    UA_Server *server,
    UA_NodeId parrent,
    struct nodeTypes *types,
    struct nodePin *variables,
    char *name,
    int output) 
{
    UA_ObjectAttributes oAttr = UA_ObjectAttributes_default;
    oAttr.displayName = UA_LOCALIZEDTEXT("en-US", name);
    UA_Server_addObjectNode(
        server,
        UA_NODEID_NULL,
        parrent,
        UA_NODEID_NUMERIC(0, UA_NS0ID_ORGANIZES),
        UA_QUALIFIEDNAME(1, name),
        types->nodeRFIDReader.Type,
        oAttr,
        NULL,
        &variables->Folder);

    // Name
    AddStringNode(
        server,
        variables->Folder,
        &(variables->Name),
        "Name",
        "",
        true);

    AddStringNode(
        server,
        variables->Folder,
        &(variables->Description),
        "Description",
        "",
        true);

    AddStringNode(
        server,
        variables->Folder,
        &(variables->Location),
        "Location",
        "",
        true);
    AddIntNode(
        server,
        variables->Folder,
        &(variables->Pin),
        "Pin",
        0,
        false);
    AddBoolNode(
        server,
        variables->Folder,
        &(variables->Pin),
        "Mode",
        false,
        false);


    AddNodeVariable(
        server,
        variables->Folder,
        types,
        &(variables->Status),
        "Status",
        output);

}
//#endregion

//#region [rgba(0,0,255,0.15)]
/**
 * @brief Adds variable nodes of Type RFIDReader to parrent node
 * @param server The UA_Server instance
 * @param parrent NodeId of the parrent node
 * @param types The types definitions struct
 * @param variables The struct for the resulting nodeIds
 * @param name The name of the node
 */ 
static void AddNodeRFIDReader(
    UA_Server *server,
    UA_NodeId parrent,
    struct nodeTypes *types,
    struct nodeRFIDReader *variables,
    char *name)
{
    UA_ObjectAttributes oAttr = UA_ObjectAttributes_default;
    oAttr.displayName = UA_LOCALIZEDTEXT("en-US", name);
    UA_Server_addObjectNode(
        server,
        UA_NODEID_NULL,
        parrent,
        UA_NODEID_NUMERIC(0, UA_NS0ID_ORGANIZES),
        UA_QUALIFIEDNAME(1, name),
        types->nodeRFIDReader.Type,
        oAttr,
        NULL,
        &variables->Folder);

    // Name
    AddStringNode(
        server,
        variables->Folder,
        &(variables->Name),
        "Name",
        "",
        true);

    AddStringNode(
        server,
        variables->Folder,
        &(variables->Description),
        "Description",
        "",
        true);

    AddStringNode(
        server,
        variables->Folder,
        &(variables->Location),
        "Location",
        "",
        true);

    AddStringNode(
        server,
        variables->Folder,
        &(variables->IPAdress),
        "IPAdress",
        "",
        false);
    AddIntNode(
        server,
        variables->Folder,
        &(variables->IPAdress),
        "Port",
        0,
        false);
    AddStringNode(
        server,
        variables->Folder,
        &(variables->SerialNumber),
        "SerialNumber",
        "",
        false);
    AddStringNode(
        server,
        variables->Folder,
        &(variables->Hardwarerevision),
        "Hardwarerevision",
        "",
        false);
    AddStringNode(
        server,
        variables->Folder,
        &(variables->Softwarerevision),
        "Softwarerevision",
        "",
        false);
    
    for (int a=0; a < sLesegeraetPinInputs;a++) {
        char name[32];
        sprintf(name,"Input %ld", a);
        AddNodePin( server,
        variables->Folder,
        types,
        &(variables->InputPins[a]),
        name,
        false);
    }

    for (int a=0; a < sLesegeraetPinOutput;a++) {
        char name[32];
        sprintf(name,"Output %ld", a);
        AddNodePin( server,
        variables->Folder,
        types,
        &(variables->OutputPins[a]),
        name,
        true);
    }

    // TemperaturSensorNodes are generated at runtime !

}
//#endregion

//#region [rgba(255,255,0,0.15)]
/**
 * @brief Adds variable nodes of Type TemperaturMonitoring to parrent node
 * @param server The UA_Server instance
 * @param parrent NodeId of the parrent node
 * @param types The types definitions struct
 * @param variables The struct for the resulting nodeIds
 * @param name The name of the node
 */ 
static void AddNodeTemperaturMonitoring(
    UA_Server *server,
    UA_NodeId parrent,
    struct nodeTypes *types,
    struct nodeTemperaturMonitoring *variables,
    char *name)
{
    UA_ObjectAttributes oAttr = UA_ObjectAttributes_default;
    oAttr.displayName = UA_LOCALIZEDTEXT("en-US", name);
    UA_Server_addObjectNode(
        server,
        UA_NODEID_NULL,
        parrent,
        UA_NODEID_NUMERIC(0, UA_NS0ID_ORGANIZES),
        UA_QUALIFIEDNAME(1, name),
        types->nodeTemperaturMonitoring.Type,
        oAttr,
        NULL,
        &variables->Folder);

    AddStringNode(
        server,
        variables->Folder,
        &(variables->Name),
        "Name",
        "",
        true);

    AddStringNode(
        server,
        variables->Folder,
        &(variables->Description),
        "Description",
        "",
        true);

    AddStringNode(
        server,
        variables->Folder,
        &(variables->Location),
        "Location",
        "",
        true);

    AddStringNode(
        server,
        variables->Folder,
        &(variables->Version),
        "Version",
        "",
        false);

    UA_DateTime LastContactText;
    AddDateNode(
        server,
        variables->Folder,
        &(variables->LastContact),
        "LastContact",
        LastContactText,
        false);

    AddNodeRFIDReader(
        server,
        variables->Folder,
        types,
        &(variables->Reader),
        "Reader");
}
//#endregion



#endif