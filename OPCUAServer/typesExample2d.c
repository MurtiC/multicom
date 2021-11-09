
#include "open62541/open62541.h"


#include <signal.h>
#include <stdlib.h>

static UA_NodeId pointTypeId;

static void
addVariableType2DPoint(UA_Server *server) {
    UA_VariableTypeAttributes vtAttr = UA_VariableTypeAttributes_default;
    vtAttr.dataType = UA_TYPES[UA_TYPES_DOUBLE].typeId;
    vtAttr.valueRank = UA_VALUERANK_ONE_DIMENSION;
    UA_UInt32 arrayDims[1] = {2};
    vtAttr.arrayDimensions = arrayDims;
    vtAttr.arrayDimensionsSize = 1;
    vtAttr.displayName = UA_LOCALIZEDTEXT("en-US", "2DPoint Type5445");

    /* a matching default value is required */
    UA_Double zero[2] = {0.0, 0.0};
    UA_Variant_setArray(&vtAttr.value, zero, 2, &UA_TYPES[UA_TYPES_DOUBLE]);
    
    UA_Server_addVariableTypeNode(
        server, 
        UA_NODEID_NULL,
        UA_NODEID_NUMERIC(0, UA_NS0ID_BASEDATAVARIABLETYPE),
        UA_NODEID_NUMERIC(0, UA_NS0ID_HASSUBTYPE),
        UA_QUALIFIEDNAME(1, "2DPoint Type5445"), UA_NODEID_NULL,
        vtAttr,
        NULL,
        &pointTypeId
        );
}
static UA_NodeId pointVariableId;

static void addVariable1(UA_Server *server) {
    /* Prepare the node attributes */
    UA_VariableAttributes vAttr = UA_VariableAttributes_default;
    vAttr.dataType = UA_TYPES[UA_TYPES_DOUBLE].typeId;
    vAttr.valueRank = UA_VALUERANK_ONE_DIMENSION;
    UA_UInt32 arrayDims[1] = {2};
    vAttr.arrayDimensions = arrayDims;
    vAttr.arrayDimensionsSize = 1;
    vAttr.displayName = UA_LOCALIZEDTEXT("en-US", "2DPoint Variable");
    vAttr.accessLevel = UA_ACCESSLEVELMASK_READ | UA_ACCESSLEVELMASK_WRITE;
    /* vAttr.value is left empty, the server instantiates with the default value */

    /* Add the node */
    UA_Server_addVariableNode(server, UA_NODEID_NULL,
                              UA_NODEID_NUMERIC(0, UA_NS0ID_OBJECTSFOLDER),
                              UA_NODEID_NUMERIC(0, UA_NS0ID_HASCOMPONENT),
                              UA_QUALIFIEDNAME(1, "2DPoint Type123"), pointTypeId,
                              vAttr, NULL, &pointVariableId);
}
static void writeVariable1(UA_Server *server) {
    UA_StatusCode retval = UA_Server_writeValueRank(server, pointVariableId, UA_VALUERANK_ONE_OR_MORE_DIMENSIONS);
    
    UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND,
                "Setting the Value Rank failed with Status Code %s",
                UA_StatusCode_name(retval));

}

static UA_NodeId typeGenericValueId;

static void addGenericValueType(UA_Server *server) {
    // Object Folder
    UA_ObjectTypeAttributes typeGenericValueAttr = UA_ObjectTypeAttributes_default;
    typeGenericValueAttr.displayName = UA_LOCALIZEDTEXT("en-US", "GenericValue");
    UA_Server_addObjectTypeNode(
        server,
        UA_NODEID_NULL,
        UA_NODEID_NUMERIC(0, UA_NS0ID_BASEOBJECTTYPE),
        UA_NODEID_NUMERIC(0, UA_NS0ID_HASSUBTYPE),
        UA_QUALIFIEDNAME(1, "GenericValue"),
        typeGenericValueAttr,
        NULL,
        &typeGenericValueId);

    // Variable Value
    UA_NodeId typeValueId;
    UA_VariableAttributes typeValueAttr = UA_VariableAttributes_default;
    typeValueAttr.displayName = UA_LOCALIZEDTEXT("en-US", "Value");
    typeValueAttr.dataType =  UA_TYPES[UA_TYPES_STRING].typeId;
    UA_Server_addVariableNode(
        server, 
        UA_NODEID_NULL, 
        typeGenericValueId,
        UA_NODEID_NUMERIC(0, UA_NS0ID_HASCOMPONENT),
        UA_QUALIFIEDNAME(1, "Value"),
        UA_NODEID_NUMERIC(0, UA_NS0ID_BASEDATAVARIABLETYPE),
        typeValueAttr,
        NULL,
        &typeValueId);
    // Make the Value mandatory
    UA_Server_addReference(
        server,
        typeValueId,
        UA_NODEID_NUMERIC(0, UA_NS0ID_HASMODELLINGRULE),
        UA_EXPANDEDNODEID_NUMERIC(0, UA_NS0ID_MODELLINGRULE_MANDATORY),
        true);

    // Variable Unit
    UA_NodeId typeUnitId;
    UA_VariableAttributes typeUnitAttr = UA_VariableAttributes_default;
    typeUnitAttr.displayName = UA_LOCALIZEDTEXT("en-US", "Unit");
    typeUnitAttr.dataType =  UA_TYPES[UA_TYPES_STRING].typeId;

    UA_Server_addVariableNode(
        server, 
        UA_NODEID_NULL, 
        typeGenericValueId,
        UA_NODEID_NUMERIC(0, UA_NS0ID_HASCOMPONENT),
        UA_QUALIFIEDNAME(1, "Unit"),
        UA_NODEID_NUMERIC(0, UA_NS0ID_BASEDATAVARIABLETYPE),
        typeUnitAttr,
        NULL,
        &typeUnitId);
       // Make the Unit mandatory
UA_Server_addReference(
        server,
        typeUnitId,
        UA_NODEID_NUMERIC(0, UA_NS0ID_HASMODELLINGRULE),
        UA_EXPANDEDNODEID_NUMERIC(0, UA_NS0ID_MODELLINGRULE_MANDATORY),
        true);

}

static UA_NodeId variableGenericValueId;

static void addGenericValueVariable(UA_Server *server) {
    /* Prepare the node attributes */
    UA_ObjectAttributes vAttr = UA_ObjectAttributes_default;
    vAttr.displayName = UA_LOCALIZEDTEXT("en-US", "GenericValue Variable");
    //vAttr.accessLevel = UA_ACCESSLEVELMASK_READ | UA_ACCESSLEVELMASK_WRITE;

    /* Add the node */
    UA_Server_addObjectNode(
        server, 
        UA_NODEID_NULL,
        UA_NODEID_NUMERIC(0, UA_NS0ID_OBJECTSFOLDER),
        UA_NODEID_NUMERIC(0, UA_NS0ID_HASCOMPONENT),
        UA_QUALIFIEDNAME(1, "GenericValue"), 
        typeGenericValueId,
        vAttr, 
        NULL, 
        &variableGenericValueId);
}