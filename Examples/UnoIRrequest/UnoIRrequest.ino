/*
* UnoIRrequest.ino
* Example sketch for Zumo32U4IRreceiver library.
*
* Program to send IR commands to a device and receive the answer
* Run on an Arduino Uno with an IR LED connected to pin 3 and an IR receiver connected to pin 2
* Used for "talking" to the Zumo32u4AnswerIR program
*/
#define DECODE_NEC  // We want to decode NEC protocol

#define IR_RECEIVE_PIN 2 
#define IR_SEND_PIN 3 // Must be on a PWM pin

#include <IRremote.hpp>

#define DEVICE_ID 200  // ID of the device, used to identify the sender

#define COMMAND_IDENTIFY 0xF0     // Command for telling the robot to identify itself
#define COMMAND_TOL_STATION 0xF1  // Command for telling the robot it's at a tol station

#define COMMAND_OPEN 0x01  // Command for telling the tol station to open the gate

#define ANSER_TIMEOUT 1000  // Time between transmissions

bool awaitingAnswer = false;
unsigned long lastTransmission = 0;

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);

  Serial.begin(9600);
  delay(3000);

  // Just to know which program is running on my Arduino
  Serial.println(F("START " __FILE__ " from " __DATE__ "\r\nUsing library version " VERSION_IRREMOTE));

  Serial.print(F("Send IR signals at pin "));
  Serial.println(IR_SEND_PIN);
  Serial.print(F("Ready to receive IR signals of protocols: "));
  printActiveIRProtocols(&Serial);
  Serial.print(F("at pin "));
  Serial.println(IR_RECEIVE_PIN);

  IrReceiver.begin(IR_RECEIVE_PIN);
  IrSender.begin(DISABLE_LED_FEEDBACK);
}

void loop() {
  unsigned long timeNow = millis();
  if (timeNow - lastTransmission > ANSER_TIMEOUT) {
    Serial.println();
    Serial.print(F("Send now: address=0x"));
    Serial.print(DEVICE_ID, HEX);
    Serial.print(F(", command=0x"));
    Serial.print(COMMAND_IDENTIFY, HEX);
    Serial.println();

    Serial.println(F("Send standard NEC with 8 bit address"));
    Serial.flush();
    IrSender.sendNEC(DEVICE_ID, COMMAND_IDENTIFY, 0);
    lastTransmission = timeNow;
    awaitingAnswer = true;
  }
  if (IrReceiver.decode()) {

    IrReceiver.printIRResultShort(&Serial);


    if (IrReceiver.decodedIRData.protocol == UNKNOWN) {
      Serial.println(F("Received noise or an unknown (or not yet enabled) protocol"));
      // We have an unknown protocol here, print more info
      IrReceiver.printIRResultRawFormatted(&Serial, true);
    }
    Serial.println();
    IrReceiver.resume();  // Enable receiving of the next value

    if (awaitingAnswer && IrReceiver.decodedIRData.address != DEVICE_ID) {
      Serial.println("Got answer");
      Serial.print("The car ID is:");
      Serial.println(IrReceiver.decodedIRData.address);

      Serial.print("Recived command is:");
      Serial.println(IrReceiver.decodedIRData.command);
      awaitingAnswer = false;
    }
  }
}