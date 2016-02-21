#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

RF24 radio(7, 8);

#define AVERAGE 2
#define LATENCY 20
#define RED 0
#define YELLOW 20
#define GREEN 40
#define LDRpin 14
#define greenLed 5
#define redLed  3

byte currentState = 1;
long int calibrated = 0;

const byte rxAddr[6] = "00001";

void ledDisplay(int x)
{
  
  if (x == 0) {
    analogWrite(redLed, 255);
    analogWrite(greenLed, 0);
  }
  else if (x == 1) {
    analogWrite(redLed, 250);
    analogWrite(greenLed, 5);
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

void setup()
{
  while (!Serial);
  Serial.begin(9600);
    
  radio.begin();
  radio.openReadingPipe(0, rxAddr);
  for(byte i=0;i<AVERAGE;i++){
    calibrated+=LDR();  
  }
  calibrated/=AVERAGE;
  radio.startListening();
}


 void loop() 
 {

  char text[32] = {0};
  
  if(radio.available())
  {
    radio.read(&text, sizeof(text));
    Serial.println(text);
  }
  
  String temp = text;
  ledDisplay(currentState / LATENCY);
  int ldrValue = LDR();
  if (ldrValue - calibrated > temp.toInt()) {
    if (currentState != RED) {
      currentState--;
    }
  }
  if (ldrValue - calibrated < temp.toInt()) {
    if (currentState != GREEN) {
      currentState++;
    }
  }
  Serial.print(calibrated);Serial.print("  ");Serial.println(ldrValue);
//Serial.print("recv");Serial.print(temp.toInt());
  Serial.print("curs");Serial.println(currentState);

}
