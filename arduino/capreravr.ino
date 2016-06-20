/*
  CaprerAVR

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
 
 https://github.com/ciotto/CaprerAVR
 */

#include <SoftwareSerial.h>
#include <DFRobot_utility.h>
#include <DFPlayer_Mini_Mp3.h>

// costants
const int buzzPin = 2;       // the number of the buzz button pin
const int ledPin =  13;      // the number of the buzz button LED pin

// variables
int buttonState = 0;         // variable for reading the buzz button status
int tracks = 0;              // tracks in microSD

int read_int () {
        delay (100);
        
        long recv_leng = serialRead(Serial1, recv_buf, 10, 3);
        if (recv_leng) {
                int res = 0;
                res = res * 256 + (unsigned char)recv_buf[4];
                res = res * 256 + (unsigned char)recv_buf[5];
                res = res * 256 + (unsigned char)recv_buf[6];

                return res;
        }
        return -1;
}

void setup() {
  // initialize the LED pin as an output:
  pinMode(ledPin, OUTPUT);
  // initialize the buzz button pin as an input:
  pinMode(buzzPin, INPUT);

  // Init serial
  Serial1.begin (9600);
  Serial.begin (9600);
  while (!Serial);

  // Set Serial for DFPlayer-mini mp3 module
  mp3_set_serial(Serial1);
  // Wait 1ms
  delay(1);
  // Set volume (value 0~30)
  mp3_set_volume(27);
  // Set device to microSD
  mp3_set_device(2);
  // Query the total number of microSD card files
  mp3_get_tf_sum();
  tracks = read_int();

  Serial.print('Find ');
  Serial.print(tracks);
  Serial.println(' tracks.');
}

void loop() {
  // read the state of the buzz button value:
  buttonState = digitalRead(buzzPin);

  // check if the buzz button is pressed.
  // if it is, the buttonState is HIGH (antibounce hardware):
  if (buttonState == HIGH) {
    // turn LED on:
    digitalWrite(ledPin, HIGH);

    mp3_play(random(1, tracks + 1));
  } else {
    // turn LED off:
    digitalWrite(ledPin, LOW);
  }
}
