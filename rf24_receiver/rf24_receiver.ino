#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

RF24 radio(7, 8);

// TODO, fix : blink led is quicker if remove serial.print

const byte rxAddr2[6] = "00002";
int ledW2 = 9;
int ledW1 = 3;
// true is window open, false is close
bool stateW1;
bool stateW2;

bool w1SensorInit = false;
bool w2SensorInit = false;

int isW1Down = 0;
int isW2Down = 0;

int maxCount = 150; // approximately 2min
int ledState;

void setup()
{
  while (!Serial);
  Serial.begin(9600);
  pinMode(ledW2, OUTPUT);
  pinMode(ledW1, OUTPUT);

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

    if (String(text) == "window-reu-open"){
      // check init state
      if (!w1SensorInit){
        w1SensorInit = true;
      }
      stateW1 = true;
      isW1Down = 0;
      isW2Down += 1;

    } else if (String(text) == "window-reu-close"){
      // check init state
      if (!w1SensorInit){
        w1SensorInit = true;
      }
      stateW1 = false;
      isW1Down = 0;
      isW2Down += 1;

    } else if (String(text) == "window-mezza-open"){
      // check init state
      if (!w2SensorInit){
        w2SensorInit = true;
      }
      stateW2 = true;
      isW2Down = 0;
      isW1Down += 1;

    } else if (String(text) == "window-mezza-close"){
      // check init state
      if (!w2SensorInit){
        w2SensorInit = true;
      }
      stateW2 = false;
      isW2Down = 0;
      isW1Down += 1;
    }
  } else {
      // no message
      isW1Down += 1;
      isW2Down += 1;
    }
  handleLightState(stateW1, ledW1, isW1Down);
  handleLightState(stateW2, ledW2, isW2Down);

}


void handleLightState(bool wState, int ledPin, int isWdown) {
  Serial.print("led state beggining :");
  Serial.println(ledState);
 
  Serial.print("wState :");
  Serial.println(wState);
  if (wState) {
    //blink
    if (ledState = HIGH) {
      Serial.println("entered in if (ledState = HIGH)");
      ledState = LOW;
    } else {
      Serial.println("entered in ELSE OF if (ledState = HIGH)");
      ledState = HIGH;
    }
    digitalWrite(ledPin, ledState);
    delay(500);
  }

  ledState = HIGH;
  Serial.print("w1SensorInit :");
  Serial.println(w1SensorInit);
    Serial.print("w2SensorInit :");
  Serial.println(w2SensorInit);
   // if no message from W1 since too long, turn off led
  if (maxCount <= isWdown) {
    if (ledState = HIGH) {
      ledState = LOW;
      Serial.println("maxCount reach");
    }
  }
  digitalWrite(ledPin, ledState);
}

