#include <SoftwareSerial.h>

#include <PN532_SWHSU.h>

#include <PN532.h>



SoftwareSerial SWSerial( 10, 11 ); // RX, TX

PN532_SWHSU pn532swhsu( SWSerial );

PN532 nfc( pn532swhsu );

// serial event vars
String inputString = "";         // a String to hold incoming data
bool stringComplete = false;
int interractionTime = 0;


void setup(void) {

  

  Serial.begin(9600);

  inputString.reserve(200); // reserve 200 bytes for the inputString

  Serial.println("READER ACTIVATED");

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

  Serial.println("Waiting for an ISO14443A Card ...");
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);

  
}

void loop() {

  boolean success;
  boolean relay;

  uint8_t uid[] = { 0, 0, 0, 0, 0, 0, 0 };  // Buffer to store the returned UID

  uint8_t uidLength;                       // Length of the UID (4 or 7 bytes depending on ISO14443A card type)

  success = nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, &uid[0], &uidLength);

  // When a card is detected: request UID
  if(success)
  {

    Serial.println("Card Found! Sending UID CALL REQUEST");
    //start timer
    int timerStart = millis();   //  gets time since program has run
    // // Serial Write request for UID
    // inputString = "UID Request";
    // Serial.write(inputString.c_str());


    // Read data recieved
    
    
      // read uid
      for(uint8_t i = 0 ; i < uidLength ; ++i)
    {
      // Serial.print(" 0x");
      // Serial.print(uid[i], HEX); // in hex
    
      inputString += " 0x";
      inputString += uid[i];

      Serial.print("UID Length: "); Serial.print(uidLength, DEC); Serial.println(" bytes");
      Serial.print("UID Value: ");
      Serial.write(inputString.c_str());
      
    }
    


    
    if(inputString == " 0x102 0x57 0x63 0x212Tim") // if Input string matches value, then it passes the checks
    {
      Serial.print("Pass");

      // interractionTime = (millis() - timerStart);
      // Serial.print("");
      // Serial.print("Time of interraction: ");
      // Serial.print(interractionTime);
      
    }
    inputString = "";

    delay(10000);

    //while(1) // halt
  }

  else 
  {
    // PN532 timed out
    Serial.println("Timed out! Waiting for a card...");
   }
}



