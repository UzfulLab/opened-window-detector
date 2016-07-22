#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

RF24 radio(7, 8);

const byte rxAddr2[6] = "00002";
int ledSalleReu = 6;
int ledMezza = 2;

void setup()
{
  while (!Serial);
  Serial.begin(9600);
  pinMode(ledSalleReu, OUTPUT);
  pinMode(ledMezza, OUTPUT);
  
  radio.begin();
  radio.openReadingPipe(0, rxAddr2);
  
  radio.startListening();
}

void loop()
{
  
  if (radio.available())
  {
    Serial.println("radio available");
    char text[32] = {0};
    radio.read(&text, sizeof(text));
    
    Serial.println(text);
    if (String(text) == "window-mezza-open"){
      // blink for 5s
      for(int i = 0; i<5; i++){
        digitalWrite(ledMezza, HIGH);
        delay(500);
        digitalWrite(ledMezza, LOW);
        delay(500);
      }
    }
    if (String(text) == "window-mezza-close"){
      digitalWrite(ledMezza, HIGH);
    }
    if (String(text) == "window-reu-open"){
      // blink for 5s
      for(int i = 0; i<5; i++){
        digitalWrite(ledSalleReu, HIGH);
        delay(500);
        digitalWrite(ledSalleReu, LOW);
        delay(500);
      }
    }
    if (String(text) == "window-reu-close"){
      digitalWrite(ledSalleReu, HIGH);
    }
    int mezzaUp = String(text).indexOf("mezza");
    int reuUp = String(text).indexOf("reu");
    if(mezzaUp != -1){
      digitalWrite(ledMezza, HIGH);
    } else {
      digitalWrite(ledMezza, LOW);
    }
    if(reuUp != -1){
      digitalWrite(ledSalleReu, HIGH);
    } else {
      digitalWrite(ledSalleReu, LOW);
    }
  }
}
