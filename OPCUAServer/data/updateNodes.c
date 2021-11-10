

#ifndef updateNodes_c
#define updateNodes_c

#include "dataStruct.c"
#include "../open62541/open62541.h"
#include <signal.h>
#include <stdlib.h>
#include "../readFile.c"

//#region [rgba(255,0,0,0.15)] 
/**
 * @brief Compare a UA_Variant with UA_Variant
 * @param variant1 UA_Variant
 * @param variant2 UA_Variant
 */
static bool UA_VariantCompare(UA_Variant *variant1, UA_Variant *variant2)
{
    if (UA_Variant_hasScalarType(variant1, &UA_TYPES[UA_TYPES_STRING]))
    {
        if (!UA_Variant_hasScalarType(variant2, &UA_TYPES[UA_TYPES_STRING]))
        {
            return false;
        }
        return strcmp(variant1->data, variant2->data) != 0;
    }
    else if (UA_Variant_hasScalarType(variant1, &UA_TYPES[UA_TYPES_DATETIME]))
    {
        if (!UA_Variant_hasScalarType(variant2, &UA_TYPES[UA_TYPES_DATETIME]))
        {
            return false;
        }
        UA_DateTime value1 = *((UA_DateTime *)variant1->data);
        UA_DateTimeStruct dateStruct1 = UA_DateTime_toStruct(value1);

        UA_DateTime value2 = *((UA_DateTime *)variant2->data);
        UA_DateTimeStruct dateStruct2 = UA_DateTime_toStruct(value2);

        return dateStruct1.nanoSec == dateStruct2.nanoSec &&
               dateStruct1.microSec == dateStruct2.microSec &&
               dateStruct1.milliSec == dateStruct2.milliSec &&
               dateStruct1.sec == dateStruct2.sec &&
               dateStruct1.min == dateStruct2.min &&
               dateStruct1.hour == dateStruct2.hour &&
               dateStruct1.day == dateStruct2.day &&
               dateStruct1.month == dateStruct2.month &&
               dateStruct1.year == dateStruct2.year;
    }
    else if (UA_Variant_hasScalarType(variant1, &UA_TYPES[UA_TYPES_INT32]))
    {
        if (!UA_Variant_hasScalarType(variant2, &UA_TYPES[UA_TYPES_INT32]))
        {
            return false;
        }
        int value1 = *((int *)variant1->data);
        int value2 = *((int *)variant2->data);
        return value1 == value2;
    }
    else if (UA_Variant_hasScalarType(variant1, &UA_TYPES[UA_TYPES_BOOLEAN]))
    {
        if (!UA_Variant_hasScalarType(variant2, &UA_TYPES[UA_TYPES_BOOLEAN]))
        {
            return false;
        }
        bool value1 = *((bool *)variant1->data);
        bool value2 = *((bool *)variant2->data);
        return value1 == value2;
    }
    else if (UA_Variant_hasScalarType(variant1, &UA_TYPES[UA_TYPES_DOUBLE]))
    {
        if (!UA_Variant_hasScalarType(variant2, &UA_TYPES[UA_TYPES_DOUBLE]))
        {
            return false;
        }
        double value1 = *((double *)variant1->data);
        double value2 = *((double *)variant2->data);
        return value1 == value2;
    }
    else
    {
        return false;
    }
    return false;
}
//#endregion

//#region [rgba(255,255,255,0.15)] Convert from Type to Variant

/**
 * @brief Convert int to UA_Variant 
 * @param variant UA_Variant
 * @param value int
 */
static void IntToUA_Variant(UA_Variant *variant, int* value)
{
    UA_Variant_setScalarCopy(variant, value, &UA_TYPES[UA_TYPES_INT32]);
}

/**
 * @brief Convert bool to UA_Variant 
 * @param variant UA_Variant
 * @param value bool
 */
static void BoolToUA_Variant(UA_Variant *variant,bool* value)
{
     UA_Variant_setScalarCopy(variant, value, &UA_TYPES[UA_TYPES_BOOLEAN]);
}

/**
 * @brief Convert double to UA_Variant 
 * @param variant UA_Variant
 * @param value double
 */
static void DoubleToUA_Variant( UA_Variant *variant,double* value)
{
    UA_Variant_setScalarCopy(variant, value, &UA_TYPES[UA_TYPES_DOUBLE]);
}

/**
 * @brief Convert string to UA_Variant 
 * @param variant UA_Variant
 * @param value string
 */
static void StringToUA_Variant(UA_Variant *variant,char* value) 
{
    UA_String uaText = UA_String_fromChars(value);
    UA_Variant_setScalarCopy(variant, &uaText, &UA_TYPES[UA_TYPES_STRING]);
}

/**
 * @brief Cast a Text to variant type of variant and insert into variant
 * @param text string
 * @param variant UA_Variant
 */
static bool TextToUA_Variant(char *text, UA_Variant *variant)
{
    if (UA_Variant_hasScalarType(variant, &UA_TYPES[UA_TYPES_STRING]))
    {
        // UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND, text);
        //UA_String uaText = UA_String_fromChars(text);
        //UA_Variant_setScalarCopy(variant, &uaText, &UA_TYPES[UA_TYPES_STRING]);
        StringToUA_Variant(variant,text);
        return true;
    }
    else if (UA_Variant_hasScalarType(variant, &UA_TYPES[UA_TYPES_DATETIME]))
    {
        // 2021-10-15;12:37:11.713864291
        // YYYY-MM-DD
        // hh:mm:ssssssssssss
        // YYYY-MM-DD;hh:mm:ssssssssssss
        // YYYY-MM-DD hh:mm:ssssssssssss
        // 1    2  3  4  5  6
        int textlen = strlen(text);
        char year[10] = "";
        char month[10] = "";
        char day[10] = "";
        char hour[10] = "";
        char minute[10] = "";
        char second[20] = "";
        int mode = 0;

        char tmpText[50] = "";
        for (int a = 0; a < textlen; a++)
        {
            if ((mode == 0 || mode > 3) && (text[a] == ':' || text[a] == ';' || text[a] == ' ' || text[a] == '\0' || a == textlen - 1)) // Time Seperator
            {
                if (mode == 0)
                {
                    mode = 4;
                }
                if (mode == 4)
                {
                    strcpy(hour, tmpText);
                    mode = 5;
                }
                else if (mode == 5)
                {
                    strcpy(minute, tmpText);
                    mode = 6;
                }
                else if (mode == 6)
                {
                    strcpy(second, tmpText);
                    mode = 7;
                }
                strcpy(tmpText, "");
            }
            else if ((mode == 0 || mode <= 3) && (text[a] == '-' || text[a] == ';' || text[a] == ' ' || text[a] == '\0' || a == textlen - 1))
            {
                // Date Seperator
                if (mode == 0 || mode > 3)
                {
                    mode = 1;
                }
                if (mode == 1)
                {
                    strcpy(year, tmpText);
                    mode = 2;
                }
                else if (mode == 2)
                {
                    strcpy(month, tmpText);
                    mode = 3;
                }
                else if (mode == 3)
                {
                    strcpy(day, tmpText);
                    mode = 4;
                }
                strcpy(tmpText, "");
            }
            else if (text[a] == ';' || text[a] == ' ')
            { // Split Seperator
                mode = 0;
            }
            else
            {
                strncat(tmpText, &text[a], 1);
            }
        }

        UA_DateTimeStruct dateStruct;
        dateStruct.year = atoi(year);
        dateStruct.month = atoi(month);
        dateStruct.day = atoi(day);

        dateStruct.hour = atoi(hour);
        dateStruct.min = atoi(minute);
        char *eptr;
        double seconds = strtod(second, &eptr);
        if (errno == ERANGE)
        {
            return false;
        }

        dateStruct.sec = (int)seconds;
        seconds = (seconds - dateStruct.sec) * 1000;
        dateStruct.milliSec = (int)seconds;
        seconds = (seconds - dateStruct.milliSec) * 1000;
        dateStruct.microSec = (int)seconds;
        seconds = (seconds - dateStruct.microSec) * 1000;
        dateStruct.nanoSec = (int)seconds;

        /*char printer[1000] = "";
        sprintf(printer,"Text:%s-%s-%s %s:%s:%s",
        year,
        month,
        day,
        hour,
        minute,
        second
        );
        UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND, printer);

        char printer1[1000] = "";
        sprintf(printer1,"Date:%i-%i-%i %i:%i:%i, %i, %i, %i",
        dateStruct.year,
        dateStruct.month,
        dateStruct.day,
        dateStruct.hour,
        dateStruct.min,
        dateStruct.sec,
        dateStruct.milliSec,
        dateStruct.microSec,
        dateStruct.nanoSec
        );
        UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND, printer1);
*/

        UA_DateTime value = UA_DateTime_fromStruct(dateStruct);
        UA_Variant_setScalarCopy(variant, &value, &UA_TYPES[UA_TYPES_DATETIME]);
        return true;
    }
    else if (UA_Variant_hasScalarType(variant, &UA_TYPES[UA_TYPES_INT32]))
    {
        int value = atoi(text);
        IntToUA_Variant(variant,&value);
        //UA_Variant_setScalarCopy(variant, &value, &UA_TYPES[UA_TYPES_INT32]);
        return true;
    }
    else if (UA_Variant_hasScalarType(variant, &UA_TYPES[UA_TYPES_BOOLEAN]))
    {
        bool value = strcmp(text,"true")==0 || strcmp(text,"1")==0;
        BoolToUA_Variant(variant,&value);
        //UA_Variant_setScalarCopy(variant, &value, &UA_TYPES[UA_TYPES_BOOLEAN]);
        return true;
    }
    else if (UA_Variant_hasScalarType(variant, &UA_TYPES[UA_TYPES_DOUBLE]))
    {
        char *eptr;
        double value = strtod(text, &eptr);
        if (errno == ERANGE)
        {
            return false;
        }
        DoubleToUA_Variant(variant,&value);
        //UA_Variant_setScalarCopy(variant, &value, &UA_TYPES[UA_TYPES_DOUBLE]);
        return true;
    }
    return false;
}
//#endregion

//#region [rgba(0,255,255,0.15)] Convert from Variant to Type

/**
 * @brief Convert a UA_Variant to Int
 * @param variant UA_Variant
 */
static int UA_VariantToInt(const UA_Variant *variant)
{
    if (UA_Variant_hasScalarType(variant, &UA_TYPES[UA_TYPES_INT32]))
    {
        int value = *((int *)variant->data);
        return value;
    }
    return 0;
}

/**
 * @brief Convert a UA_Variant to Bool
 * @param variant UA_Variant
 */
static bool UA_VariantToBool(const UA_Variant *variant)
{
    if (UA_Variant_hasScalarType(variant, &UA_TYPES[UA_TYPES_BOOLEAN]))
    {
        bool value = *((bool *)variant->data);
        return value;
    }
    return false;
}

/**
 * @brief Convert a UA_Variant to Double
 * @param variant UA_Variant
 */
static double UA_VariantToDouble(const UA_Variant *variant)
{
    if (UA_Variant_hasScalarType(variant, &UA_TYPES[UA_TYPES_DOUBLE]))
    {
        double value = *((double *)variant->data);
        return value;
    }
    return 0;
}

/**
 * @brief Convert UA_Variant to string
 * @param variant UA_Variant
 * @param value string
 */
static bool UA_VariantToString(const UA_Variant *variant, char* value) 
{
    if (UA_Variant_hasScalarType(variant, &UA_TYPES[UA_TYPES_STRING]))
    {
        UA_String uaText = *((UA_String *)variant->data);
        char newtext[uaText.length + 2];
        int a = 0;
        for (a = 0; a < uaText.length; a++)
        {
            newtext[a] = uaText.data[a];
        }
        newtext[a] = '\0';
        strcpy(value, newtext);
        return true;
    }
    return false;
}

/**
 * @brief Convert a UA_Variant to String
 * @param variant UA_Variant
 * @param text string
 */
static bool UA_VariantToText(const UA_Variant *variant, char *text)
{
    if (UA_Variant_hasScalarType(variant, &UA_TYPES[UA_TYPES_STRING]))
    {
        return UA_VariantToString(variant,text);
        /*
        UA_String uaText = *((UA_String *)variant->data);
        char newtext[uaText.length + 2];
        int a = 0;
        for (a = 0; a < uaText.length; a++)
        {
            newtext[a] = uaText.data[a];
        }
        newtext[a] = '\0';
        strcpy(text, newtext);
        return true;
        */
    }
    else if (UA_Variant_hasScalarType(variant, &UA_TYPES[UA_TYPES_DATETIME]))
    {
        UA_DateTime value = *((UA_DateTime *)variant->data);
        UA_DateTimeStruct dateStruct = UA_DateTime_toStruct(value);
        strcpy(text, "");

        double seconds = dateStruct.sec;
        seconds = seconds + dateStruct.milliSec / 1000;
        seconds = seconds + dateStruct.microSec / 1000000;
        seconds = seconds + dateStruct.nanoSec / 1000000000;

        // 2021-10-15;12:37:11.713864291
        // YYYY-MM-DD hh:mm:ssssssssssss
        sprintf(
            text,
            "%02d-%02d-%02d %02d:%02d:%02.9f",
            dateStruct.year,
            dateStruct.month,
            dateStruct.day,
            dateStruct.hour,
            dateStruct.min,
            seconds);
        return true;
    }
    else if (UA_Variant_hasScalarType(variant, &UA_TYPES[UA_TYPES_INT32]))
    {
        int value = UA_VariantToInt(variant);
        //int value = *((int *)variant->data);
        sprintf(text, "%d", value);
        return true;
    }
    else if (UA_Variant_hasScalarType(variant, &UA_TYPES[UA_TYPES_BOOLEAN]))
    {
        bool value = UA_VariantToBool(variant);
        //bool value = *((bool *)variant->data);
        if (value)
        {
            strcpy(text, "true");
        }
        else
        {
            strcpy(text, "false");
        }
        return true;
    }
    else if (UA_Variant_hasScalarType(variant, &UA_TYPES[UA_TYPES_DOUBLE]))
    {
        double value = UA_VariantToDouble(variant);
        //double value = *((double *)variant->data);
        sprintf(text, "%f", value);
        return true;
    }
    return false;
}
//#endregion

//#region [rgba(255,0,0,0.15)]
/**
 * @brief Updates a variable node
 * @param server The UA_Server instance
 * @param nodeId Inserts the resulting nodeId of this node
 * @param text text with value
 * @param write True:reads from node to text. False: writes from text to node
 */
static void UpdateNode(
    UA_Server *server,
    UA_NodeId nodeId,
    char *text,
    bool write)
{
    UA_Variant valueVariant;
    UA_Server_readValue(server, nodeId, &valueVariant);
    if (UA_Variant_isEmpty(&valueVariant))
    {
        write = false;
    }

    if (write)
    {
        UA_VariantToText(&valueVariant, text);
    }
    else
    {
        //char oldText[100];
        //UA_VariantToText(&valueVariant,oldText);
        
        UA_Variant newValueVariant = valueVariant;
        bool result =  TextToUA_Variant(text, &newValueVariant);

        //char newText[100];
        //UA_VariantToText(&newValueVariant,newText);
        
        bool equals =UA_VariantCompare(&newValueVariant, &valueVariant);

        //UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND, "UpdateNode Read:Input:%s Old:%s New:%s equals:%d result:%d",text,oldText,newText,equals,result);

        if (!equals && result)
        {
           UA_Server_writeValue(server, nodeId, newValueVariant);
        }
    }
}
//#endregion

//#region [rgba(0,255,0,0.15)]
/**
 * @brief Updates variable nodes of Type NodePin to parrent node
 * @param server The UA_Server instance
 * @param parrent NodeId of the parrent node
 * @param types The types definitions struct
 * @param variables The struct for the resulting nodeIds
 * @param name The name of the node
 */
/*
static void UpdateNodeVariable(
    UA_Server *server,
    struct nodeVariable *variables)
{

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

     UpdateStringNode(
          server,
         (variables->Unit),
         "Unit",
         ""
     );
     UpdateDoubleNode( server,
         (variables->Value),
         "Value",
         0.00);
 
}*/
//#endregion

//#region [rgba(255,255,0,0.15)]
/**
 * @brief Updates variable nodes of Type NodeTemperaturSensor to parrent node
 * @param server The UA_Server instance
 * @param parrent NodeId of the parrent node
 * @param types The types definitions struct
 * @param variables The struct for the resulting nodeIds
 * @param name The name of the node
 */
/* 
static void UpdateNodeTemperaturSensor(
    UA_Server *server,
    struct nodeTemperaturSensor *variables)
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
      UpdateStringNode(
          server,
          (variables->Name),
          "Name",
          "");

      UpdateStringNode(
          server,
          (variables->Description),
          "Description",
          "");

      UpdateStringNode(
          server,
          (variables->Location),
          "Location",
          "");

      UpdateIntNode(
          server,
          (variables->Antenna),
          "Antenna",
          0);

       UpdateNodeVariable(
          server,
          variables->Folder,
          types,
          &(variables->Temperatur),
          "Temperatur");
      UpdateNodeVariable(
          server,
          variables->Folder,
          types,
          &(variables->MaxTemperatur),
          "MaxTemperatur");
       UpdateNodeVariable(
          server,
          variables->Folder,
          types,
          &(variables->MinTemperatur),
          "MinTemperatur");
  
}*/
//#endregion

//#region [rgba(0,255,255,0.15)]
/**
 * @brief Updates variable nodes of Type NodePin to parrent node
 * @param server The UA_Server instance
 * @param parrent NodeId of the parrent node
 * @param types The types definitions struct
 * @param variables The struct for the resulting nodeIds
 * @param name The name of the node
 */
/*
static void UpdateNodePin(
    UA_Server *server,
    struct nodePin *variables,
    int configRows,
    int configColums,
    int configLength,
    char configCSV[configRows][configColums][configLength],
    int currentRows,
    int currentColums,
    int currentLength,
    char currentCSV[currentRows][currentColums][currentLength],
    int currentRow,
    bool write,
    bool output)
{
    bool read = 0;
    int currentColum = 0;
    currentColum = getCSVColum(
        configRows,
        configColums,
        configLength,
        configCSV,
        "Name");
    if (currentColum >= 0)
    {
        UpdateNode(
            server,
            (variables->Name),
            configCSV[currentRow][currentColum],
            write);
    }

    currentColum = getCSVColum(
        configRows,
        configColums,
        configLength,
        configCSV,
        "Description");
    if (currentColum >= 0)
    {
        UpdateNode(
            server,
            (variables->Description),
            configCSV[currentRow][currentColum],
            write);
    }
    currentColum = getCSVColum(
        configRows,
        configColums,
        configLength,
        configCSV,
        "Location");
    if (currentColum >= 0)
    {
        UpdateNode(
            server,
            (variables->Location),
            configCSV[currentRow][currentColum],
            write);
    }

    currentColum = getCSVColum(
        configRows,
        configColums,
        configLength,
        configCSV,
        "PinMode");
    if (currentColum >= 0)
    {
        UpdateNode(
            server,
            (variables->Mode),
            configCSV[currentRow][currentColum],
            read);
    }

    currentColum = getCSVColum(
        currentRows,
        currentColums,
        currentLength,
        currentCSV,
        "PinState");
    if (currentColum >= 0)
    {
        UpdateNode(
            server,
            (variables->Status.Value),
            currentCSV[currentRow][currentColum],
            output);
    }
/*
    /*UA_ObjectAttributes oAttr = UA_ObjectAttributes_default;
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
    UpdateStringNode(
        server,
        (variables->Name),
        "Name",
        "");

    UpdateStringNode(
        server,
        (variables->Description),
        "Description",
        "");

    UpdateStringNode(
        server,
        (variables->Location),
        "Location",
        "");
    UpdateIntNode(
        server,
        (variables->Pin),
        "Pin",
        0);
    UpdateBoolNode(
        server,
        (variables->Pin),
        "Mode",
        false);


    UpdateNodeVariable(
        server,
        variables->Folder,
        types,
        &(variables->Status),
        "Status");

}*/
//#endregion

//#region [rgba(0,0,255,0.15)]
/**
 * @brief Updates variable nodes of Type RFIDReader to parrent node
 * @param server The UA_Server instance
 * @param parrent NodeId of the parrent node
 * @param types The types definitions struct
 * @param variables The struct for the resulting nodeIds
 * @param name The name of the node
 */
/*
static void UpdateNodeRFIDReader(
    UA_Server *server,
    struct nodeRFIDReader *variables,
    bool init)
{
    bool write = !init;
    bool read = false;
    int currentRow = 1;
    int currentColum = 1;
    int length = 256;
    // config
    int configRows = 10;
    int configColums = 10;
    char configCSV[configRows][configColums][length];
    createCSV(configRows, configColums, length, configCSV);
    getCSV(
        "/files/reader/config.csv",
        configRows,
        configColums,
        length,
        configCSV);
    // opcCurrent
    int opcRows = 10;
    int opcColums = 15;
    char opcCSV[opcRows][opcColums][length];
    createCSV(opcRows, opcColums, length, opcCSV);
    getCSV(
        "/files/reader/opcCurrent.csv",
        opcRows,
        opcColums,
        length,
        opcCSV);
    // readerCurrent
    int readerRows = 10;
    int readerColums = 15;
    char readerCSV[readerRows][readerColums][length];
    createCSV(readerRows, readerColums, length, readerCSV);
    getCSV(
        "/files/reader/readerCurrent.csv",
        readerRows,
        readerColums,
        length,
        readerCSV);
    ///////////////////////////////////
    currentColum = getCSVColum(
        configRows,
        configColums,
        length,
        configCSV,
        "Name");
    //  printf("\n");
    if (currentColum >= 0)
    {

        UpdateNode(
            server,
            (variables->Name),
            configCSV[currentRow][currentColum],
            write);
    }

    currentColum = getCSVColum(
        configRows,
        configColums,
        length,
        configCSV,
        "Description");
    if (currentColum >= 0)
    {
        UpdateNode(
            server,
            (variables->Description),
            configCSV[currentRow][currentColum],
            write);
    }
    currentColum = getCSVColum(
        configRows,
        configColums,
        length,
        configCSV,
        "Location");
    if (currentColum >= 0)
    {
        UpdateNode(
            server,
            (variables->Location),
            configCSV[currentRow][currentColum],
            write);
    }

    currentColum = getCSVColum(
        readerRows,
        readerColums,
        length,
        readerCSV,
        "IPAdress");
    if (currentColum >= 0)
    {
        UpdateNode(
            server,
            (variables->IPAdress),
            configCSV[currentRow][currentColum],
            read);
    }
    currentColum = getCSVColum(
        readerRows,
        readerColums,
        length,
        readerCSV,
        "Port");
    if (currentColum >= 0)
    {
        UpdateNode(
            server,
            (variables->Port),
            configCSV[currentRow][currentColum],
            write);
    }
    currentColum = getCSVColum(
        readerRows,
        readerColums,
        length,
        readerCSV,
        "SerialNumber");
    if (currentColum >= 0)
    {
        UpdateNode(
            server,
            (variables->SerialNumber),
            configCSV[currentRow][currentColum],
            write);
    }
    currentColum = getCSVColum(
        readerRows,
        readerColums,
        length,
        readerCSV,
        "Hardwarerevision");
    if (currentColum >= 0)
    {
        UpdateNode(
            server,
            (variables->Hardwarerevision),
            configCSV[currentRow][currentColum],
            write);
    }
    currentColum = getCSVColum(
        readerRows,
        readerColums,
        length,
        readerCSV,
        "Softwarerevision");
    if (currentColum >= 0)
    {
        UpdateNode(
            server,
            (variables->Softwarerevision),
            readerCSV[currentRow][currentColum],
            write);
    }
    int inputRow = 0;
    int outputRow = 0;
    int input = 0;
    int output = 0;
    for (int input = 0; input < sLesegeraetPinInputs; input++)
    {
        inputRow = currentRow + input+1;
        UpdateNodePin(
            server,
            &(variables->InputPins[input]),
            configRows,
            configColums,
            length,
            configCSV,
            readerRows,
            readerColums,
            length,
            readerCSV,
            inputRow,
            write,
            false
        );
    }

    for (int output = 0; output < sLesegeraetPinOutput; output++)
    {
        outputRow = inputRow + output + 1;
        UpdateNodePin(
            server,
            &(variables->InputPins[input]),
            configRows,
            configColums,
            length,
            configCSV,
            opcRows,
            opcColums,
            length,
            opcCSV,
            outputRow,
            write,
            true
        );
    }

    

    // printCSV( configRows,
    //    configColums,
    //     length,
    //     csv);
    // printf("\n");

    /*


    for (int a=0; a < sLesegeraetPinInputs;a++) {
        char name[32];
        sprintf(name,"Input %ld", a);
        UpdateNodePin( server,
        variables->Folder,
        types,
        &(variables->InputPins[a]),
        name);
    }

    for (int a=0; a < sLesegeraetPinOutput;a++) {
        char name[32];
        sprintf(name,"Output %ld", a);
        UpdateNodePin( server,
        variables->Folder,
        types,
        &(variables->OutputPins[a]),
        name);
    }

    writeCSV(
        "/files/reader/config.csv",
        configRows,
        configColums,
        length,
        configCSV);

    writeCSV(
        "/files/reader/opcCurrent.csv",
        opcRows,
        opcColums,
        length,
        opcCSV);

    /*writeCSV(
        "/files/reader/readerCurrent.csv",
        readerRows,
        readerColums,
        length,
        readerCSV);
    //
    // TemperaturSensorNodes are generated at runtime !
}*/
//#endregion

//#region [rgba(255,255,0,0.15)]
/**
 * @brief Updates variable nodes of Type TemperaturMonitoring to parrent node
 * @param server The UA_Server instance
 * @param parrent NodeId of the parrent node
 * @param types The types definitions struct
 * @param variables The struct for the resulting nodeIds
 * @param name The name of the node
 */
/*
static void UpdateNodeTemperaturMonitoring(
    UA_Server *server,
    struct nodeTemperaturMonitoring *variables,
    bool init)
{
    bool write = !init;
    bool read = false;
    int currentRow = 1;

    int configRows = 2;
    int configColums = 10;
    int length = 256;
    char csv[configRows][configColums][length];
    createCSV(configRows, configColums, length, csv);

    getCSV(
        "/files/reader/config.csv",
        configRows,
        configColums,
        length,
        csv);

    // printCSV( configRows,
    // configColums,
    // length,
    // csv);

    int columName = getCSVColum(
        configRows,
        configColums,
        length,
        csv,
        "Name");
    //  printf("\n");
    if (columName >= 0)
    {

        UpdateNode(
            server,
            (variables->Name),
            csv[currentRow][columName],
            write);
    }

    int columDescription = getCSVColum(
        configRows,
        configColums,
        length,
        csv,
        "Description");
    if (columDescription >= 0)
    {
        UpdateNode(
            server,
            (variables->Description),
            csv[currentRow][columDescription],
            write);
    }
    int columLocation = getCSVColum(
        configRows,
        configColums,
        length,
        csv,
        "Location");
    if (columLocation >= 0)
    {
        UpdateNode(
            server,
            (variables->Location),
            csv[currentRow][columLocation],
            write);
    }
    int columVersion = getCSVColum(
        configRows,
        configColums,
        length,
        csv,
        "Version");
    if (columVersion >= 0)
    {
        UpdateNode(
            server,
            (variables->Version),
            csv[currentRow][columVersion],
            write);
    }
    int columLastContact = getCSVColum(
        configRows,
        configColums,
        length,
        csv,
        "LastContact");
    if (columLastContact >= 0)
    {
        UpdateNode(
            server,
            (variables->LastContact),
            csv[currentRow][columLastContact],
            read);
    }

    // printCSV( configRows,
    //    configColums,
    //     length,
    //     csv);
    // printf("\n");
    writeCSV(
        "/files/reader/config.csv",
        configRows,
        configColums,
        length,
        csv);

    /*
        UpdateStringNode(
            server,
            (variables->Version),
            "Version");


        UA_DateTime LastContactText;
        UpdateDateNode(
            server,
            (variables->LastContact),
            LastContactText);

        UpdateNodeRFIDReader(
            server,
            (variables->Reader));
}*/
//#endregion

#endif
