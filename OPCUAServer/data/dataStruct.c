#ifndef typeStruct_c
#define typeStruct_c


#include "../open62541/open62541.h"
#include <signal.h>
#include <stdlib.h>

#define sLesegeraetPinInputs 2
#define sLesegeraetPinOutput 4
#define sLesegeraetTemperaturSensors 10



///////////////////////////////////////////////////////////////////////////////////////


struct nodeVariableType {
    UA_NodeId Type;
    UA_NodeId Value;
    UA_NodeId Unit;
};

struct nodeTemperaturSensorType {
    UA_NodeId Type;
    // Fields
    UA_NodeId Name;
    UA_NodeId Description;
    UA_NodeId Location;
    UA_NodeId TagId;
    UA_NodeId Temperatur;
    UA_NodeId MaxTemperatur;
    UA_NodeId MinTemperatur;
    UA_NodeId LastContact;
    // Events
    UA_NodeId TimeoutEvent;
    UA_NodeId AboveMaxTemperaturEvent;
    UA_NodeId BelowMaxTemperaturEvent;
};

struct nodePinType {
    UA_NodeId Type;
    // Fields
    UA_NodeId Name;
    UA_NodeId Description;
    UA_NodeId Location;

    UA_NodeId Pin;
    UA_NodeId Mode;
    UA_NodeId Status;
    // Events
    UA_NodeId ChangedEvent;
};

struct nodeRFIDReaderType {
    UA_NodeId Type;
    // Fields
    UA_NodeId Name;
    UA_NodeId Description;
    UA_NodeId Location;
    UA_NodeId IPAdress;
    UA_NodeId Port;
    UA_NodeId SerialNumber;
    UA_NodeId Hardwarerevision;
    UA_NodeId Softwarerevision;
    UA_NodeId InputPins;
    UA_NodeId OutputPins;
    // Events
    UA_NodeId ReaderTimeout;
    // Methoden
    UA_NodeId addTemperaturSensor;
    UA_NodeId removeTemperaturSensor;
    UA_NodeId setPinState;
};

struct nodeTemperaturMonitoringType {
    UA_NodeId Type;
    UA_NodeId Name;
    UA_NodeId Description;
    UA_NodeId Location;
    UA_NodeId Version;
    UA_NodeId LastContact;
    UA_NodeId RFIDReader;
    // Events
    UA_NodeId ReaderTimeoutEvent;
};

struct nodeTypes {
    struct nodeVariableType nodeVariable;
    struct nodeTemperaturSensorType nodeTemperaturSensor;
    struct nodePinType nodePin;
    struct nodeRFIDReaderType nodeRFIDReader;
    struct nodeTemperaturMonitoringType nodeTemperaturMonitoring;
};


// Variable
struct nodeVariable {
    UA_NodeId Folder;
    UA_NodeId Value;
    UA_NodeId Unit;
};

struct nodeTemperaturSensor {
    UA_NodeId Folder;
    // Fields
    UA_NodeId Name;
    UA_NodeId Description;
    UA_NodeId Location;
    
    UA_NodeId TagId;
    UA_NodeId Antenna;
    struct nodeVariable Temperatur;
    struct nodeVariable MaxTemperatur;
    struct nodeVariable MinTemperatur;
    // Events
    UA_NodeId TimeoutEvent;
    UA_NodeId AboveMaxTemperaturEvent;
    UA_NodeId BelowMaxTemperaturEvent;
    // Methoden
    
} ;

struct nodePin {
    UA_NodeId Folder;
    // Fields
    UA_NodeId Name;
    UA_NodeId Description;
    UA_NodeId Location;

    UA_NodeId Pin;
    UA_NodeId Mode;
   struct nodeVariable Status;
    // Events
    UA_NodeId ChangedEvent;
};

struct nodeRFIDReader {
    UA_NodeId Folder;
    // Fields
    UA_NodeId Name;
    UA_NodeId Description;
    UA_NodeId Location;
    UA_NodeId IPAdress;
    UA_NodeId Port;
    UA_NodeId SerialNumber;
    UA_NodeId Hardwarerevision;
    UA_NodeId Softwarerevision;
    struct nodePin InputPins[sLesegeraetPinInputs];
    struct nodePin OutputPins[sLesegeraetPinOutput];
    struct nodeTemperaturSensor TemperaturSensors[sLesegeraetTemperaturSensors];
    // Events
    UA_NodeId ReaderTimeout;
    // Methoden
    UA_NodeId addTemperaturSensor;
    UA_NodeId removeTemperaturSensor;
    UA_NodeId setPinState;
};

struct nodeTemperaturMonitoring {
    UA_NodeId Folder;
    // Fields
    UA_NodeId Name;
    UA_NodeId Description;
    UA_NodeId Location;
    UA_NodeId Version;
    UA_NodeId LastContact;
   struct nodeRFIDReader Reader;
    // Events
    UA_NodeId ReaderTimeoutEvent;
};



#endif

