#include <Arduino.h>
#include <SoftwareSerial.h>
#include <PN532_SWHSU.h>

#include <PN532.h>


#include <emulatetag.h>
#include <NdefMessage.h> // doesnt exist?



SoftwareSerial SWSerial( 10, 11 ); // RX, TX

PN532_SWHSU pn532swhsu( SWSerial );

PN532 nfc_read( pn532swhsu );

EmulateTag nfc_write(pn532swhsu);

// serial event vars
String inputString = "";         // a String to hold incoming data
NdefMessage message;
uint8_t uid[] = { 0, 0, 0, 0, 0, 0, 0 };
int step = 0;
int interractionTime = 0;


void setup()
{

  Serial.begin(9600);
  //LocalLink.begin(9600);

// reserve 200 bytes for the inputString:
  inputString.reserve(200);

  Serial.println("Hello Maker!");

  nfc_read.begin();

  uint32_t versiondata = nfc_read.getFirmwareVersion();

  if (! versiondata) 
  {

    Serial.print("Didn't Find PN53x Module");

    while (1); // Halt

  }

  // Got valid data, print it out!

  Serial.print("Found chip PN5"); Serial.println((versiondata>>24) & 0xFF, HEX);

  Serial.print("Firmware ver. "); Serial.print((versiondata>>16) & 0xFF, DEC);

  Serial.print('.'); Serial.println((versiondata>>8) & 0xFF, DEC);




  // Configure board to emulate nfc tag

  //uint8_t uidInt = uid.toInt();
  Serial.println("PROXY ACTIVATED");
  
  //TODO Make set UID, it is currently 000000000 but setting the UID does not change the value
  nfc_write.setUid(uid);
  nfc_write.init();
}

void loop() 
{
  int timerStart = millis();
  
  //Serial.println("DBG:  Running Loop");
  //Serial.println("MSG Message to Mole");
  //Serial.println("DBG:  Done");
  //delay(5000);
if(step == 0)
{
  
  Serial.println("MSG Message to Mole"); 
  Serial.println("Proxy is Listening ...");
  step = 1;
}
  

  if(step == 1)
  {
    if(Serial.available())
    {
      Serial.println("Response found!");
      inputString = Serial.readString();
      step = 2;
    }
  }
  
  

  
  // Emulates as an NFC TAG with relayed UID Value
  if (step == 2) 
  {
    //Remove "MSG" prefix
    
    
    Serial.println(inputString);
    inputString.remove(0);
    inputString.remove(0);
    inputString.remove(0);
    // Convert arduino string into uint8_t

    //8124793
    //uint8_t inputStringInt = inputString.toInt();
    uint8_t uid[3] = { 0x7B, 0xF9, 0x79 };
    // uid[0] = (uint8_t)(inputStringInt >> 24);
    // uid[1] = (uint8_t)(inputStringInt >> 16);
    // uid[2] = (uint8_t)(inputStringInt >> 8);
    //uid[3] = (uint8_t)inputStringInt;
    
    
    
    // uint8_t inputStringInt = 8124793;
    // //uint8_t* ptr = &inputStringInt;
    // uint8_t* ptr = reinterpret_cast<uint8_t*>(8124793);
    // Serial.println(*ptr);
    nfc_write.setUid(uid); // 80097 is memory address, it should be 8124793 value initialise a pointer and set the value of the pointer to the UID Value.   
    interractionTime = (millis() - timerStart);
    Serial.println("Time of interraction: ");
    Serial.println(interractionTime);
    nfc_write.init();
    nfc_write.emulate();

    
  }

}