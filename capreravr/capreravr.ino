/*
  CapreraVR

  Play a random mp3 sample in a DFR0299 microSD card slot.
  
  
  The circuit:
    * button LED attached from pin 13 to ground
    * buzz button attached to pin 2 from +5V
    * 10K resistor attached to pin 2 from ground
    * DFR0299 mp3 player attached to pin 0 and 1 (serial) and power
    * speaker attached to DFR0299 pin 6 and 8
  
  * Note: on most Arduinos there is already an LED on the board
  attached to pin 13.
  
  
  created 2016
  by Ciotto <http://ci8.it>
  
  DFPlayer library are available here:
    https://github.com/Arduinolibrary/DFRobot_Mini_Player/raw/master/DFPlayer_Mini_mp3.zip
  
    Methods:
      mp3_play ();     //start play
      mp3_play (5);    //play "mp3/0005.mp3"
      mp3_next ();     //play next 
      mp3_prev ();     //play previous
      mp3_set_volume (uint16_t volume);    //0~30
      mp3_set_EQ ();   //0~5
      mp3_pause ();
      mp3_stop ();
      void mp3_get_state ();   //send get state command
      void mp3_get_volume (); 
      void mp3_get_u_sum (); 
      void mp3_get_tf_sum (); 
      void mp3_get_flash_sum (); 
      void mp3_get_tf_current (); 
      void mp3_get_u_current (); 
      void mp3_get_flash_current (); 
      void mp3_single_loop (boolean state);    //set single loop 
      void mp3_DAC (boolean state); 
      void mp3_random_play (); 
 
 https://github.com/ciotto/CapreraVR
 */

#include <SoftwareSerial.h>
#include <DFPlayer_Mini_Mp3.h>

// costants
const int buzzPin = 2;     // the number of the buzz button pin
const int ledPin =  13;      // the number of the buzz button LED pin

// variables
int buttonState = 0;         // variable for reading the buzz button status

void setup() {
  // initialize the LED pin as an output:
  pinMode(ledPin, OUTPUT);
  // initialize the buzz button pin as an input:
  pinMode(buzzPin, INPUT);

  // Init serial
  Serial.begin(9600);

  // Set Serial for DFPlayer-mini mp3 module
  mp3_set_serial(Serial);
  // Wait 1ms
  delay(1);
  // Set volume (value 0~30)
  mp3_set_volume(15);
}

void loop() {
  // read the state of the buzz button value:
  buttonState = digitalRead(buzzPin);

  // check if the buzz button is pressed.
  // if it is, the buttonState is HIGH:
  if (buttonState == HIGH) {
    // turn LED on:
    digitalWrite(ledPin, HIGH);
  } else {
    // turn LED off:
    digitalWrite(ledPin, LOW);
  }
}
