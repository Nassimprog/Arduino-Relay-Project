#include <SoftwareSerial.h>

#include <PN532_SWHSU.h>

#include <PN532.h>



SoftwareSerial SWSerial( 10, 11 ); // RX, TX

PN532_SWHSU pn532swhsu( SWSerial );

PN532 nfc( pn532swhsu );

// serial event vars
String inputString = "MSG";         // a String to hold incoming data, has MSG prefix so it can be sent across Serial
bool stringComplete = false;
int step = 0;


void setup(void) {

  Serial.begin(9600);
  //LocalLink.begin(9600);

  inputString.reserve(200); // reserve 200 bytes for the inputString

  Serial.println("MOLE ACTIVATED");

  nfc.begin();

  uint32_t versiondata = nfc.getFirmwareVersion();

  if(!versiondata)
  {
    Serial.print("Didn't Find PN53x Module");
    while (1); // Halt
  }

  // Got valid data, print it out!

  Serial.print("Found chip PN5"); Serial.println((versiondata>>24) & 0xFF, HEX);

  Serial.print("Firmware ver. "); Serial.print((versiondata>>16) & 0xFF, DEC);

  Serial.print('.'); Serial.println((versiondata>>8) & 0xFF, DEC);

  // Configure board to read RFID tags

  nfc.SAMConfig();

  
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);
}



void loop() {

  boolean success;

  uint8_t uid[] = { 0, 0, 0, 0, 0, 0, 0 };  // Buffer to store the returned UID

  uint8_t uidLength;                       // Length of the UID (4 or 7 bytes depending on ISO14443A card type)

  
  if(step == 0)
  {
    Serial.println("DBG Mole is Listening ...");
    step = 1;
  }
  
  // When request is recieved, 1st step
  if(step == 1)
  {
    if (Serial.available())
    {
      Serial.println("Request Found");
      

      // Read NFC TAG
      Serial.println("Waiting for an ISO14443A Card ...");

      success = nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, &uid[0], &uidLength);
      if(success)
      {
        Serial.println("Found A Card!");

        Serial.print("UID Length: "); Serial.print(uidLength, DEC); Serial.println(" bytes");
        Serial.print("UID Value: ");

        for(uint8_t i = 0 ; i < uidLength ; ++i)
        {
          // Serial.print(" 0x");
          // Serial.print(uid[i], HEX); // in hex
        
          //inputString += " 0x";
          inputString += uid[i];
        }
        


        // should serial write UID
        //Serial.write(inputString.c_str());
        Serial.println(inputString); // this sends as it contains MSG prefix
        while(1); //halt
        
      }

      else 
      {
        // PN532 timed out
        Serial.println("Timed out! Waiting for a card...");
      }
    }
  }

}