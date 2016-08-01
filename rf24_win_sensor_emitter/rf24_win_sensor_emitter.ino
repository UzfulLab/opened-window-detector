#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <Bounce2.h>
#define BUTTON_PIN  3  // Arduino Digital I/O pin for button/reed switch

Bounce debouncer = Bounce();

RF24 radio(7, 8);

const byte rxAddr[6] = "00002";

void setup()
{
  while (!Serial);
  Serial.begin(9600);
  // Setup the button
  pinMode(BUTTON_PIN,INPUT);
  // Activate internal pull-up
  digitalWrite(BUTTON_PIN,HIGH);
  // After setting up the button, setup debouncer
  debouncer.attach(BUTTON_PIN);
  debouncer.interval(5);
  // Start radio
  radio.begin();
  radio.setRetries(15, 15);
  // open pipe on declared adress
  radio.openWritingPipe(rxAddr);
  radio.stopListening();
  
}

void loop()
{
  debouncer.update();
  // Get the update value
  int value = debouncer.read();

  // Send every 5s the state of the module
  value = value==HIGH ? 1 : 0;
  if (value == 1){
    // create and send message for open state
    const char msgOpen[] = "window-1-open";
    radio.write(&msgOpen, sizeof(msgOpen));
  } else {
    // create and send message for close state
    const char msgClose[] = "window-1-close";
    radio.write(&msgClose, sizeof(msgClose));
  }  
  // wait 5s
  delay(5000);
}
