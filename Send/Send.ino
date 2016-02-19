/* 

   - CONNECTIONS
   - nRF24L01 Radio Module: See http://arduino-info.wikispaces.com/Nrf24L01-2.4GHz-HowTo
   
   1 - GND
   2 - VCC 3.3V !!! NOT 5V
   3 - CE to Arduino pin 7
   4 - CSN to Arduino pin 8
   5 - SCK to Arduino pin 13
   6 - MOSI to Arduino pin 11
   7 - MISO to Arduino pin 12
   8 - UNUSED
   - V2.12 02/08/2016
   - Uses the RF24 Library by TMRH20 and Maniacbug: https://github.com/TMRh20/RF24 (Download ZIP)
  

/*-----( Import needed libraries )-----*/
#include <SPI.h>   // Comes with Arduino IDE
#include "RF24.h"  // Download and Install (See above)
#include "printf.h" // Needed for "printDetails" Takes up some memory

/*-----( Declare Constants and Pin Numbers )-----*/
#define  CE_PIN  7   // The pins to be used for CE and SN
#define  CSN_PIN 8

/*-----( Declare objects )-----*/
/* Hardware configuration: Set up nRF24L01 radio on SPI bus plus (usually) pins 7 & 8 (Can be changed) */
RF24 radio(CE_PIN, CSN_PIN);

/*-----( Declare Variables )-----*/
byte addresses[][6] = {"1Node", "2Node"}; // These will be the names of the "Pipes"

unsigned long timeNow;  // Used to grab the current time, calculate delays
unsigned long started_waiting_at;
boolean timeout;       // Timeout? True or False

/**
  Create a data structure for transmitting and receiving data
  This allows many variables to be easily sent and received in a single transmission
  See http://www.cplusplus.com/doc/tutorial/structures/
*/

struct dataStruct 
{
  unsigned long _micros;   // to save response times
  String test;             // This is a test string
} myData;                 // This can be accessed in the form:  myData.Xposition  etc.


void setup()   /****** SETUP: RUNS ONCE ******/
{
  Serial.begin(115200);  // MUST reset the Serial Monitor to 115200 (lower right of window )
  // NOTE: The "F" in the print statements means "unchangable data; save in Flash Memory to conserve SRAM"
  Serial.println(F(" Send data by nRF24L01 radio to another Arduino -  Rachit"));
  printf_begin(); // Needed for "printDetails" Takes up some memory
  
  
  
  // pinMode(JOYSTICK_SW, INPUT_PULLUP);  // Pin A2 will be used as a digital input // use this 



  radio.begin();          // Initialize the nRF24L01 Radio
  radio.setChannel(108);  // Above most WiFi frequencies 
  radio.setDataRate(RF24_250KBPS); // Fast enough.. Better range
  radio.setPALevel(RF24_PA_LOW);
  // radio.setPALevel(RF24_PA_MAX);

  // Open a writing and reading pipe on each radio, with opposite addresses
  radio.openWritingPipe(addresses[0]);
  radio.openReadingPipe(1, addresses[1]);

  // Start the radio listening for data
  radio.startListening();

//  radio.printDetails(); //Uncomment to show LOTS of debugging information
}//--(end setup )---


void loop()   /****** LOOP: RUNS CONSTANTLY ******/
{
  radio.stopListening();                                    // First, stop listening so we can talk.

  myData.test = "Hello World !";  // Send some known fake data
  
  myData._micros = micros();  // Send back for timing


  Serial.print(F("Now sending  -  "));

  if (!radio.write( &myData, sizeof(myData) )) {            // Send data, checking for error ("!" means NOT)
    Serial.println(F("Transmit failed "));
  }

  radio.startListening();                                    // Now, continue listening

  started_waiting_at = micros();               // timeout period, get the current microseconds
  timeout = false;                            //  variable to indicate if a response was received or not

  while ( ! radio.available() ) {                            // While nothing is received
    if (micros() - started_waiting_at > 200000 ) {           // If waited longer than 200ms, indicate timeout and exit while loop
      timeout = true;
      break;
    }
  }

  if ( timeout )
  { // Describe the results
    Serial.println(F("Response timed out -  no Acknowledge."));
  }
  else
  {
    // Grab the response, compare, and send to Serial Monitor
    radio.read( &myData, sizeof(myData) );
    timeNow = micros();

    // Show it
    Serial.print(F("Sent "));
    Serial.print(timeNow);
    Serial.print(F(", Got response "));
    Serial.print(myData._micros);
    Serial.print(F(", Round-trip delay "));
    Serial.print(timeNow - myData._micros);
    Serial.println(F(" microseconds "));

  }

  // Send again after delay. When working OK, change to something like 100
  delay(100);

}
