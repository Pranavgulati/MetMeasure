#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

RF24 radio(7, 8);

#define LDRpin 14
#define greenLed 5
#define redLed  3
#define AVERAGE 12
#define LATENCY 20
#define RED 0
#define YELLOW 20
#define GREEN 49

const byte rxAddr[6] = "00001";
long  calibrated=0;
byte currentState=1;
void setup()
{ Serial.begin(9600);
  Serial.flush();
  radio.begin();
  radio.setRetries(15, 15);
  radio.openWritingPipe(rxAddr); 
  radio.stopListening();
  for(byte i=0;i<AVERAGE;i++){
    calibrated+=LDR();  
  }
  calibrated/=AVERAGE;
}
void ledDisplay(int x){
  
  if (x == 0) {
    analogWrite(redLed, 255);
    analogWrite(greenLed, 0);
  }
  else if (x == 1) {
    analogWrite(redLed, 250);
    analogWrite(greenLed,5 );
  }
else if (x == 2) {
  analogWrite(redLed, 0);
  analogWrite(greenLed, 255);
}

}

long int LDR(void)
{
  long int avg;
  long int  sum = 0;
  for (byte i = 0; i < AVERAGE; i++)
  { int sensorValue = analogRead(LDRpin);
    sum += sensorValue;
  }
  avg = sum / AVERAGE;
  return avg;
}


void loop()
{
  ledDisplay(currentState/LATENCY);
  int ldrValue=LDR();
  if(ldrValue>calibrated+10){
      if(currentState!=RED){currentState--;}
        }
  if(ldrValue<calibrated+10){
      if(currentState!=GREEN){ currentState++;}
        }      
   char text[4];
  Serial.print(calibrated);Serial.print("  ");Serial.println(ldrValue);
  ldrValue-=calibrated;
  itoa(ldrValue,text,10);
  Serial.print("curs");Serial.println(currentState);Serial.println(text);
  radio.write(&text, sizeof(text));
  delay(5);
  
}
