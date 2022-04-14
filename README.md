# Arduino-Relay-Project
Final year project simulating a relay attack scenario using Arduinos that emulate mobile devices.

Pre-requisites:
3 Arduino UNO devices
3 PN532 NFC Modules installed into Arduino UNO

Connect the three Arduinos into the machine running the relay.
Upload the proxy, mole and reader files into each Arduino, make a note of the COM for each
Configure the Proxy and Mole with the correct COM number ie. COM9 is proxy, etc.
Ensure the NFC Tag is placed on the mole before the code is run
with all devices connected, run serialRelay.py
The proxy will output a time to record
open the serial monitor for the reader
use the proxy as an NFC card and interract with the reader
Reader outputs the time
