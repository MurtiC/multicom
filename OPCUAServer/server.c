
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include "open62541/open62541.h"

#include "readFile.c"

#include "data/updateNodes.c"

#include "data/nodeset.c"

#define SoftwareTimeoutduration 20 // in seconds
#define MaxLogSize 1073741824

static volatile UA_Boolean running = true;
static int LoopCount = 0;

#define LogToFile
static FILE *logFile;

static void stopHandler(int sig)
{
    fclose(logFile);
    UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND, "received ctrl-c");
    running = false;
}

static void LogIntoFile()
{

    if (logFile != NULL)
    {
        fclose(logFile);
    }

    // Log File
    time_t currentTimeT;
    time(&currentTimeT);
    struct tm *currenttimeTm;
    currenttimeTm = gmtime(&currentTimeT);

    char logPath[PATH_MAX];
    char logPathFull[PATH_MAX];
    sprintf(
        logPath,
        "/files/server/logs/opc_%d_%d_%d_%d.txt",
        currenttimeTm->tm_year + 1900,
        currenttimeTm->tm_mon,
        currenttimeTm->tm_mday,
        currenttimeTm->tm_hour);
    UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND, "Log to:%s", logPath);

    getFullPath(logPath, logPathFull);
    #ifdef LogToFile
    logFile = freopen(logPathFull,"a",stdout);
    #endif
    UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND, "Log to:%s", logPath);
}

static void ClearOldLogs()
{

    char oldlogFiles[200][PATH_MAX];
    getFiles(200, PATH_MAX, oldlogFiles, "/files/server/logs");
    long long size = 0;
    char logFileName[PATH_MAX];
    struct stat file;
    for (int a = 0; a < 200; a++)
    {
        if (strlen(oldlogFiles[a]) > 0)
        {
            strcpy(logFileName, "/files/server/logs/");
            strcat(logFileName, oldlogFiles[a]);

            if (fileStats(logFileName, &file))
            {
                size += file.st_size;
            }

            if (size > MaxLogSize)
            {
                // deleteFile(logFileName);
            }

            UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND, "Logs:%s , size:%d / %d", oldlogFiles[a], size, MaxLogSize);
        }
    }
}

void updateEverySecond(UA_Server *server, void *data)
{
    LoopCount++;
    // struct nodeRFIDReader* variables = (struct nodeRFIDReader*)data;
    UpdateDynamicNodes(server);
}

void updateEveryHour(UA_Server *server, void *data)
{
    UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND, "Alive:%d", LoopCount);
    LoopCount = 0;
    LogIntoFile();
    ClearOldLogs();
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
    UA_VariantToText(&inputVariant, inputText);
    UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND, "AddTemperaturSensor:%s", inputText);

    if (strlen(inputText) != 24)
    {
        // return;
    }

    for (int a = 0; a < NODESCOUNT; a++)
    {
        if (strcmp(Nodes[a].Name, inputText) == 0)
        {
            return;
        }
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
    Folder.Type = TypeObject;
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
    Name.Type = TypeString;
    strcpy(Name.Description, "The name of the sensor");
    strcpy(Name.Name, currentName);
    strcpy(Name.DisplayName, nodeName);
    strcpy(Name.Parrent, Folder.Name);
    Name.Write = true;
    Name.DataSource = SourceCSV;
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
    Description.Type = TypeString;
    strcpy(Description.Description, "The description of the sensor");
    strcpy(Description.Name, currentName);
    strcpy(Description.DisplayName, nodeName);
    strcpy(Description.Parrent, Folder.Name);
    Description.Write = true;
    Description.DataSource = SourceCSV;
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
    Location.Type = TypeString;
    strcpy(Location.Description, "The location of the sensor");
    strcpy(Location.Name, currentName);
    strcpy(Location.DisplayName, nodeName);
    strcpy(Location.Parrent, Folder.Name);
    Location.Write = true;
    Location.DataSource = SourceCSV;
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
    Antenna.Type = TypeInteger;
    strcpy(Antenna.Description, "The number of antenna the sensor belongs to. []");
    strcpy(Antenna.Name, currentName);
    strcpy(Antenna.DisplayName, nodeName);
    strcpy(Antenna.Parrent, Folder.Name);
    Antenna.Write = true;
    Antenna.DataSource = SourceCSV;
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
    Temperatur.Type = TypeDouble;
    strcpy(Temperatur.Description, "The measured temperatur. [°C]");
    strcpy(Temperatur.Name, currentName);
    strcpy(Temperatur.DisplayName, nodeName);
    strcpy(Temperatur.Parrent, Folder.Name);
    Temperatur.Write = false;
    Temperatur.DataSource = SourceCSV;
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
    MinTemperatur.Type = TypeDouble;
    strcpy(MinTemperatur.Description, "The lower temperatur limit. [°C]");
    strcpy(MinTemperatur.Name, currentName);
    strcpy(MinTemperatur.DisplayName, nodeName);
    strcpy(MinTemperatur.Parrent, Folder.Name);
    MinTemperatur.Write = true;
    MinTemperatur.DataSource = SourceCSV;
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
    MaxTemperatur.Type = TypeDouble;
    strcpy(MaxTemperatur.Description, "The upper temperatur limit. [°C]");
    strcpy(MaxTemperatur.Name, currentName);
    strcpy(MaxTemperatur.DisplayName, nodeName);
    strcpy(MaxTemperatur.Parrent, Folder.Name);
    MaxTemperatur.Write = true;
    MaxTemperatur.DataSource = SourceCSV;
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
    OverTemperatur.Type = TypeBoolean;
    strcpy(OverTemperatur.Description, "Is set when temperatur is equal or above maxtemperatur [I/O]");
    strcpy(OverTemperatur.Name, currentName);
    strcpy(OverTemperatur.DisplayName, nodeName);
    strcpy(OverTemperatur.Parrent, Folder.Name);
    OverTemperatur.Write = false;
    OverTemperatur.DataSource = SourceCallback;
    strcpy(OverTemperatur.SourceCallback, "OverTemperatur");

    // UnderTemperatur
    struct myNode UnderTemperatur;
    CreateNode(&UnderTemperatur);
    strcpy(nodeName, "UnderTemperatur");
    strcpy(currentName, inputText);
    strcat(currentName, nodeName);
    UnderTemperatur.Type = TypeBoolean;
    strcpy(UnderTemperatur.Description, "Is set when temperatur is equal or below mintemperatur [I/O]");
    strcpy(UnderTemperatur.Name, currentName);
    strcpy(UnderTemperatur.DisplayName, nodeName);
    strcpy(UnderTemperatur.Parrent, Folder.Name);
    UnderTemperatur.Write = false;
    UnderTemperatur.DataSource = SourceCallback;
    strcpy(UnderTemperatur.SourceCallback, "UnderTemperatur");

    // NotConnected
    struct myNode NotConnected;
    CreateNode(&NotConnected);
    strcpy(nodeName, "NotConnected");
    strcpy(currentName, inputText);
    strcat(currentName, nodeName);
    NotConnected.Type = TypeBoolean;
    strcpy(NotConnected.Description, "Is set when sensor doesend respond for the timeout duration [I/O]");
    strcpy(NotConnected.Name, currentName);
    strcpy(NotConnected.DisplayName, nodeName);
    strcpy(NotConnected.Parrent, Folder.Name);
    NotConnected.Write = false;
    NotConnected.DataSource = SourceCSV;
    strcpy(NotConnected.CSVName, currentPath);
    strcpy(NotConnected.CSVIdentifierColum, ident);
    strcpy(NotConnected.CSVIdentifier, inputText);
    strcpy(NotConnected.CSVValueColum, nodeName);

    // SoftwareNotConnected
    struct myNode SoftwareNotConnected;
    CreateNode(&SoftwareNotConnected);
    strcpy(nodeName, "SoftwareNotConnected");
    strcpy(currentName, inputText);
    strcat(currentName, nodeName);
    SoftwareNotConnected.Type = TypeBoolean;
    strcpy(SoftwareNotConnected.Description, "Is set when reader client doesend respond for the timeout duration");
    strcpy(SoftwareNotConnected.Name, currentName);
    strcpy(SoftwareNotConnected.DisplayName, nodeName);
    strcpy(SoftwareNotConnected.Parrent, Folder.Name);
    SoftwareNotConnected.Write = false;
    SoftwareNotConnected.DataSource = SourceCallback;
    strcpy(SoftwareNotConnected.CSVName, currentPath);
    strcpy(SoftwareNotConnected.SourceCallback, "SoftwareNotConnected");

    // Timeout
    struct myNode Timeout;
    CreateNode(&Timeout);
    strcpy(nodeName, "Timeout");
    strcpy(currentName, inputText);
    strcat(currentName, nodeName);
    Timeout.Type = TypeDouble;
    strcpy(Timeout.Description, "Duration unil sensor is marked as not connected. [seconds]");
    strcpy(Timeout.Name, currentName);
    strcpy(Timeout.DisplayName, nodeName);
    strcpy(Timeout.Parrent, Folder.Name);
    Timeout.Write = true;
    Timeout.DataSource = SourceCSV;
    strcpy(Timeout.CSVName, configPath);
    strcpy(Timeout.CSVIdentifierColum, ident);
    strcpy(Timeout.CSVIdentifier, inputText);
    strcpy(Timeout.CSVValueColum, nodeName);

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
                 AppandNodeToNodes(server, &Timeout) < 0 ||
                 AppandNodeToNodes(server, &SoftwareNotConnected) < 0 ||
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
        RemoveNodeFromNodes(server, &Timeout);
        RemoveNodeFromNodes(server, &SoftwareNotConnected);
        RemoveNodeFromNodes(server, &NotConnected);
    }
    else
    {
        // Store Nodes
        StoreNodes("/files/server/runtimeNode.csv");
        char text[100];
        strcpy(text, inputText);
        UA_String uaText = UA_String_fromChars(text);
        UA_Variant_setScalarCopy(output, &uaText, &UA_TYPES[UA_TYPES_STRING]);
    }
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
    UA_VariantToText(input, inputText);
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

    if (found)
    {
        char text[100];
        strcpy(text, inputText);
        UA_String uaText = UA_String_fromChars(text);
        UA_Variant_setScalarCopy(output, &uaText, &UA_TYPES[UA_TYPES_STRING]);
    }
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

static void UpdateNodesetNode(
    UA_Server *server,
    struct myNode *node)
{
    // checks if node is a Upper or Lower Temperatur
    // UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND, "UpdateNodesetNode");

    bool overTemp = strcmp(node->SourceCallback, "OverTemperatur") == 0;
    bool underTemp = strcmp(node->SourceCallback, "UnderTemperatur") == 0;
    bool SoftwareNotConnected = strcmp(node->SourceCallback, "SoftwareNotConnected") == 0;

    if (overTemp || underTemp)
    {
        char temperaturName[NODECHARLENGTH];
        strcpy(temperaturName, node->Parrent);
        strcat(temperaturName, "Temperatur");
        char minTemperaturName[NODECHARLENGTH];
        strcpy(minTemperaturName, node->Parrent);
        strcat(minTemperaturName, "MinTemperatur");
        char maxTemperaturName[NODECHARLENGTH];
        strcpy(maxTemperaturName, node->Parrent);
        strcat(maxTemperaturName, "MaxTemperatur");
        struct myNode *temperaturStruct;
        struct myNode *minTemperaturStruct;
        struct myNode *maxTemperaturStruct;
        for (int a = 0; a < NODESCOUNT; a++)
        {
            if (strcmp(Nodes[a].Name, temperaturName) == 0)
            {
                temperaturStruct = &Nodes[a];
            }
            if (strcmp(Nodes[a].Name, minTemperaturName) == 0)
            {
                minTemperaturStruct = &Nodes[a];
            }
            if (strcmp(Nodes[a].Name, maxTemperaturName) == 0)
            {
                maxTemperaturStruct = &Nodes[a];
            }
        }

        UA_Variant temperaturVariant;
        UA_Variant minTemperaturVariant;
        UA_Variant maxTemperaturVariant;
        UA_Variant valueVariant;
        UA_Variant oldValueVariant;

        UA_Server_readValue(server, temperaturStruct->NodeId, &temperaturVariant);
        UA_Server_readValue(server, minTemperaturStruct->NodeId, &minTemperaturVariant);
        UA_Server_readValue(server, maxTemperaturStruct->NodeId, &maxTemperaturVariant);

        UA_Server_readValue(server, node->NodeId, &oldValueVariant);

        double temperaturValue = UA_VariantToDouble(&temperaturVariant);
        double minTemperaturValue = UA_VariantToDouble(&minTemperaturVariant);
        double maxTemperaturValue = UA_VariantToDouble(&maxTemperaturVariant);
        bool oldValue = UA_VariantToBool(&oldValueVariant);
        bool value = false;

        if (overTemp)
        {
            value = temperaturValue >= maxTemperaturValue;
        }
        else if (underTemp)
        {
            value = temperaturValue <= minTemperaturValue;
        }
        BoolToUA_Variant(&valueVariant, &value);

        if (value != oldValue)
        {
            UA_Server_writeValue(server, node->NodeId, valueVariant);
        }
    }
    else if (SoftwareNotConnected)
    {

        time_t rawtime;
        time(&rawtime);

        struct stat stats;

        bool timeout = true;
        double diffseconds = 0;
        if (fileStats(node->CSVName, &stats))
        {
            // struct tm time;
            // time = *(gmtime(&stats.st_ctime));

            double diffsecondsCTime = difftime(rawtime, stats.st_ctime);
            double diffsecondsMTime = difftime(rawtime, stats.st_mtime);

            diffseconds = diffsecondsCTime;
            if (diffseconds < diffsecondsMTime)
            {
                diffseconds = diffsecondsMTime;
            }

            timeout = diffseconds >= SoftwareTimeoutduration;
        }
        UA_Variant valueVariant;
        UA_Server_readValue(server, node->NodeId, &valueVariant);
        bool oldValue = UA_VariantToBool(&valueVariant);

        if (oldValue != timeout)
        {
            BoolToUA_Variant(&valueVariant, &timeout);
            UA_Server_writeValue(server, node->NodeId, valueVariant);
        }
        if (timeout)
        {
            UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND, "SoftwareNotConnected:%s: %0.01f", node->CSVName, diffseconds);
        }
    }
}

int main(void)
{
    ClearOldLogs();
    LogIntoFile();
    // Start Server
    UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND, "Start Server");

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

    // NodeSet

    NodesetAddMethodNode = AddMethodNode;
    NodesetUpdateNode = UpdateNodesetNode;
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
    UpdateDynamicNodes(server);
    StoreNodes("/files/server/runtimeNode.csv");

    UA_Server_addRepeatedCallback(
        server, updateEverySecond,
        &Nodes,
        1000,
        NULL);

    // 1h : 3600000

    UA_Server_addRepeatedCallback(
        server, updateEveryHour,
        NULL,
        3600000,
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