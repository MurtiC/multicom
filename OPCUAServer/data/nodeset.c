#ifndef nodeset_C
#define nodeset_C

#include "updateNodes.c"

/*
Create of Nodes
Node
    String: Type
    String: Name
    String: DisplayName
    String: Description
    String: Parrent
    Bool:   Write
    Double: Min
    Double: Max
    String: CSV
    Int:    Row
    String: Colum
    String: MethodCallback
    String: EventCallback
*/


enum myNodeType
{
    TypeNone = 0,
    TypeObject = 1,
    TypeInteger = 2,
    TypeBoolean = 3,
    TypeString = 4,
    TypeDouble = 5,
    TypeDate = 6,
    TypeMethod = 7
};
enum myNodeDataSourceType
{
    SourceNone = 0,
    SourceCSV = 1,
    SourceCallback = 2,

};

#ifndef NODECHARLENGTH
#define NODECHARLENGTH 256
#endif
struct myNode
{
    // File
    enum myNodeType Type;            // Datatype of Node [Object, Integer, Boolean, String, Double]
    char Name[NODECHARLENGTH];        // Name of Node
    char Description[NODECHARLENGTH*4]; // Description of Node
    char DisplayName[NODECHARLENGTH]; // DisplayName of Node
    char Parrent[NODECHARLENGTH];     // Parrent Name of Node (BaseFolder is the Root)
    bool Write;                      // Set the ReadWrite Parameter
    double Min;                      // Min Value
    double Max;                      // Max Value
    enum myNodeDataSourceType DataSource;
    char CSVName[NODECHARLENGTH];     // Value Source CSV
    char CSVIdentifierColum[NODECHARLENGTH];
    char CSVIdentifier[NODECHARLENGTH];
    char CSVValueColum[NODECHARLENGTH];  // Value Source Colum Name
    char MethodCallback[NODECHARLENGTH]; // MethodCallback Name, gets called when this method gets called
    // Runtime:
    bool Created;
    UA_NodeId NodeId;
};

#ifndef NODESCOUNT
#define NODESCOUNT 300
#endif
#ifndef CSVROWS
#define CSVROWS NODESCOUNT+1
#endif
#ifndef CSVCOLUMS
#define CSVCOLUMS 20
#endif
#ifndef CSVLENGTH
#define CSVLENGTH NODECHARLENGTH*4
#endif

// Create a global List of Nodes 
struct myNode Nodes[NODESCOUNT];
// Create a Global Callback for adding a Method to Node
//                          (Server, node, parrent, attr)
void (*NodesetAddMethodNode)(UA_Server*, struct myNode *, UA_NodeId, UA_MethodAttributes*);
void (*NodesetUpdateNode)(UA_Server*, struct myNode *, );

static void CreateNode(struct myNode *node) {
    node->Type = TypeNone;
    strcpy(node->Name,"");
    strcpy(node->Description,"");
    strcpy(node->DisplayName,"");
    strcpy(node->Parrent,"");
    node->Write = false;
    node->Min = 0;
    node->Max = 0;
    node->DataSource = SourceNone;
    strcpy(node->CSVName,"");
    strcpy(node->CSVIdentifierColum,"");
    strcpy(node->CSVIdentifier,"");
    strcpy(node->CSVValueColum,"");
    strcpy(node->MethodCallback,"");
    node->Created = false;
    node->NodeId = UA_NODEID_NULL;
}

static bool CompareNode(struct myNode *node1,struct myNode *node2) {
    if (node1->Type != node2->Type) {
        return false;
    }
    if (strcmp(node1->Name,node2->Name) != 0) {
       return false;
    }
    if (strcmp(node1->Description,node2->Description) != 0) {
       return false;
    }
    if (strcmp(node1->DisplayName,node2->DisplayName) != 0) {
       return false;
    }
    if (strcmp(node1->Parrent,node2->Parrent) != 0) {
       return false;
    }
    if (node1->Write != node2->Write) {
        return false;
    }
    if (node1->Min != node2->Min) {
        return false;
    }
    if (node1->Max != node2->Max) {
        return false;
    }
    if (strcmp(node1->CSVName,node2->CSVName) != 0) {
       return false;
    }
    if (strcmp(node1->CSVIdentifierColum,node2->CSVIdentifierColum) != 0) {
       return false;
    }
    if (strcmp(node1->CSVIdentifier,node2->CSVIdentifier) != 0) {
       return false;
    }
    if (strcmp(node1->CSVValueColum,node2->CSVValueColum) != 0) {
       return false;
    }
    if (strcmp(node1->MethodCallback,node2->MethodCallback) != 0) {
       return false;
    }
    if (strcmp(node1->Name,node2->Name) != 0) {
       return false;
    }
    if (strcmp(node1->Name,node2->Name) != 0) {
       return false;
    }
    return true;
}

//#region [rgba(255,0,0,0.1)]
/**
 * @brief Loads a CSV File
 * @param path path to CSV File
 */
static bool LoadNodes(char *path)
{
    int nodesCSV_Rows = CSVROWS;
    int nodesCSV_Colums = CSVCOLUMS;
    int nodesCSV_Length = CSVLENGTH;
    char nodesCSV[nodesCSV_Rows][nodesCSV_Colums][nodesCSV_Length];
    createCSV(nodesCSV_Rows, nodesCSV_Colums, nodesCSV_Length, nodesCSV);
    if (getCSV(
            path,
            nodesCSV_Rows,
            nodesCSV_Colums,
            nodesCSV_Length,
            nodesCSV) < 0)
    {
        UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND, "Cannot load CSV:%s", path);
        return false;
    }

    // printCSV(nodesCSV_Rows, nodesCSV_Colums, nodesCSV_Length, nodesCSV );

    int colum_Type = getCSVColum(nodesCSV_Rows, nodesCSV_Colums, nodesCSV_Length, nodesCSV, "Type");
    int colum_Name = getCSVColum(nodesCSV_Rows, nodesCSV_Colums, nodesCSV_Length, nodesCSV, "Name");
    int colum_Description = getCSVColum(nodesCSV_Rows, nodesCSV_Colums, nodesCSV_Length, nodesCSV, "Description");
    int colum_DisplayName = getCSVColum(nodesCSV_Rows, nodesCSV_Colums, nodesCSV_Length, nodesCSV, "DisplayName");
    int colum_Parrent = getCSVColum(nodesCSV_Rows, nodesCSV_Colums, nodesCSV_Length, nodesCSV, "Parrent");
    int colum_Write = getCSVColum(nodesCSV_Rows, nodesCSV_Colums, nodesCSV_Length, nodesCSV, "Write");
    int colum_Min = getCSVColum(nodesCSV_Rows, nodesCSV_Colums, nodesCSV_Length, nodesCSV, "Min");
    int colum_Max = getCSVColum(nodesCSV_Rows, nodesCSV_Colums, nodesCSV_Length, nodesCSV, "Max");
    int colum_DataSource = getCSVColum(nodesCSV_Rows, nodesCSV_Colums, nodesCSV_Length, nodesCSV, "DataSource");
    int colum_CSVName = getCSVColum(nodesCSV_Rows, nodesCSV_Colums, nodesCSV_Length, nodesCSV, "CSVName");
    int colum_CSVIdentifierColum = getCSVColum(nodesCSV_Rows, nodesCSV_Colums, nodesCSV_Length, nodesCSV, "CSVIdentifierColum");
    int colum_CSVIdentifier = getCSVColum(nodesCSV_Rows, nodesCSV_Colums, nodesCSV_Length, nodesCSV, "CSVIdentifier");
    int colum_CSVValueColum = getCSVColum(nodesCSV_Rows, nodesCSV_Colums, nodesCSV_Length, nodesCSV, "CSVValueColum");
    int colum_MethodCallback = getCSVColum(nodesCSV_Rows, nodesCSV_Colums, nodesCSV_Length, nodesCSV, "MethodCallback");

    if (colum_Type < 0 || colum_Type >= nodesCSV_Colums)
    {
        UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND, "CSV does not contain Type");
        return false;
    }
    if (colum_Name < 0 || colum_Name >= nodesCSV_Colums)
    {
        UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND, "CSV does not contain Name");
        return false;
    }
    if (colum_Description < 0 || colum_Description >= nodesCSV_Colums)
    {
        UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND, "CSV does not contain Description");
        return false;
    }
    if (colum_DisplayName < 0 || colum_DisplayName >= nodesCSV_Colums)
    {
        UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND, "CSV does not contain DisplayName");
        return false;
    }
    if (colum_Parrent < 0 || colum_Parrent >= nodesCSV_Colums)
    {
        UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND, "CSV does not contain Parrent");
        return false;
    }

    for (int a = 1; a < nodesCSV_Rows; a++)
    {
        
        int nodeNum = a - 1;
        // Init Values
        CreateNode(&Nodes[nodeNum]);
        // UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND,
        //            "Node:%d, %s",
        //            colum_Type, nodesCSV[a][colum_Type]);
        if (colum_Type > -1 && colum_Name > -1 && colum_Parrent > -1)
        {
            if (strlen(nodesCSV[a][colum_Type]) > 0 &&
                strlen(nodesCSV[a][colum_Type]) > 0 &&
                strlen(nodesCSV[a][colum_Parrent]) > 0)
            {

                Nodes[nodeNum].Type = (enum myNodeType)atoi(nodesCSV[a][colum_Type]);
                strcpy(Nodes[nodeNum].Name, nodesCSV[a][colum_Name]);

                if (colum_Description > -1)
                {
                    if (strlen(nodesCSV[a][colum_Description]) > 0)
                    {
                        strcpy(Nodes[nodeNum].Description, nodesCSV[a][colum_Description]);
                    }
                }
                if (colum_DisplayName > -1)
                {
                    if (strlen(nodesCSV[a][colum_DisplayName]) > 0)
                    {
                        strcpy(Nodes[nodeNum].DisplayName, nodesCSV[a][colum_DisplayName]);
                    }
                }

                strcpy(Nodes[nodeNum].Parrent, nodesCSV[a][colum_Parrent]);

                if (colum_Write > -1)
                {
                    if (strlen(nodesCSV[a][colum_Write]) > 0)
                    {
                        Nodes[nodeNum].Write = strcmp(nodesCSV[a][colum_Write], "1") == 0;
                    }
                }
                if (colum_Min > -1)
                {
                    if (strlen(nodesCSV[a][colum_Min]) > 0)
                    {
                        Nodes[nodeNum].Min = atof(nodesCSV[a][colum_Min]);
                    }
                }
                if (colum_Max > -1)
                {
                    if (strlen(nodesCSV[a][colum_Max]) > 0)
                    {
                        Nodes[nodeNum].Max = atof(nodesCSV[a][colum_Max]);
                    }
                }
                if (colum_DataSource > -1) 
                {
                    if (strlen(nodesCSV[a][colum_DataSource]) > 0) 
                    {
                        Nodes[nodeNum].DataSource = (enum myNodeDataSourceType) atoi(nodesCSV[a][colum_DataSource]);
                    }
                }

                if (colum_CSVName > -1)
                {
                    if (strlen(nodesCSV[a][colum_CSVName]) > 0)
                    {
                        // CSV
                        strcpy(Nodes[nodeNum].CSVName, nodesCSV[a][colum_CSVName]);
                    }
                }
                if (colum_CSVIdentifierColum > -1)
                {
                    if (strlen(nodesCSV[a][colum_CSVIdentifierColum]) > 0)
                    {
                        strcpy(Nodes[nodeNum].CSVIdentifierColum, nodesCSV[a][colum_CSVIdentifierColum]);
                    }
                }
                if (colum_CSVIdentifier > -1)
                {
                    if (strlen(nodesCSV[a][colum_CSVIdentifier]) > 0)
                    {
                        strcpy(Nodes[nodeNum].CSVIdentifier, nodesCSV[a][colum_CSVIdentifier]);
                    }
                }
                if (colum_CSVValueColum > -1)
                {
                    if (strlen(nodesCSV[a][colum_CSVValueColum]) > 0)
                    {
                        strcpy(Nodes[nodeNum].CSVValueColum, nodesCSV[a][colum_CSVValueColum]);
                    }
                }
                if (colum_MethodCallback > -1)
                {
                    if (strlen(nodesCSV[a][colum_MethodCallback]) > 0)
                    {
                        strcpy(Nodes[nodeNum].MethodCallback, nodesCSV[a][colum_MethodCallback]);
                    }
                }
            }
        }
        /*UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND,
                    "Node:%d, %s, %s\n",
                    Nodes[nodeNum].Type,
                    Nodes[nodeNum].Name,
                    Nodes[nodeNum].Parrent);
        /*UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND,
            "Node:%s, %s\n",
            nodesCSV[a][colum_Type],
            nodesCSV[a][colum_Name]);
        //*/
    }
    printf("\n");
    return true;
}
//#endregion

//#region [rgba(0,255,0,0.1)]
/**
 * @brief Store Nodes to CSV File
 * @param path path to CSV File
 */
static void StoreNodes(char *path)
{
    int nodesCSV_Rows = CSVROWS; // add one for Headers
    int nodesCSV_Colums = CSVCOLUMS;
    int nodesCSV_Length = CSVLENGTH;
    char nodesCSV[nodesCSV_Rows][nodesCSV_Colums][nodesCSV_Length];
    createCSV(nodesCSV_Rows, nodesCSV_Colums, nodesCSV_Length, nodesCSV);

    int b = 0;
    int colum_Type = b++;
    int colum_Name = b++;
    int colum_Description = b++;
    int colum_DisplayName = b++;
    int colum_Parrent = b++;
    int colum_Write = b++;
    int colum_Min = b++;
    int colum_Max = b++;
    int colum_DataSource = b++;
    int colum_CSVName = b++;
    int colum_CSVIdentifierColum = b++;
    int colum_CSVIdentifier = b++;
    int colum_CSVValueColum = b++;
    int colum_MethodCallback = b++;

    // Setup Colum Names
    strcpy(nodesCSV[0][colum_Type], "Type");
    strcpy(nodesCSV[0][colum_Name], "Name");
    strcpy(nodesCSV[0][colum_Description], "Description");
    strcpy(nodesCSV[0][colum_DisplayName], "DisplayName");
    strcpy(nodesCSV[0][colum_Parrent], "Parrent");
    strcpy(nodesCSV[0][colum_Write], "Write");
    strcpy(nodesCSV[0][colum_Min], "Min");
    strcpy(nodesCSV[0][colum_Max], "Max");
    strcpy(nodesCSV[0][colum_DataSource], "DataSource");
    strcpy(nodesCSV[0][colum_CSVName], "CSVName");
    strcpy(nodesCSV[0][colum_CSVIdentifierColum], "CSVIdentifierColum");
    strcpy(nodesCSV[0][colum_CSVIdentifier], "CSVIdentifier");
    strcpy(nodesCSV[0][colum_CSVValueColum], "CSVValueColum");
    strcpy(nodesCSV[0][colum_MethodCallback], "MethodCallback");

    // Fill Nodes into Colums
    for (int a = 1; a < nodesCSV_Rows; a++)
    {
        int nodeIndex = a - 1;
        sprintf(nodesCSV[a][colum_Type], "%d", Nodes[nodeIndex].Type);
        sprintf(nodesCSV[a][colum_Name], "%s", Nodes[nodeIndex].Name);
        sprintf(nodesCSV[a][colum_Description], "%s", Nodes[nodeIndex].Description);
        sprintf(nodesCSV[a][colum_DisplayName], "%s", Nodes[nodeIndex].DisplayName);
        sprintf(nodesCSV[a][colum_Parrent], "%s", Nodes[nodeIndex].Parrent);
        sprintf(nodesCSV[a][colum_Write], "%d", Nodes[nodeIndex].Write);
        sprintf(nodesCSV[a][colum_Min], "%f", Nodes[nodeIndex].Min);
        sprintf(nodesCSV[a][colum_Max], "%f", Nodes[nodeIndex].Max);
        sprintf(nodesCSV[a][colum_Type], "%d", Nodes[nodeIndex].DataSource);


        sprintf(nodesCSV[a][colum_CSVName], "%s", Nodes[nodeIndex].CSVName);
        sprintf(nodesCSV[a][colum_CSVIdentifierColum], "%s", Nodes[nodeIndex].CSVIdentifierColum);
        sprintf(nodesCSV[a][colum_CSVIdentifier], "%s", Nodes[nodeIndex].CSVIdentifier);
        sprintf(nodesCSV[a][colum_CSVValueColum], "%s", Nodes[nodeIndex].CSVValueColum);

        sprintf(nodesCSV[a][colum_MethodCallback], "%s", Nodes[nodeIndex].MethodCallback);
    }

    writeCSV(
        path,
        nodesCSV_Rows,
        nodesCSV_Colums,
        nodesCSV_Length,
        nodesCSV);
}
//#endregion

//#region [rgba(0,0,255,0.1)]
/**
 * @brief get a NodeId from List
 * @param variant1 UA_Variant
 * @param variant2 UA_Variant
 */
static UA_NodeId getNodeId(char *name)
{
    if (name == NULL)
    {
        printf("Pointer is null");
        return UA_NODEID_NULL;
    }
    if (strlen(name) == 0)
    {
        return UA_NODEID_NULL;
    }

    if (strcmp(name, "ORGANIZES") == 0)
    {
        return UA_NODEID_NUMERIC(0, UA_NS0ID_OBJECTSFOLDER);
    }

    for (int a = 0; a < NODESCOUNT; a++)
    {
        if (strcmp(Nodes[a].Name, name) == 0)
        {
            return Nodes[a].NodeId;
        }
    }
    //*/
    return UA_NODEID_NULL;
}
//#endregion

//#region [rgba(255,0,255,0.1)]
/**
 * @brief Adds a Node from list to Server
 * @param server The UA Server
 * @param base BaseID
 * @param node the nodeset
 */
static void AddDynamicNode(
    UA_Server *server,
    struct myNode *node)
{
    if (node->Created) {
        return;
    }
    node->Created = true;
    /*if (node->Parrent == NULL)
    {
        UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND, "Parrent is null:%s", node->Name);
        return;
    }*/
    
    if (strlen(node->Parrent) == 0)
    {
        // UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND, "Parrent length is null:%s", node->Name);
        return;
    }
    // Find Parrend Node Id

    UA_NodeId parrent = getNodeId(node->Parrent);

    if (node->Type == TypeObject)
    {
        UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND, "Create Folder:%s", node->Name);
        UA_ObjectAttributes oAttr = UA_ObjectAttributes_default;
        oAttr.displayName = UA_LOCALIZEDTEXT("en-US", node->DisplayName);
        oAttr.description = UA_LOCALIZEDTEXT("en-US", node->Description);
        UA_Server_addObjectNode(
            server,
            UA_NODEID_NULL,
            parrent,
            UA_NODEID_NUMERIC(0, UA_NS0ID_ORGANIZES),
            UA_QUALIFIEDNAME(1, node->Name),
            UA_NODEID_NUMERIC(0, UA_NS0ID_BASEOBJECTTYPE),
            oAttr,
            NULL,
            &(node->NodeId));

        //*/
    }
    else if (node->Type == TypeMethod)
    {
        UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND, "Create Method:%s", node->Name);
        UA_MethodAttributes Attr = UA_MethodAttributes_default;
        Attr.description = UA_LOCALIZEDTEXT("en-US",node->DisplayName);
        Attr.displayName = UA_LOCALIZEDTEXT("en-US",node->Description);
        Attr.executable = true;
        Attr.userExecutable = true;

        NodesetAddMethodNode(server,node,parrent,&Attr);
        /*
        Todo in NodesetAddMethodNode:

        void AddMethodNode(UA_Server* server,struct myNode* node,UA_MethodAttributes* Attr) {
            
            if (node->any == something)
            {

            UA_Argument inputArgument;
            UA_Argument_init(&inputArgument);
            inputArgument.description = UA_LOCALIZEDTEXT("en-US", "A String");
            inputArgument.name = UA_STRING("MyInput");
            inputArgument.dataType = UA_TYPES[UA_TYPES_STRING].typeId;
            inputArgument.valueRank = UA_VALUERANK_SCALAR;

            UA_Argument outputArgument;
            UA_Argument_init(&outputArgument);
            outputArgument.description = UA_LOCALIZEDTEXT("en-US", "A String");
            outputArgument.name = UA_STRING("MyOutput");
            outputArgument.dataType = UA_TYPES[UA_TYPES_STRING].typeId;
            outputArgument.valueRank = UA_VALUERANK_SCALAR;
            
            UA_Server_addMethodNode(
                server,
                UA_NODEID_NULL,
                parrent,
                UA_NODEID_NUMERIC(0, UA_NS0ID_HASCOMPONENT),
                UA_QUALIFIEDNAME(1,  node->Name),
                Attr, 
                &helloWorldMethodCallback,
                1,
                &inputArgument,
                1,
                &outputArgument,
                NULL,
                &(node->NodeId));
            }
        }

       
        */
    }
    else
    {
        UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND, "Create Variable:%s", node->Name);
        UA_VariableAttributes Attr = UA_VariableAttributes_default;
        if (node->Write)
        {
            Attr.accessLevel = UA_ACCESSLEVELMASK_READ | UA_ACCESSLEVELMASK_WRITE;
        }
        else
        {
            Attr.accessLevel = UA_ACCESSLEVELMASK_READ;
        }
        Attr.displayName = UA_LOCALIZEDTEXT("en-US", node->DisplayName);
        Attr.description = UA_LOCALIZEDTEXT("en-US", node->Description);

        if (node->Type == TypeInteger)
        {
            Attr.dataType = UA_TYPES[UA_TYPES_INT32].typeId;
            int defaultValue = 0;
            UA_Variant_setScalar(
                &Attr.value,
                &defaultValue,
                &UA_TYPES[UA_TYPES_INT32]);
        }
        else if (node->Type == TypeBoolean)
        {
            Attr.dataType = UA_TYPES[UA_TYPES_BOOLEAN].typeId;
            bool defaultValue = 0;
            UA_Variant_setScalar(
                &Attr.value,
                &defaultValue,
                &UA_TYPES[UA_TYPES_BOOLEAN]);
        }
        else if (node->Type == TypeString)
        {
            Attr.dataType = UA_TYPES[UA_TYPES_STRING].typeId;
            UA_String Text = UA_STRING("");
            UA_Variant_setScalar(
                &Attr.value,
                &Text,
                &UA_TYPES[UA_TYPES_STRING]);
        }
        else if (node->Type == TypeDouble)
        {
            Attr.dataType = UA_TYPES[UA_TYPES_DOUBLE].typeId;
            double defaultValue = 0;
            UA_Variant_setScalar(
                &Attr.value,
                &defaultValue,
                &UA_TYPES[UA_TYPES_DOUBLE]);
        }
        else if (node->Type == TypeDate)
        {
            UA_DateTime defaultValue;
            UA_Variant_setScalar(
                &Attr.value,
                &defaultValue,
                &UA_TYPES[UA_TYPES_DATETIME]);
        }

        UA_Server_addVariableNode(
            server,
            UA_NODEID_NULL,
            parrent,
            UA_NODEID_NUMERIC(0, UA_NS0ID_HASCOMPONENT),
            UA_QUALIFIEDNAME(1, node->Name),
            UA_NODEID_NUMERIC(0, UA_NS0ID_BASEDATAVARIABLETYPE),
            Attr,
            NULL,
            &(node->NodeId));
        //*/
    }
}
//#endregion

//#region [rgba(0,255,255,0.1)]
/**
 * @brief Store Nodes to CSV File
 * @param path path to CSV File
 */
static void UpdateDynamicNode(UA_Server *server,struct myNode *node, bool init);
static int AppandNodeToNodes(UA_Server* server,struct myNode *node)
{
    for (int a=0; a < NODESCOUNT;a++) {
        if (Nodes[a].Type == TypeNone) {
            Nodes[a] = *node;
            AddDynamicNode(server,&Nodes[a]);
            UpdateDynamicNode(server,&Nodes[a],true);
            return a;
        }
    }
    return -1;
}
//#endregion

//#region [rgba(255,0,255,0.1)]
/**
 * @brief Store Nodes to CSV File
 * @param path path to CSV File
 */
static int RemoveNodeFromNodes(UA_Server* server,struct myNode *node)
{
    for (int a=0; a < NODESCOUNT;a++) {
        if (CompareNode(node,&Nodes[a]))
        {            
            if (node->Type == Object)
            {
                for (int b=0; b < NODESCOUNT;b++) 
                {
                    if (strcmp(Nodes[b].Parrent, node->Name)==0) {
                        RemoveNodeFromNodes(server,&Nodes[b]);
                    }
                }
            }
             UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND, "Removed Node:%s", node->Name);
            UA_Server_deleteNode(server,node->NodeId,true);

            CreateNode(&Nodes[a]);


            return a;
        }
    }
    return -1;
}
//#endregion

//#region [rgba(0,255,255,0.1)]
/**
 * @brief Updates a Node
 * @param server The UA Server
 * @param node the nodeset
 */
static void UpdateDynamicNode(
    UA_Server *server,
    struct myNode *node, 
    bool init)
{

    if (!node->Created)
    {
        AddDynamicNode(server, node);
       
    }

    if (node->Type == TypeNone || node->Type == Object || node->Type == Method)
    {
        // Node is Object, nothing todo
        return;
    }

    bool write = node->Write && !init;

    int csvRows = CSVROWS;
    int csvColums = CSVCOLUMS;
    int csvLength = CSVLENGTH;

    char csv[csvRows][csvColums][csvLength];
    createCSV(csvRows, csvColums, csvLength, csv);
    if(getCSV(
        node->CSVName,
        csvRows,
        csvColums,
        csvLength,
        csv) == -1 && !write)
    {
        return;
    }

    int identColum = getCSVColum(
        csvRows,
        csvColums,
        csvLength,
        csv,
        node->CSVIdentifierColum);
    if (identColum < 0)
    {
        if (write)
        {
            identColum = addCSVColum(csvRows,
                                     csvColums,
                                     csvLength,
                                     csv,
                                     node->CSVIdentifierColum);
            if (identColum < 0)
            {
                UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND,
                            "[Write] CSV:%s: cannot add Colum:%s -> %d",
                            node->CSVName,
                            node->CSVIdentifierColum,
                            identColum);
                return;
            }
        }
        else
        {
            UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND,
                        "[Read ] CSV:%s: cannot find Colum:%s -> %d",
                        node->CSVName,
                        node->CSVIdentifierColum,
                        identColum);
            return;
        }
    }
    int currentRow = getCSVRow(
        csvRows,
        csvColums,
        csvLength,
        csv,
        identColum,
        node->CSVIdentifier);
    if (currentRow < 0)
    {
        if (write)
        {
            currentRow = addCSVRow(csvRows,
                                   csvColums,
                                   csvLength,
                                   csv,
                                   identColum,
                                   node->CSVIdentifier);
            if (currentRow < 0)
            {
                UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND,
                            "[Write] CSV:%s: cannot add Row:%s -> %d",
                            node->CSVName,
                            node->CSVIdentifier,
                            currentRow);
                return;
            }
        }
        else
        {
            UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND,
                        "[Read ] CSV:%s: cannot find Row:%s -> %d",
                        node->CSVName,
                        node->CSVIdentifier,
                        currentRow);
            return;
        }
    }

    int currentColum = getCSVColum(
        csvRows,
        csvColums,
        csvLength,
        csv,
        node->CSVValueColum);
    if (currentColum < 0)
    {
        if (write)
        {
            currentColum = addCSVColum(csvRows,
                                       csvColums,
                                       csvLength,
                                       csv,
                                       node->CSVValueColum);
            if (currentColum < 0)
            {
                UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND,
                            "[Write] CSV:%s: cannot add Colum:%s -> %d",
                            node->CSVName,
                            node->CSVValueColum,
                            currentColum);
                return;
            }
        }
        else
        {
            UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND,
                        "[Read ] CSV:%s: cannot find Colum:%s -> %d",
                        node->CSVName,
                        node->CSVValueColum,
                        currentColum);
            return;
        }
    }


       // UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND, "Node %s R/W:%d from %s [%d][%d]=%s",node->Name, write , node->CSVName,currentRow, currentColum, csv[currentRow][currentColum]);


    UpdateNode(
        server,
        node->NodeId,
        csv[currentRow][currentColum],
        write);

    if (write)
    {
        writeCSV(
            node->CSVName,
            csvRows,
            csvColums,
            csvLength,
            csv);
    }
    //*/
}
//#endregion

//#region [rgba(255,255,0,0.1)]
/**
 * @brief Updates a Node
 * @param server The UA Server
 * @param node the nodeset
 */
static void UpdateDynamicNodes(UA_Server *server, bool init)
{
    for (int a = 0; a < NODESCOUNT; a++)
    {
        // UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND, "Node:%d,%s", a, Nodes[a].Name);
        UpdateDynamicNode(
            server,
            &Nodes[a], init);
    }
}
//#endregion

#endif