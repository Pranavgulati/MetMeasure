#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

RF24 radio(7, 8);
long double calibrated=0;
#define AVERAGE 20
#define LATENCY 5
#define RED 0
#define YELLOW 5
#define GREEN 10

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
 if (radio.available())
  {
    char text[32] = {0};
    radio.read(&text, sizeof(text));
    Serial.println(text);
  }
  String temp=text;
  ledDisplay(currentState/LATENCY);
  int ldrValue=LDR();
  if(ldrValue-calibrated > temp.toInt()){
      if(currentState!=RED){currentState--;}
        }
  if(ldrValue-calibrated < temp.toInt()){
      if(currentState!=GREEN){ currentState++;}
        }
 
}
