#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

RF24 radio(7, 8);

const byte rxAddr[6] = "00001";
long double calibrated=0;
#define AVERAGE 20
#define RED 0
#define YELLOW 1
#define GREEN 2
byte currentState=1;
void setup()
{Serial.begin(9600);
  radio.begin();
  radio.setRetries(15, 15);
  radio.openWritingPipe(rxAddr); 
  radio.stopListening();
  for(byte i=0;i<AVERAGE;i++){
    calibrated+=LDR();  
  }
  calibrated/=AVERAGE;
}
void ledDisplay(byte colour){}
int LDR(){
return 256;}

void loop()
{
  ledDisplay(currentState);
  int ldrValue=LDR();
  if(ldrValue>calibrated){
      if(currentState!=RED){currentState--;}
        }
  if(ldrValue<calibrated){
      if(currentState!=GREEN){ currentState++;}
        }      
   char text[] = "";
  itoa(ldrValue,text,10);
  Serial.println(text);
  radio.write(&text, sizeof(text));
  
  delay(1000);
}
