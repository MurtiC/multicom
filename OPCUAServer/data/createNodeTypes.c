#ifndef createNodeTypes_c
#define createNodeTypes_c

#include "dataStruct.c"
// #region[rgba(0,255,0,0.15)] AddNodeTemperaturSensorType
static void AddNodeVariableType(
    UA_Server *server,
    struct nodeTypes *types)
{

    UA_ObjectTypeAttributes FolderAttr = UA_ObjectTypeAttributes_default;
    FolderAttr.displayName = UA_LOCALIZEDTEXT("en-US", "VariableType");
    UA_Server_addObjectTypeNode(
        server,
        UA_NODEID_NULL,
        UA_NODEID_NUMERIC(0, UA_NS0ID_BASEOBJECTTYPE),
        UA_NODEID_NUMERIC(0, UA_NS0ID_HASSUBTYPE),
        UA_QUALIFIEDNAME(1, "VariableType"),
        FolderAttr,
        NULL,
        &(types->nodeVariable.Type));

    UA_VariableAttributes ValueAttr = UA_VariableAttributes_default;
    ValueAttr.valueRank = UA_VALUERANK_SCALAR;
    ValueAttr.displayName = UA_LOCALIZEDTEXT("en-US", "Value");
    UA_Server_addVariableNode(
        server,
        UA_NODEID_NULL,
        types->nodeVariable.Type,
        UA_NODEID_NUMERIC(0, UA_NS0ID_HASCOMPONENT),
        UA_QUALIFIEDNAME(1, "Value"),
        UA_NODEID_NUMERIC(0, UA_NS0ID_BASEDATAVARIABLETYPE),
        ValueAttr,
        NULL,
        &types->nodeVariable.Value);

    UA_VariableAttributes UnitAttr = UA_VariableAttributes_default;
    UnitAttr.valueRank = UA_VALUERANK_SCALAR;
    UnitAttr.displayName = UA_LOCALIZEDTEXT("en-US", "Unit");
    UA_Server_addVariableNode(
        server,
        UA_NODEID_NULL,
        types->nodeVariable.Type,
        UA_NODEID_NUMERIC(0, UA_NS0ID_HASCOMPONENT),
        UA_QUALIFIEDNAME(1, "Unit"),
        UA_NODEID_NUMERIC(0, UA_NS0ID_BASEDATAVARIABLETYPE),
        UnitAttr,
        NULL,
        &types->nodeVariable.Unit);
}
//#endregion

// #region[rgba(255,0,0,0.15)] AddNodeTemperaturSensorType
static void AddNodeTemperaturSensorType(
    UA_Server *server,
    struct nodeTypes *types)
{
    UA_ObjectTypeAttributes FolderAttr = UA_ObjectTypeAttributes_default;
    FolderAttr.displayName = UA_LOCALIZEDTEXT("en-US", "Temperatur Sensor Type");
    UA_Server_addObjectTypeNode(
        server,
        UA_NODEID_NULL,
        UA_NODEID_NUMERIC(0, UA_NS0ID_BASEOBJECTTYPE),
        UA_NODEID_NUMERIC(0, UA_NS0ID_HASSUBTYPE),
        UA_QUALIFIEDNAME(1, "Temperatur Sensor Type"),
        FolderAttr,
        NULL,
        &types->nodeTemperaturSensor.Type);

    UA_VariableAttributes NameAttr = UA_VariableAttributes_default;
    NameAttr.valueRank = UA_VALUERANK_SCALAR;
    NameAttr.displayName = UA_LOCALIZEDTEXT("en-US", "Name");

    UA_Server_addVariableNode(
        server,
        UA_NODEID_NULL,
        types->nodeTemperaturSensor.Type,
        UA_NODEID_NUMERIC(0, UA_NS0ID_HASCOMPONENT),
        UA_QUALIFIEDNAME(1, "Name"),
        UA_NODEID_NUMERIC(0, UA_NS0ID_BASEDATAVARIABLETYPE),
        NameAttr,
        NULL,
        &types->nodeTemperaturSensor.Name);

    UA_VariableAttributes DescriptionAttr = UA_VariableAttributes_default;
    DescriptionAttr.valueRank = UA_VALUERANK_SCALAR;
    DescriptionAttr.displayName = UA_LOCALIZEDTEXT("en-US", "Description");

    UA_Server_addVariableNode(
        server,
        UA_NODEID_NULL,
        types->nodeTemperaturSensor.Type,
        UA_NODEID_NUMERIC(0, UA_NS0ID_HASCOMPONENT),
        UA_QUALIFIEDNAME(1, "Description"),
        UA_NODEID_NUMERIC(0, UA_NS0ID_BASEDATAVARIABLETYPE),
        DescriptionAttr,
        NULL,
        &types->nodeTemperaturSensor.Description);

    UA_VariableAttributes LocationAttr = UA_VariableAttributes_default;
    LocationAttr.valueRank = UA_VALUERANK_SCALAR;
    LocationAttr.displayName = UA_LOCALIZEDTEXT("en-US", "Location");

    UA_Server_addVariableNode(
        server,
        UA_NODEID_NULL,
        types->nodeTemperaturSensor.Type,
        UA_NODEID_NUMERIC(0, UA_NS0ID_HASCOMPONENT),
        UA_QUALIFIEDNAME(1, "Location"),
        UA_NODEID_NUMERIC(0, UA_NS0ID_BASEDATAVARIABLETYPE),
        LocationAttr,
        NULL,
        &types->nodeTemperaturSensor.Location);

    UA_VariableAttributes TagIdAttr = UA_VariableAttributes_default;
    TagIdAttr.valueRank = UA_VALUERANK_SCALAR;
    TagIdAttr.displayName = UA_LOCALIZEDTEXT("en-US", "Tag Id");

    UA_Server_addVariableNode(
        server,
        UA_NODEID_NULL,
        types->nodeTemperaturSensor.Type,
        UA_NODEID_NUMERIC(0, UA_NS0ID_HASCOMPONENT),
        UA_QUALIFIEDNAME(1, "Tag Id"),
        UA_NODEID_NUMERIC(0, UA_NS0ID_BASEDATAVARIABLETYPE),
        TagIdAttr,
        NULL,
        &types->nodeTemperaturSensor.TagId);

    UA_ObjectAttributes TemperaturAttr = UA_ObjectAttributes_default;
    TemperaturAttr.displayName = UA_LOCALIZEDTEXT("en-US", "Temperatur");
    UA_Server_addObjectNode(
        server,
        UA_NODEID_NULL,
        types->nodeTemperaturSensor.Type,
        UA_NODEID_NUMERIC(0, UA_NS0ID_ORGANIZES),
        UA_QUALIFIEDNAME(1, "Temperatur"),
        types->nodeVariable.Type,
        TemperaturAttr,
        NULL,
        &types->nodeTemperaturSensor.Temperatur);

    UA_ObjectAttributes MaxTemperaturAttr = UA_ObjectAttributes_default;
    MaxTemperaturAttr.displayName = UA_LOCALIZEDTEXT("en-US", "Max Temperatur");
    UA_Server_addObjectNode(
        server,
        UA_NODEID_NULL,
        types->nodeTemperaturSensor.Type,
        UA_NODEID_NUMERIC(0, UA_NS0ID_ORGANIZES),
        UA_QUALIFIEDNAME(1, "Max Temperatur"),
        types->nodeVariable.Type,
        MaxTemperaturAttr,
        NULL,
        &types->nodeTemperaturSensor.MaxTemperatur);

    UA_ObjectAttributes MinTemperaturAttr = UA_ObjectAttributes_default;
    MinTemperaturAttr.displayName = UA_LOCALIZEDTEXT("en-US", "Min Temperatur");
    UA_Server_addObjectNode(
        server,
        UA_NODEID_NULL,
        types->nodeTemperaturSensor.Type,
        UA_NODEID_NUMERIC(0, UA_NS0ID_ORGANIZES),
        UA_QUALIFIEDNAME(1,  "Min Temperatur"),
        types->nodeVariable.Type,
        MinTemperaturAttr,
        NULL,
        &types->nodeTemperaturSensor.MinTemperatur);

    UA_VariableAttributes LastContactAttr = UA_VariableAttributes_default;
    LastContactAttr.valueRank = UA_VALUERANK_SCALAR;
    LastContactAttr.displayName = UA_LOCALIZEDTEXT("en-US", "Last Contact");
    UA_Server_addVariableNode(
        server,
        UA_NODEID_NULL,
        types->nodeTemperaturSensor.Type,
        UA_NODEID_NUMERIC(0, UA_NS0ID_HASCOMPONENT),
        UA_QUALIFIEDNAME(1, "Last Contact"),
        UA_NODEID_NUMERIC(0, UA_NS0ID_BASEDATAVARIABLETYPE),
        LastContactAttr,
        NULL,
        &types->nodeTemperaturSensor.LastContact);
}
//#endregion

// #region[rgba(0,0,255,0.15)] AddNodePinType
static void AddNodePinType(
    UA_Server *server,
    struct nodeTypes *types)
{

    UA_ObjectTypeAttributes FolderAttr = UA_ObjectTypeAttributes_default;
    FolderAttr.displayName = UA_LOCALIZEDTEXT("en-US", "IO Pin Type");
    UA_Server_addObjectTypeNode(
        server,
        UA_NODEID_NULL,
        UA_NODEID_NUMERIC(0, UA_NS0ID_BASEOBJECTTYPE),
        UA_NODEID_NUMERIC(0, UA_NS0ID_HASSUBTYPE),
        UA_QUALIFIEDNAME(1, "IO Pin Type"),
        FolderAttr,
        NULL,
        &types->nodePin.Type);

    UA_VariableAttributes NameAttr = UA_VariableAttributes_default;
    NameAttr.valueRank = UA_VALUERANK_SCALAR;
    NameAttr.displayName = UA_LOCALIZEDTEXT("en-US", "Name");

    UA_Server_addVariableNode(
        server,
        UA_NODEID_NULL,
        types->nodePin.Type,
        UA_NODEID_NUMERIC(0, UA_NS0ID_HASCOMPONENT),
        UA_QUALIFIEDNAME(1, "Name"),
        UA_NODEID_NUMERIC(0, UA_NS0ID_BASEDATAVARIABLETYPE),
        NameAttr,
        NULL,
        &types->nodePin.Name);

    UA_VariableAttributes DescriptionAttr = UA_VariableAttributes_default;
    DescriptionAttr.valueRank = UA_VALUERANK_SCALAR;
    DescriptionAttr.displayName = UA_LOCALIZEDTEXT("en-US", "Description");

    UA_Server_addVariableNode(
        server,
        UA_NODEID_NULL,
        types->nodePin.Type,
        UA_NODEID_NUMERIC(0, UA_NS0ID_HASCOMPONENT),
        UA_QUALIFIEDNAME(1, "Description"),
        UA_NODEID_NUMERIC(0, UA_NS0ID_BASEDATAVARIABLETYPE),
        DescriptionAttr,
        NULL,
        &types->nodePin.Description);

    UA_VariableAttributes LocationAttr = UA_VariableAttributes_default;
    LocationAttr.valueRank = UA_VALUERANK_SCALAR;
    LocationAttr.displayName = UA_LOCALIZEDTEXT("en-US", "Location");

    UA_Server_addVariableNode(
        server,
        UA_NODEID_NULL,
        types->nodePin.Type,
        UA_NODEID_NUMERIC(0, UA_NS0ID_HASCOMPONENT),
        UA_QUALIFIEDNAME(1, "Location"),
        UA_NODEID_NUMERIC(0, UA_NS0ID_BASEDATAVARIABLETYPE),
        LocationAttr,
        NULL,
        &types->nodePin.Location);

    UA_VariableAttributes PinAttr = UA_VariableAttributes_default;
    PinAttr.valueRank = UA_VALUERANK_SCALAR;
    PinAttr.displayName = UA_LOCALIZEDTEXT("en-US", "Pin");

    UA_Server_addVariableNode(
        server,
        UA_NODEID_NULL,
        types->nodePin.Type,
        UA_NODEID_NUMERIC(0, UA_NS0ID_HASCOMPONENT),
        UA_QUALIFIEDNAME(1, "Pin"),
        UA_NODEID_NUMERIC(0, UA_NS0ID_BASEDATAVARIABLETYPE),
        PinAttr,
        NULL,
        &types->nodePin.Pin);

    UA_VariableAttributes ModeAttr = UA_VariableAttributes_default;
    ModeAttr.valueRank = UA_VALUERANK_SCALAR;
    ModeAttr.displayName = UA_LOCALIZEDTEXT("en-US", "Mode");

    UA_Server_addVariableNode(
        server,
        UA_NODEID_NULL,
        types->nodePin.Type,
        UA_NODEID_NUMERIC(0, UA_NS0ID_HASCOMPONENT),
        UA_QUALIFIEDNAME(1, "Mode"),
        UA_NODEID_NUMERIC(0, UA_NS0ID_BASEDATAVARIABLETYPE),
        ModeAttr,
        NULL,
        &types->nodePin.Mode);

     UA_ObjectAttributes StatusAttr = UA_ObjectAttributes_default;
    StatusAttr.displayName = UA_LOCALIZEDTEXT("en-US", "Status");
    UA_Server_addObjectNode(
        server,
        UA_NODEID_NULL,
        types->nodePin.Type,
        UA_NODEID_NUMERIC(0, UA_NS0ID_ORGANIZES),
        UA_QUALIFIEDNAME(1,  "Status"),
        types->nodeVariable.Type,
        StatusAttr,
        NULL,
        &types->nodePin.Status);
}
//#endregion

// #region[rgba(255,255,0,0.15)] AddNodeRFIDReaderType
static void AddNodeRFIDReaderType(
     UA_Server *server,
    struct nodeTypes *types)
{

    UA_ObjectTypeAttributes FolderAttr = UA_ObjectTypeAttributes_default;
    FolderAttr.displayName = UA_LOCALIZEDTEXT("en-US", "RFID Reader Type");
    UA_Server_addObjectTypeNode(
        server,
        UA_NODEID_NULL,
        UA_NODEID_NUMERIC(0, UA_NS0ID_BASEOBJECTTYPE),
        UA_NODEID_NUMERIC(0, UA_NS0ID_HASSUBTYPE),
        UA_QUALIFIEDNAME(1, "RFID Reader Type"),
        FolderAttr,
        NULL,
        &types->nodeRFIDReader.Type);

    UA_VariableAttributes NameAttr = UA_VariableAttributes_default;
    NameAttr.valueRank = UA_VALUERANK_SCALAR;
    NameAttr.displayName = UA_LOCALIZEDTEXT("en-US", "Name");

    UA_Server_addVariableNode(
        server,
        UA_NODEID_NULL,
        types->nodeRFIDReader.Type,
        UA_NODEID_NUMERIC(0, UA_NS0ID_HASCOMPONENT),
        UA_QUALIFIEDNAME(1, "Name"),
        UA_NODEID_NUMERIC(0, UA_NS0ID_BASEDATAVARIABLETYPE),
        NameAttr,
        NULL,
        &types->nodeRFIDReader.Name);

    UA_VariableAttributes DescriptionAttr = UA_VariableAttributes_default;
    DescriptionAttr.valueRank = UA_VALUERANK_SCALAR;
    DescriptionAttr.displayName = UA_LOCALIZEDTEXT("en-US", "Description");

    UA_Server_addVariableNode(
        server,
        UA_NODEID_NULL,
        types->nodeRFIDReader.Type,
        UA_NODEID_NUMERIC(0, UA_NS0ID_HASCOMPONENT),
        UA_QUALIFIEDNAME(1, "Description"),
        UA_NODEID_NUMERIC(0, UA_NS0ID_BASEDATAVARIABLETYPE),
        DescriptionAttr,
        NULL,
        &types->nodeRFIDReader.Description);

    UA_VariableAttributes LocationAttr = UA_VariableAttributes_default;
    LocationAttr.valueRank = UA_VALUERANK_SCALAR;
    LocationAttr.displayName = UA_LOCALIZEDTEXT("en-US", "Location");

    UA_Server_addVariableNode(
        server,
        UA_NODEID_NULL,
        types->nodeRFIDReader.Type,
        UA_NODEID_NUMERIC(0, UA_NS0ID_HASCOMPONENT),
        UA_QUALIFIEDNAME(1, "Location"),
        UA_NODEID_NUMERIC(0, UA_NS0ID_BASEDATAVARIABLETYPE),
        LocationAttr,
        NULL,
        &types->nodeRFIDReader.Location);

    UA_VariableAttributes IPAdressAttr = UA_VariableAttributes_default;
    IPAdressAttr.valueRank = UA_VALUERANK_SCALAR;
    IPAdressAttr.displayName = UA_LOCALIZEDTEXT("en-US", "IPAdress");

    UA_Server_addVariableNode(
        server,
        UA_NODEID_NULL,
        types->nodeRFIDReader.Type,
        UA_NODEID_NUMERIC(0, UA_NS0ID_HASCOMPONENT),
        UA_QUALIFIEDNAME(1, "IPAdress"),
        UA_NODEID_NUMERIC(0, UA_NS0ID_BASEDATAVARIABLETYPE),
        IPAdressAttr,
        NULL,
        &types->nodeRFIDReader.IPAdress);

    UA_VariableAttributes PortAttr = UA_VariableAttributes_default;
    PortAttr.valueRank = UA_VALUERANK_SCALAR;
    PortAttr.displayName = UA_LOCALIZEDTEXT("en-US", "Port");

    UA_Server_addVariableNode(
        server,
        UA_NODEID_NULL,
        types->nodeRFIDReader.Type,
        UA_NODEID_NUMERIC(0, UA_NS0ID_HASCOMPONENT),
        UA_QUALIFIEDNAME(1, "Port"),
        UA_NODEID_NUMERIC(0, UA_NS0ID_BASEDATAVARIABLETYPE),
        PortAttr,
        NULL,
        &types->nodeRFIDReader.Port);

    UA_VariableAttributes SerialNumberAttr = UA_VariableAttributes_default;
    SerialNumberAttr.valueRank = UA_VALUERANK_SCALAR;
    SerialNumberAttr.displayName = UA_LOCALIZEDTEXT("en-US", "SerialNumber");

    UA_Server_addVariableNode(
        server,
        UA_NODEID_NULL,
        types->nodeRFIDReader.Type,
        UA_NODEID_NUMERIC(0, UA_NS0ID_HASCOMPONENT),
        UA_QUALIFIEDNAME(1, "SerialNumber"),
        UA_NODEID_NUMERIC(0, UA_NS0ID_BASEDATAVARIABLETYPE),
        SerialNumberAttr,
        NULL,
        &types->nodeRFIDReader.SerialNumber);

    UA_VariableAttributes HardwarerevisionAttr = UA_VariableAttributes_default;
    HardwarerevisionAttr.valueRank = UA_VALUERANK_SCALAR;
    HardwarerevisionAttr.displayName = UA_LOCALIZEDTEXT("en-US", "Hardwarerevision");

    UA_Server_addVariableNode(
        server,
        UA_NODEID_NULL,
        types->nodeRFIDReader.Type,
        UA_NODEID_NUMERIC(0, UA_NS0ID_HASCOMPONENT),
        UA_QUALIFIEDNAME(1, "Hardwarerevision"),
        UA_NODEID_NUMERIC(0, UA_NS0ID_BASEDATAVARIABLETYPE),
        HardwarerevisionAttr,
        NULL,
        &types->nodeRFIDReader.Hardwarerevision);

    UA_VariableAttributes SoftwarerevisionAttr = UA_VariableAttributes_default;
    SoftwarerevisionAttr.valueRank = UA_VALUERANK_SCALAR;
    SoftwarerevisionAttr.displayName = UA_LOCALIZEDTEXT("en-US", "Softwarerevision");

    UA_Server_addVariableNode(
        server,
        UA_NODEID_NULL,
        types->nodeRFIDReader.Type,
        UA_NODEID_NUMERIC(0, UA_NS0ID_HASCOMPONENT),
        UA_QUALIFIEDNAME(1, "Softwarerevision"),
        UA_NODEID_NUMERIC(0, UA_NS0ID_BASEDATAVARIABLETYPE),
        SoftwarerevisionAttr,
        NULL,
        &types->nodeRFIDReader.Softwarerevision);

     UA_ObjectAttributes InputPinsAttr = UA_ObjectAttributes_default;
    InputPinsAttr.displayName = UA_LOCALIZEDTEXT("en-US", "Input Pins");
    UA_Server_addObjectNode(
        server,
        UA_NODEID_NULL,
        types->nodeRFIDReader.Type,
        UA_NODEID_NUMERIC(0, UA_NS0ID_ORGANIZES),
        UA_QUALIFIEDNAME(1,  "Input Pins"),
        types->nodePin.Type,
        InputPinsAttr,
        NULL,
        &types->nodeRFIDReader.InputPins);

  UA_ObjectAttributes OutputPinsAttr = UA_ObjectAttributes_default;
    OutputPinsAttr.displayName = UA_LOCALIZEDTEXT("en-US", "Output Pins");
    UA_Server_addObjectNode(
        server,
        UA_NODEID_NULL,
        types->nodeRFIDReader.Type,
        UA_NODEID_NUMERIC(0, UA_NS0ID_ORGANIZES),
        UA_QUALIFIEDNAME(1,  "Output Pins"),
        types->nodePin.Type,
        OutputPinsAttr,
        NULL,
        &types->nodeRFIDReader.OutputPins);

}
//#endregion

// #region[rgba(0,255,255,0.15)] nodeTemperaturMonitoring

static void AddNodeTemperaturMonitoringType(
     UA_Server *server,
    struct nodeTypes *types)
{

  UA_ObjectTypeAttributes FolderAttr = UA_ObjectTypeAttributes_default;
    FolderAttr.displayName = UA_LOCALIZEDTEXT("en-US", "Temperatur Monitoring Type");
    UA_Server_addObjectTypeNode(
        server,
        UA_NODEID_NULL,
        UA_NODEID_NUMERIC(0, UA_NS0ID_BASEOBJECTTYPE),
        UA_NODEID_NUMERIC(0, UA_NS0ID_HASSUBTYPE),
        UA_QUALIFIEDNAME(1, "Temperatur Monitoring Type"),
        FolderAttr,
        NULL,
        &types->nodeTemperaturMonitoring.Type);


    UA_VariableAttributes NameAttr = UA_VariableAttributes_default;
    NameAttr.valueRank = UA_VALUERANK_SCALAR;
    NameAttr.dataType = UA_TYPES[UA_TYPES_STRING].typeId;
    NameAttr.displayName = UA_LOCALIZEDTEXT("en-US", "Name");

    UA_Server_addVariableNode(
        server,
        UA_NODEID_NULL,
        types->nodeTemperaturMonitoring.Type,
        UA_NODEID_NUMERIC(0, UA_NS0ID_HASCOMPONENT),
        UA_QUALIFIEDNAME(1, "Name"),
        UA_NODEID_NUMERIC(0, UA_NS0ID_BASEDATAVARIABLETYPE),
        NameAttr,
        NULL,
        &types->nodeTemperaturMonitoring.Name);

    UA_VariableAttributes DescriptionAttr = UA_VariableAttributes_default;
    DescriptionAttr.valueRank = UA_VALUERANK_SCALAR;
    DescriptionAttr.displayName = UA_LOCALIZEDTEXT("en-US", "Description");

    UA_Server_addVariableNode(
        server,
        UA_NODEID_NULL,
        types->nodeTemperaturMonitoring.Type,
        UA_NODEID_NUMERIC(0, UA_NS0ID_HASCOMPONENT),
        UA_QUALIFIEDNAME(1, "Description"),
        UA_NODEID_NUMERIC(0, UA_NS0ID_BASEDATAVARIABLETYPE),
        DescriptionAttr,
        NULL,
        &types->nodeTemperaturMonitoring.Description);

    UA_VariableAttributes LocationAttr = UA_VariableAttributes_default;
    LocationAttr.valueRank = UA_VALUERANK_SCALAR;
    LocationAttr.displayName = UA_LOCALIZEDTEXT("en-US", "Location");

    UA_Server_addVariableNode(
        server,
        UA_NODEID_NULL,
        types->nodeTemperaturMonitoring.Type,
        UA_NODEID_NUMERIC(0, UA_NS0ID_HASCOMPONENT),
        UA_QUALIFIEDNAME(1, "Location"),
        UA_NODEID_NUMERIC(0, UA_NS0ID_BASEDATAVARIABLETYPE),
        LocationAttr,
        NULL,
        &types->nodeTemperaturMonitoring.Location);

   UA_VariableAttributes VersionAttr = UA_VariableAttributes_default;
    VersionAttr.valueRank = UA_VALUERANK_SCALAR;
    VersionAttr.displayName = UA_LOCALIZEDTEXT("en-US", "Version");

    UA_Server_addVariableNode(
        server,
        UA_NODEID_NULL,
        types->nodeTemperaturMonitoring.Type,
        UA_NODEID_NUMERIC(0, UA_NS0ID_HASCOMPONENT),
        UA_QUALIFIEDNAME(1, "Version"),
        UA_NODEID_NUMERIC(0, UA_NS0ID_BASEDATAVARIABLETYPE),
        VersionAttr,
        NULL,
        &types->nodeTemperaturMonitoring.Version);

 UA_ObjectAttributes RFIDReaderAttr = UA_ObjectAttributes_default;
    RFIDReaderAttr.displayName = UA_LOCALIZEDTEXT("en-US", "RFID Reader");
    UA_Server_addObjectNode(
        server,
        UA_NODEID_NULL,
        types->nodeTemperaturMonitoring.Type,
        UA_NODEID_NUMERIC(0, UA_NS0ID_ORGANIZES),
        UA_QUALIFIEDNAME(1,  "RFID Reader"),
        types->nodeRFIDReader.Type,
        RFIDReaderAttr,
        NULL,
        &types->nodeTemperaturMonitoring.RFIDReader);
    
}

//#endregion


#endif