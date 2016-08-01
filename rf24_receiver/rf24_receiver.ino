#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

RF24 radio(7, 8);

const byte rxAddr2[6] = "00002";
int ledW2 = 9;
int ledW1 = 3;
// true is window open, false is close
bool stateW1;
bool stateW2;

//bool for sensor's init
bool w1SensorInit = false;
bool w2SensorInit = false;

// counter to monitor if sensor is down
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

    // you can change "window-1-open" with name of your window, but don't forget to change it in your sensor's code too !
    if (String(text) == "window-1-open"){
      // check init state
      if (!w1SensorInit){
        w1SensorInit = true;
      }
      stateW1 = true;
      isW1Down = 0;
      isW2Down += 1;

    } else if (String(text) == "window-1-close"){
      // check init state
      if (!w1SensorInit){
        w1SensorInit = true;
      }
      stateW1 = false;
      isW1Down = 0;
      isW2Down += 1;

    } else if (String(text) == "window-2-open"){
      // check init state
      if (!w2SensorInit){
        w2SensorInit = true;
      }
      stateW2 = true;
      isW2Down = 0;
      isW1Down += 1;

    } else if (String(text) == "window-2-close"){
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
  handleLightState(w1SensorInit, stateW1, ledW1, isW1Down);
  handleLightState(w2SensorInit, stateW2, ledW2, isW2Down);

}


void handleLightState(bool sensorInit, bool wState, int ledPin, int isWdown) {
  if (wState) {
    //blink
    if (ledState = HIGH) {
      ledState = LOW;
    } else {
      ledState = HIGH;
    }
    digitalWrite(ledPin, ledState);
    delay(500);
  }
  ledState = HIGH;
   // if no message from W1 since too long, turn off led
  if (maxCount <= isWdown) {
    if (ledState = HIGH) {
      ledState = LOW;
    }
  }
  if(!sensorInit){
    ledState = LOW;
  }

  digitalWrite(ledPin, ledState);
  delay(30);
}
