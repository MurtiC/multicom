

#ifndef updateNodes_c
#define updateNodes_c


#include "../open62541/open62541.h"
#include <signal.h>
#include <stdlib.h>
#include "../readFile.c"

static bool UA_VariantToString(const UA_Variant *variant, char* value) ;

//#region [rgba(255,0,0,0.15)] 
/**
 * @brief Compare a UA_Variant with UA_Variant
 * @param variant1 UA_Variant
 * @param variant2 UA_Variant
 */
static bool UA_VariantCompare(const UA_Variant *variant1,const UA_Variant *variant2)
{
    // Check if variant1 is a string
    if (UA_Variant_hasScalarType(variant1, &UA_TYPES[UA_TYPES_STRING]))
    {
        // Check if variant2 is a string
        if (!UA_Variant_hasScalarType(variant2, &UA_TYPES[UA_TYPES_STRING]))
        {
            //UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND,"[UA_VariantCompare] Variant1: STRING, Variant2: Not STRING");
            return false;
        }
        
        // Compare variant1 string to variant2 string
        
        char text1[1024];
        char text2[1024];
        UA_VariantToString(variant1,text1);
        UA_VariantToString(variant2,text2);
        //UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND,"[UA_VariantCompare] Variant1: STRING, Variant2: STRING-> %s, %s",text1,text2);
        return strcmp(text1, text2) == 0;
    }
    // Check if variant1 is a datetime
    else if (UA_Variant_hasScalarType(variant1, &UA_TYPES[UA_TYPES_DATETIME]))
    {
        // Check if variant2 is a datetime
        if (!UA_Variant_hasScalarType(variant2, &UA_TYPES[UA_TYPES_DATETIME]))
        {
            //UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND,"[UA_VariantCompare] Variant1: DATETIME, Variant2: Not DATETIME");
            return false;
        }
        // Convert variant to date struct
        UA_DateTime value1 = *((UA_DateTime *)variant1->data);
        UA_DateTimeStruct dateStruct1 = UA_DateTime_toStruct(value1);

        UA_DateTime value2 = *((UA_DateTime *)variant2->data);
        UA_DateTimeStruct dateStruct2 = UA_DateTime_toStruct(value2);
        // compare date structs
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
    // Check if variant1 is a int32
    else if (UA_Variant_hasScalarType(variant1, &UA_TYPES[UA_TYPES_INT32]))
    {
        // Check if variant2 is a int32
        if (!UA_Variant_hasScalarType(variant2, &UA_TYPES[UA_TYPES_INT32]))
        {
            //UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND,"[UA_VariantCompare] Variant1: INT32, Variant2: Not INT32");
            return false;
        }
        // Convert variant to int
        int value1 = *((int *)variant1->data);
        int value2 = *((int *)variant2->data);
        // Compare values
        return value1 == value2;
    }
    // Check if variant1 is a bool
    else if (UA_Variant_hasScalarType(variant1, &UA_TYPES[UA_TYPES_BOOLEAN]))
    {
        // Check if variant2 is a bool
        if (!UA_Variant_hasScalarType(variant2, &UA_TYPES[UA_TYPES_BOOLEAN]))
        {
            //UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND,"[UA_VariantCompare] Variant1:BOOLEAN, Variant2: Not BOOLEAN");
            return false;
        }
        // Convert variant to bool
        bool value1 = *((bool *)variant1->data);
        bool value2 = *((bool *)variant2->data);
        // Compare values
        return value1 == value2;
    }
    // Check if variant1 is a double
    else if (UA_Variant_hasScalarType(variant1, &UA_TYPES[UA_TYPES_DOUBLE]))
    {
        // Check if variant2 is a double
        if (!UA_Variant_hasScalarType(variant2, &UA_TYPES[UA_TYPES_DOUBLE]))
        {
            //UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND,"[UA_VariantCompare] Variant1:DOUBLE, Variant2: Not DOUBLE");
            return false;
        }
        // Convert variant to double
        double value1 = *((double *)variant1->data);
        double value2 = *((double *)variant2->data);
        // Compare values
        return value1 == value2;
    }
    else
    {
        //UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND,"[UA_VariantCompare] Variant1:Unknown, Variant2: Unknown");
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
    // Convert char arr into UA_String
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
    if (strlen(text) == 0) {
        return false;
    }
    // Check if target variant is a string
    if (UA_Variant_hasScalarType(variant, &UA_TYPES[UA_TYPES_STRING]))
    {
        // update text in variant
        StringToUA_Variant(variant,text);
        return true;
    }
    // Check if target variant is a datetime
    else if (UA_Variant_hasScalarType(variant, &UA_TYPES[UA_TYPES_DATETIME]))
    {
        // Parseable Datetime formats
        // 2021-10-15;12:37:11.713864291
        // YYYY-MM-DD
        // hh:mm:ssssssssssss
        // YYYY-MM-DD;hh:mm:ssssssssssss
        // YYYY-MM-DD hh:mm:ssssssssssss
        // 1    2  3  4  5  6
        // Seperate Text in Datetime
        int textlen = strlen(text);
        char year[10] = "";
        char month[10] = "";
        char day[10] = "";
        char hour[10] = "";
        char minute[10] = "";
        char second[20] = "";
        int mode = 0;
        char tmpText[50] = "";
        // Iterate through the text and check for a seperator
        for (int a = 0; a < textlen; a++)
        {
            // If the char is a : oder a end text then copy the text to the time values
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
            // If the char is a - oder a end text then copy the text to the date values
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
            // If the char is a end text then reset
            else if (text[a] == ';' || text[a] == ' ')
            { // Split Seperator
                mode = 0;
            }
            // If char is other then appand to the temporary storrage
            else
            {
                strncat(tmpText, &text[a], 1);
            }
        }
        // Convert datetime texts to datetime struct
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
            if ( uaText.data[a] == ';') {
                 newtext[a] = ',';
            }
            else {
                newtext[a] = uaText.data[a];
            }
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
static int UpdateNode(
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
       if (UA_VariantToText(&valueVariant, text)) {
           return 1;
       }
       else {
           return -1;
       }
    }
    else
    {
        UA_Variant newValueVariant = valueVariant;

        if (strlen(text)==0) {
            return -3;
            //UA_Variant_setScalarCopy(newValueVariant, value, &UA_TYPES[UA_TYPES_BOOLEAN]);
        }
        char oldText[100];
        UA_VariantToText(&valueVariant,oldText);
        
        
        bool result =  TextToUA_Variant(text, &newValueVariant);

        char newText[100];
        UA_VariantToText(&newValueVariant,newText);
        
        bool equals = UA_VariantCompare(&newValueVariant, &valueVariant);

        //UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND, "UpdateNode Read:Input:%s Old:%s New:%s equals:%d result:%d",text,oldText,newText,equals,result);

        if (!equals && result)
        {
           UA_Server_writeValue(server, nodeId, newValueVariant);
           return 2;
        }
        return -2;
    }
}
//#endregion

#endif
