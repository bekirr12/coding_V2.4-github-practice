#include "src/Twin/Twin.h"
#include <SoftwareSerial.h>

// Set up a new SoftwareSerial object
SoftwareSerial *mySerial ;

#define _Bluetooth  Serial1
#define _USB        Serial

#define analog      1
#define digital     2

const char outputpins[] = { 6, 9, 10};

// Giden
// _USB.write(0x95);

// Gelen
// _USB.write(0x98);
      

Channel_st  channel_st;

BOOL        flagBluetooth_b         = false;
BOOL        flagUSB_b               = false;
UINT8       flagGoingByte_u8[2]     = {0};

UINT8       bufferGoingByte_u8[20]  = {0};
UINT8       current_read_status_u8  = sync1_get;
BOOL        flagAnalyzeByte_b       = false;
INT32       temp_i32                = 0;

UINT8       bleNameIsCorrect        = 0;
UINT8       trialCountForName       = 0;
String      bleName;
UINT8       bleNameIsTwin           = 0;

//Unique id variables
String deviceId = "";
String bluetoothState = "Disconnected";      
BOOL isMacQueried = false;  
BOOL isLedSet = false;
BOOL ledTurnOff = false;
BOOL isNotificationsEnabled = false;


// Double dc motor variables
UINT8 motor_1_dir = 0;
UINT8 motor_2_dir = 0;
UINT8 motor_1_speed = 0;
UINT8 motor_2_speed = 0;

void setup() {
  _Bluetooth.begin(9600);
  _USB.begin(9600);   
  initTwin();
}

void loop() {
    if (!isMacQueried) {
        deviceId = generateIdFromMac();
        if (deviceId != "") {
            Serial.println("Device ID generated: " + deviceId);
            isMacQueried = true;  // Mark MAC query as complete
        } else {
            Serial.println("Failed to generate Device ID. Exit loop!!!");
            return;
        }
    }

    handleBluetoothState();
    readMessage();
    applyCommands();
    writeMessage();
}
