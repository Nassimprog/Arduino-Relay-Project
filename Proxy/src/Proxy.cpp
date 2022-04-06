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


void setup()
{

  Serial.begin(9600);

// reserve 200 bytes for the inputString:
  inputString.reserve(200);

  Serial.println("Hello Maker!");

  nfc_read.begin();

  uint32_t versiondata = nfc_read.getFirmwareVersion();

  if (! versiondata) {

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
  Serial.println("uid: ");
  //Serial.println(uid);
  //TODO Make set UID, it is currently 000000000 but setting the UID does not change the value
  nfc_write.setUid(uid);
  nfc_write.init();
}

void loop() 
{

  //recieves 
  while (Serial.available() > 0 )
  {
    inputString = Serial.read(); // returns SINGLE byte (char)
  }
  //delay(20000);
    

  // when proxy recieves request (serialEvent)
  if (inputString.equals("UID Request")) // where arg is arduino string
  {
    Serial.println("SERIAL EVENT HAS FOUND STRING");
    Serial.println(inputString);

    // send request to mole to read card
    inputString = "GET_UID" ;
    Serial.write(inputString.c_str());


  
  }

  if (inputString.equals("INSERT_UID_HEREHARDCODE")) // where arg is arduino string
  {
     // Convert arduino string into uint8_t

    Serial.println("SERIAL EVENT HAS FOUND STRING");
    Serial.println(inputString.c_str());
    
    uint8_t inputStringInt = inputString.toInt();
    nfc_write.setUid(&inputStringInt);
    nfc_write.init();
    nfc_write.emulate();
    //inputString = ""; // clear the string as it is processed it
  }


  //nfc_write.emulate();

}