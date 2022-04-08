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
uint8_t uid[3]= {0x12, 0x34, 0x56 };
int step = 0;


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
    //Remove "MSG " prefix
    
    inputString.remove(0);
    inputString.remove(0);
    inputString.remove(0);
    // Convert arduino string into uint8_t
    uint8_t inputStringInt = inputString.toInt();
    Serial.println(inputStringInt);
    nfc_write.setUid(&inputStringInt);
    nfc_write.init();
    nfc_write.emulate();
  }

}