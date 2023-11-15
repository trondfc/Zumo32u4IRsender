/*
 * Zumo32u4SendIR.ino
 * 
 * Example of how to use the Zumo32U4IRsender library to send data using the IR LED
 */
#include <Zumo32u4IRsender.h>

#define DEVICE_ID 42 // ID of the device, used to identify the robot

uint64_t lastTime = 0;

// Initialize the IR sender with the device ID and side to be used
Zumo32U4IRsender ZumoIrSender(DEVICE_ID, RIGHT_IR);

void setup(){
    Serial.begin(9600);
    delay(3000);  // wait for serial window conection, used for debuging
    Serial.println(F("START " __FILE__ " from " __DATE__ "\r\n"));

}

void loop(){
    if(millis()-lastTime > 1000){
        lastTime = millis();
        uint8_t data = random(255);
        Serial.print("Sending:");
        Serial.println(data);
        ZumoIrSender.send(data);
    }
}