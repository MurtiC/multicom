
#include <signal.h>
#include <stdlib.h>

#include "open62541/open62541.h"

#include "nodes.c"
//#include "types.c"
//#include "typeExample.c"
#include "typeExample2.c"
//#include "typesExample2d.c"

#include "readFile.c"

#include "data/createNodes.c"
#include "data/createNodeTypes.c"
#include "data/dataStruct.c"
#include "data/updateNodes.c"

#include "data/nodeset.c"

static volatile UA_Boolean running = true;
static void stopHandler(int sig)
{
    UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND, "received ctrl-c");
    running = false;
}

void updateEverySecond(UA_Server *server, void *data)
{
    UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND, "updateEverySecond");
    // struct nodeRFIDReader* variables = (struct nodeRFIDReader*)data;
    UpdateDynamicNodes(server, false);
}

static UA_StatusCode AddTemperaturSensor(
    UA_Server *server,
    const UA_NodeId *sessionId, void *sessionHandle,
    const UA_NodeId *methodId, void *methodContext,
    const UA_NodeId *objectId, void *objectContext,
    size_t inputSize, const UA_Variant *input,
    size_t outputSize, UA_Variant *output)
{
    UA_Variant inputVariant = *input;
    char inputText[50];
    UA_VariantToString(&inputVariant, inputText);
    UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND, "AddTemperaturSensor:%s", inputText);

    if (strlen(inputText) != 24)
    {
        // return;
    }
    // Create Folder
    char relpath[255];
    strcpy(relpath, "/files/sensoren/");
    strcat(relpath, inputText);
    createFolder(relpath);
    // Appand runtimeConfig
    char configPath[255];
    strcpy(configPath, relpath);
    strcat(configPath, "/config.csv");
    char currentPath[255];
    strcpy(currentPath, relpath);
    strcat(currentPath, "/current.csv");

    struct myNode Folder;
    CreateNode(&Folder);
    Folder.Type = Object;
    strcpy(Folder.Name, inputText);
    strcpy(Folder.DisplayName, inputText);
    strcpy(Folder.Parrent, "Temperatursensoren");

    char currentName[100];
    char nodeName[100];
    char ident[100];
    strcpy(ident, "TagId");

    // Name
    struct myNode Name;
    CreateNode(&Name);
    strcpy(nodeName, "Name");
    strcpy(currentName, inputText);
    strcat(currentName, nodeName);
    Name.Type = String;
    strcpy(Name.Name, currentName);
    strcpy(Name.DisplayName, currentName);
    strcpy(Name.Parrent, Folder.Name);
    Name.Write = true;
    strcpy(Name.CSVName, configPath);
    strcpy(Name.CSVIdentifierColum, ident);
    strcpy(Name.CSVIdentifier, inputText);
    strcpy(Name.CSVValueColum, nodeName);

    // Description
    struct myNode Description;
    CreateNode(&Description);
    strcpy(nodeName, "Description");
    strcpy(currentName, inputText);
    strcat(currentName, nodeName);
    Description.Type = String;
    strcpy(Description.Name, currentName);
    strcpy(Description.DisplayName, nodeName);
    strcpy(Description.Parrent, Folder.Name);
    Description.Write = true;
    strcpy(Description.CSVName, configPath);
    strcpy(Description.CSVIdentifierColum, ident);
    strcpy(Description.CSVIdentifier, inputText);
    strcpy(Description.CSVValueColum, nodeName);

    // Location
    struct myNode Location;
    CreateNode(&Location);
    strcpy(nodeName, "Location");
    strcpy(currentName, inputText);
    strcat(currentName, nodeName);
    Location.Type = String;
    strcpy(Location.Name, currentName);
    strcpy(Location.DisplayName, nodeName);
    strcpy(Location.Parrent, Folder.Name);
    Location.Write = true;
    strcpy(Location.CSVName, configPath);
    strcpy(Location.CSVIdentifierColum, ident);
    strcpy(Location.CSVIdentifier, inputText);
    strcpy(Location.CSVValueColum, nodeName);

    // Antenna
    struct myNode Antenna;
    CreateNode(&Antenna);
    strcpy(nodeName, "Antenna");
    strcpy(currentName, inputText);
    strcat(currentName, nodeName);
    Antenna.Type = Integer;
    strcpy(Antenna.Name, currentName);
    strcpy(Antenna.DisplayName, nodeName);
    strcpy(Antenna.Parrent, Folder.Name);
    Antenna.Write = true;
    strcpy(Antenna.CSVName, configPath);
    strcpy(Antenna.CSVIdentifierColum, ident);
    strcpy(Antenna.CSVIdentifier, inputText);
    strcpy(Antenna.CSVValueColum, nodeName);

    // Temperatur
    struct myNode Temperatur;
    CreateNode(&Temperatur);
    strcpy(nodeName, "Temperatur");
    strcpy(currentName, inputText);
    strcat(currentName, nodeName);
    Temperatur.Type = Double;
    strcpy(Temperatur.Name, currentName);
    strcpy(Temperatur.DisplayName, nodeName);
    strcpy(Temperatur.Parrent, Folder.Name);
    Temperatur.Write = false;
    strcpy(Temperatur.CSVName, currentPath);
    strcpy(Temperatur.CSVIdentifierColum, ident);
    strcpy(Temperatur.CSVIdentifier, inputText);
    strcpy(Temperatur.CSVValueColum, nodeName);

    // MinTemperatur
    struct myNode MinTemperatur;
    CreateNode(&MinTemperatur);
    strcpy(nodeName, "MinTemperatur");
    strcpy(currentName, inputText);
    strcat(currentName, nodeName);
    MinTemperatur.Type = Double;
    strcpy(MinTemperatur.Name, currentName);
    strcpy(MinTemperatur.DisplayName, nodeName);
    strcpy(MinTemperatur.Parrent, Folder.Name);
    MinTemperatur.Write = true;
    strcpy(MinTemperatur.CSVName, configPath);
    strcpy(MinTemperatur.CSVIdentifierColum, ident);
    strcpy(MinTemperatur.CSVIdentifier, inputText);
    strcpy(MinTemperatur.CSVValueColum, nodeName);

    // MaxTemperatur
    struct myNode MaxTemperatur;
    CreateNode(&MaxTemperatur);
    strcpy(nodeName, "MaxTemperatur");
    strcpy(currentName, inputText);
    strcat(currentName, nodeName);
    MaxTemperatur.Type = Double;
    strcpy(MaxTemperatur.Name, currentName);
    strcpy(MaxTemperatur.DisplayName, nodeName);
    strcpy(MaxTemperatur.Parrent, Folder.Name);
    MaxTemperatur.Write = true;
    strcpy(MaxTemperatur.CSVName, configPath);
    strcpy(MaxTemperatur.CSVIdentifierColum, ident);
    strcpy(MaxTemperatur.CSVIdentifier, inputText);
    strcpy(MaxTemperatur.CSVValueColum, nodeName);

    // OverTemperatur
    struct myNode OverTemperatur;
    CreateNode(&OverTemperatur);
    strcpy(nodeName, "OverTemperatur");
    strcpy(currentName, inputText);
    strcat(currentName, nodeName);
    OverTemperatur.Type = Double;
    strcpy(OverTemperatur.Name, currentName);
    strcpy(OverTemperatur.DisplayName, nodeName);
    strcpy(OverTemperatur.Parrent, Folder.Name);
    OverTemperatur.Write = false;
    strcpy(OverTemperatur.CSVName, configPath);
    strcpy(OverTemperatur.CSVIdentifierColum, ident);
    strcpy(OverTemperatur.CSVIdentifier, inputText);
    strcpy(OverTemperatur.CSVValueColum, nodeName);

    // UnderTemperatur
    struct myNode UnderTemperatur;
    CreateNode(&UnderTemperatur);
    strcpy(nodeName, "UnderTemperatur");
    strcpy(currentName, inputText);
    strcat(currentName, nodeName);
    UnderTemperatur.Type = Double;
    strcpy(UnderTemperatur.Name, currentName);
    strcpy(UnderTemperatur.DisplayName, nodeName);
    strcpy(UnderTemperatur.Parrent, Folder.Name);
    UnderTemperatur.Write = false;
    strcpy(UnderTemperatur.CSVName, configPath);
    strcpy(UnderTemperatur.CSVIdentifierColum, ident);
    strcpy(UnderTemperatur.CSVIdentifier, inputText);
    strcpy(UnderTemperatur.CSVValueColum, nodeName);

    // NotConnected
    struct myNode NotConnected;
    CreateNode(&NotConnected);
    strcpy(nodeName, "NotConnected");
    strcpy(currentName, inputText);
    strcat(currentName, nodeName);
    NotConnected.Type = Boolean;
    strcpy(NotConnected.Name, currentName);
    strcpy(NotConnected.DisplayName, nodeName);
    strcpy(NotConnected.Parrent, Folder.Name);
    NotConnected.Write = false;
    strcpy(NotConnected.CSVName, currentPath);
    strcpy(NotConnected.CSVIdentifierColum, ident);
    strcpy(NotConnected.CSVIdentifier, inputText);
    strcpy(NotConnected.CSVValueColum, nodeName);

    bool fail = (AppandNodeToNodes(server, &Folder) < 0 ||
                 AppandNodeToNodes(server, &Name) < 0 ||
                 AppandNodeToNodes(server, &Description) < 0 ||
                 AppandNodeToNodes(server, &Location) < 0 ||
                 AppandNodeToNodes(server, &Antenna) < 0 ||
                 AppandNodeToNodes(server, &Temperatur) < 0 ||
                 AppandNodeToNodes(server, &MinTemperatur) < 0 ||
                 AppandNodeToNodes(server, &MaxTemperatur) < 0 ||
                 AppandNodeToNodes(server, &OverTemperatur) < 0 ||
                 AppandNodeToNodes(server, &UnderTemperatur) < 0 ||
                 AppandNodeToNodes(server, &NotConnected) < 0);
    if (fail)
    {
        // Remove all nodes
        RemoveNodeFromNodes(server, &Folder);
        RemoveNodeFromNodes(server, &Name);
        RemoveNodeFromNodes(server, &Description);
        RemoveNodeFromNodes(server, &Location);
        RemoveNodeFromNodes(server, &Antenna);
        RemoveNodeFromNodes(server, &Temperatur);
        RemoveNodeFromNodes(server, &MinTemperatur);
        RemoveNodeFromNodes(server, &MaxTemperatur);
        RemoveNodeFromNodes(server, &OverTemperatur);
        RemoveNodeFromNodes(server, &UnderTemperatur);
        RemoveNodeFromNodes(server, &NotConnected);
        ;
        return;
    }

    // Store Nodes
    StoreNodes("/files/server/runtimeNode.csv");
}
static UA_StatusCode RemoveTemperaturSensor(
    UA_Server *server,
    const UA_NodeId *sessionId, void *sessionHandle,
    const UA_NodeId *methodId, void *methodContext,
    const UA_NodeId *objectId, void *objectContext,
    size_t inputSize, const UA_Variant *input,
    size_t outputSize, UA_Variant *output)
{
    char inputText[50];
    UA_VariantToString(input, inputText);
    UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND, "RemoveTemperaturSensor:%s", inputText);

    bool found = false;
    for (int a = 0; a < NODESCOUNT; a++)
    {
        if (strcmp(Nodes[a].Name, inputText) == 0)
        {
            found = true;
            RemoveNodeFromNodes(server, &Nodes[a]);
            break;
        }
    }

    StoreNodes("/files/server/runtimeNode.csv");
}

static void AddMethodNode(
    UA_Server *server, 
    struct myNode *node, 
    UA_NodeId parrent, 
    UA_MethodAttributes *Attr)
{

    if (strcmp(node->MethodCallback, "AddTemperaturSensor") == 0)
    {

        UA_Argument inputArgument;
        UA_Argument_init(&inputArgument);

        inputArgument.name = UA_STRING("TagId");
        inputArgument.description = UA_LOCALIZEDTEXT("en-US", "Tag Id (24 chars");
        inputArgument.dataType = UA_TYPES[UA_TYPES_STRING].typeId;
        inputArgument.valueRank = UA_VALUERANK_SCALAR;

        UA_Argument outputArgument;
        UA_Argument_init(&outputArgument);
        outputArgument.name = UA_STRING("TagId");
        outputArgument.description = UA_LOCALIZEDTEXT("en-US", "Retruns on Sucsess the Tag Id, on Fail empty");
        outputArgument.dataType = UA_TYPES[UA_TYPES_STRING].typeId;
        outputArgument.valueRank = UA_VALUERANK_SCALAR;

        UA_Server_addMethodNode(
            server,
            UA_NODEID_NULL,
            parrent,
            UA_NODEID_NUMERIC(0, UA_NS0ID_HASCOMPONENT),
            UA_QUALIFIEDNAME(1, node->Name),
            *Attr,
            &AddTemperaturSensor,
            1,
            &inputArgument,
            1,
            &outputArgument,
            NULL,
            &(node->NodeId));
    }
    else if (strcmp(node->MethodCallback, "RemoveTemperaturSensor") == 0)
    {

        UA_Argument inputArgument;
        UA_Argument_init(&inputArgument);

        inputArgument.name = UA_STRING("TagId");
        inputArgument.description = UA_LOCALIZEDTEXT("en-US", "Tag Id (24 chars");
        inputArgument.dataType = UA_TYPES[UA_TYPES_STRING].typeId;
        inputArgument.valueRank = UA_VALUERANK_SCALAR;

        UA_Argument outputArgument;
        UA_Argument_init(&outputArgument);
        outputArgument.name = UA_STRING("TagId");
        outputArgument.description = UA_LOCALIZEDTEXT("en-US", "Retruns on Sucsess the Tag Id, on Fail empty");
        outputArgument.dataType = UA_TYPES[UA_TYPES_STRING].typeId;
        outputArgument.valueRank = UA_VALUERANK_SCALAR;

        UA_Server_addMethodNode(
            server,
            UA_NODEID_NULL,
            parrent,
            UA_NODEID_NUMERIC(0, UA_NS0ID_HASCOMPONENT),
            UA_QUALIFIEDNAME(1, node->Name),
            *Attr,
            &RemoveTemperaturSensor,
            1,
            &inputArgument,
            1,
            &outputArgument,
            NULL,
            &(node->NodeId));
    }
}

int main(void)
{
    printf("\n\nStart Server\n");

    // char files[10][256];
    // getFiles(10,256,files,"/config/server");
    // for (int a=0; a < 10; a++) {
    //     printf("File: %s\n",files[a]);
    // }

    int serverConfigRows = 2;
    int serverConfigColums = 2;
    int serverConfigLength = 256;
    char serverCSV[serverConfigRows][serverConfigColums][serverConfigLength];
    getCSV("/files/server/config.csv",
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

    char *ipAdresse = serverCSV[1][ipCol];
    char *portText = serverCSV[1][portCol];

    // #region Setup
    signal(SIGINT, stopHandler);
    signal(SIGTERM, stopHandler);

    // Create new Server instance
    UA_Server *server = UA_Server_new();

    // set port number
    UA_Int16 port = atoi(portText);
    UA_ServerConfig_setMinimal(
        UA_Server_getConfig(server),
        port,
        0);

    // set hostname
    UA_String hostname;
    UA_String_init(&hostname);
    hostname.length = strlen(ipAdresse);
    hostname.data = (UA_Byte *)ipAdresse;
    UA_ServerConfig_setCustomHostname(
        UA_Server_getConfig(server),
        hostname);
    // #endregion

    // NodeSet

    NodesetAddMethodNode = AddMethodNode;

    if (fileExist("/files/server/runtimeNode.csv"))
    {
        if (!LoadNodes("/files/server/runtimeNode.csv"))
        {
            LoadNodes("/files/server/node.csv");
        }
    }
    else
    {
        LoadNodes("/files/server/node.csv");
    }

    UpdateDynamicNodes(server, true);

    StoreNodes("/files/server/runtimeNode.csv");
    /*
    // Variables

    struct nodeTypes Types;



    struct nodeRFIDReader Variables;

    AddNodeVariableType(server,&Types);
    AddNodeTemperaturSensorType(server,&Types);
    AddNodePinType(server,&Types);
    AddNodeRFIDReaderType(server,&Types);
    AddNodeTemperaturMonitoringType(server,&Types);


    UA_NodeId root = UA_NODEID_NUMERIC(0, UA_NS0ID_OBJECTSFOLDER);
   /* AddNodeTemperaturMonitoring(
        server,
        root,
        &Types,
        &Variables,
        "Monitoring 1");
    */
    /*
     AddNodeRFIDReader(
         server,
         root,
         &Types,
         &Variables,
         "Monitoring 1");


      //  UpdateNodeTemperaturMonitoring(server,&Variables,true);

     /*AddNodeTemperaturMonitoring(
         server,
         &MonitoringType,
         UA_NODEID_NUMERIC(0, UA_NS0ID_BASEOBJECTTYPE),
         "MonitoringType"
     );

     //addGenericValueType(server);
     //addGenericValueVariable(server);
     /*manuallyDefineTemptag(
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
     */

    // addVariableType2DPoint(server);
    // addVariable1(server);
    // addVariableFail(server);
    // writeVariable1(server);

    UA_Server_addRepeatedCallback(
        server, updateEverySecond,
        &Nodes,
        1000,
        NULL);

    UA_StatusCode retval = UA_Server_run(server, &running);
    UA_Server_delete(server);
    return retval == UA_STATUSCODE_GOOD ? EXIT_SUCCESS : EXIT_FAILURE;
    //*/

    //  char buffer[1000];
    //  writeConfigFile("4840","/config/server/port.conf");
    //  readConfigFile(buffer,sizeof(buffer),"/config/server/port.conf");

    printf("Stopped Server\n\n\n");
}