/*
* Zumo32u4AnswerIR.ino
* Example sketch for Zumo32U4IRreceiver library.
*
* Example showing how to use the Zumo32U4IRreceiver library and 
* the IRemote library to receive and answer IR signals on the Zumo32U4.
*/
#define DECODE_NEC

#include <IRremote.hpp>
#include "Zumo32u4IRsender.h"

#define YELLOW_LED_PIN 13

#define IR_RECEIVE_PIN 22 //Front proximity sensor pin on the Zumo32U4

#define DEVICE_ID 25  // ID of the device, used to identify the robot#define COMMAND_IDENTIFY 0xF0
#define COMMAND_IDENTIFY 0xF0 // Command for telling the robot to identify itself
#define COMMAND_IDENTIFYING 0x02 // Command for identifying itself

#define ANSWER_DELAY 30 // Timeout for answering a command, used as sender may need some time if it has picked up its own signal

const int authorisedDevices[] = {200, 201}; // List of authorised devices

// Initialize the IR sender with the device ID and side to be used
Zumo32U4IRsender ZumoIrSender(DEVICE_ID, RIGHT_IR);

unsigned long recivedTime = 0;
bool newData = false;

void setup() {
    Serial.begin(9600);
    delay(3000);
  
    pinMode(YELLOW_LED_PIN, OUTPUT);
  
    Serial.println(F("START " __FILE__ " from " __DATE__ "\r\n"));

    IrReceiver.begin(IR_RECEIVE_PIN);

    Serial.print(F("Ready to receive IR signals of protocols: "));
    printActiveIRProtocols(&Serial);
    Serial.print(F("at pin "));
    Serial.println(IR_RECEIVE_PIN);
}

void loop() {

    if (IrReceiver.decode()) {
        if (IrReceiver.decodedIRData.protocol == UNKNOWN) {
            Serial.println(F("Received noise or an unknown (or not yet enabled) protocol"));
            // We have an unknown protocol here, print more info
            IrReceiver.printIRResultRawFormatted(&Serial, true);
        } else {
            Serial.println();
            Serial.print("Recived command ");
            Serial.print(IrReceiver.decodedIRData.command);
            Serial.print(" from ");
            Serial.println(IrReceiver.decodedIRData.address);

            recivedTime = millis();
            newData = true;
        }
    IrReceiver.start();  // Enable receiving of the next value
    }
    if (millis() - recivedTime > ANSWER_DELAY && newData == true) {
        handleCommand(IrReceiver.decodedIRData.address, IrReceiver.decodedIRData.command);
        newData = false;
        IrReceiver.start();  // Enable receiving of the next value
    }
}

/**
 * @brief Function for checking if the sender is authorised
 * 
 * @param senderID ID of the sender
 * @return true if the sender ID is in the list of authorised devices
 * @return false if not 
 */
bool isAuthorised(uint16_t senderID) {
    for (int i = 0; i < (sizeof(authorisedDevices) / sizeof(authorisedDevices[0])); i++) {
        if (senderID == authorisedDevices[i]) {
        Serial.println("Is authorised");
        return true;
        }
    }
    Serial.println("Not authorised");
    return false;
}

/**
 * @brief Function for handling commands
 * 
 * @param senderID Id of the sender
 * @param command Command to be handled
 */
void handleCommand(uint16_t senderID, uint16_t command) {
    if (isAuthorised(senderID)) {
        if (command == COMMAND_IDENTIFY) {
            Serial.println("Identify");
            ZumoIrSender.send(COMMAND_IDENTIFYING);
        } else {
            Serial.println("Unknown command");
        }
    }
}
