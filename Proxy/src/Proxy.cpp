

#include <SoftwareSerial.h>

#include <PN532_SWHSU.h>

#include <PN532.h>



SoftwareSerial SWSerial( 10, 11 ); // RX, TX

PN532_SWHSU pn532swhsu( SWSerial );

PN532 nfc( pn532swhsu );

// serial event vars
String inputString = "";         // a String to hold incoming data

void setup()
{

  Serial.begin(9600);

// reserve 200 bytes for the inputString:
  inputString.reserve(200);

  Serial.println("Hello Maker!");

  nfc.begin();

  uint32_t versiondata = nfc.getFirmwareVersion();

  if (! versiondata) {

    Serial.print("Didn't Find PN53x Module");

    while (1); // Halt

  }

  // Got valid data, print it out!

  Serial.print("Found chip PN5"); Serial.println((versiondata>>24) & 0xFF, HEX);

  Serial.print("Firmware ver. "); Serial.print((versiondata>>16) & 0xFF, DEC);

  Serial.print('.'); Serial.println((versiondata>>8) & 0xFF, DEC);

  // Configure board to read RFID tags

  nfc.SAMConfig();

  Serial.println("PROXY ACTIVATED: Waiting for an ISO14443A Card ...");

}

void loop() 
{
  while (Serial.available() > 0 )
  {
    inputString = Serial.read(); // returns SINGLE byte (char)
  }
  //delay(20000);
    

// print the string when a newline arrives: (serialEvent)
  if (inputString.endsWith("\n")) // where arg is arduino string
  {
    Serial.println("SERIAL EVENT HAS FOUND STRING");
    Serial.println(inputString);
    
    // send (write) nfc - find function for this
    inputString = ""; // clear the string as weve processed it
  }


}