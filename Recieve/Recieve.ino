#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

RF24 radio(7, 8);
long double calibrated=0;
#define AVERAGE 20
#define RED 0
#define YELLOW 1
#define GREEN 2

const byte rxAddr[6] = "00001";

void setup()
{
  while (!Serial);
  Serial.begin(9600);
  
  radio.begin();
  radio.openReadingPipe(0, rxAddr);
  
  radio.startListening();
}

void loop(){

ledDisplay(currentState);
  int ldrValue=LDR();
  if(ldrValue>calibrated){
      if(currentState!=RED){currentState--;}
        }
  if(ldrValue<calibrated){
      if(currentState!=GREEN){ currentState++;}
        }
  if (radio.available())
  {
    char text[32] = {0};
    radio.read(&text, sizeof(text));
    
    Serial.println(text);
  }
}
