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
const int BUTTON1 = B0001;
const int BUTTON2 = B0010;
const int BUTTON3 = B0100;
const int BUTTON4 = B1000;

const int buttonsCount = 4;                               // the number of the buzz button
const int busyPin = 6;                                    // Arduino pin wired to DFR0299 16 pin
const int buttonsPin[buttonsCount] = {2, 3, 4, 5};        // the number of the buzz buttons pin
const int ledsPin[buttonsCount] = {13, 8, 9, 10};       // the number of the buzz buttons LED pin

// variables
int buttonsState = 0;         // variable for reading the buzz button status
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

int set_leds (int state) {
    for (int i=0; i < buttonsCount; i++) {
      // turn LEDs on if related button pressed:
      digitalWrite(ledsPin[i], (state & (1 << i)) >> i);
    }
}

void setup() {
  // Init serial
  Serial1.begin (9600);
  Serial.begin (9600);
  while (!Serial);

  // Set Serial for DFPlayer-mini mp3 module
  mp3_set_serial(Serial1);
  // Wait 1ms
  delay(1);
  // Set volume (value 0~30)
  mp3_set_volume(20);
  // Set device to microSD
  mp3_set_device(2);
  
  for (int i=0; i < buttonsCount; i++) {
    // initialize the LED pin as an output:
    pinMode(ledsPin[i], OUTPUT);
    // initialize the buzz button pin as an input:
    pinMode(buttonsPin[i], INPUT);
  }
    
  // Query the total number of microSD card files
  mp3_get_tf_sum();
  tracks = read_int();
  Serial.print("Find ");
  Serial.print(tracks);
  Serial.println(" tracks.");

  // Blink LEDs
  set_leds(B1111);
  delay(100);
  set_leds(B0000);
}

void loop() {
  int busyState = digitalRead(busyPin);
  
  buttonsState = 0;
  for (int i=0; i < buttonsCount; i++) {
    // read the state of the buzz button value:
    int state = digitalRead(buttonsPin[i]);
    buttonsState = buttonsState | (state << i);
  }

  // check if the a buzz buttons is pressed and DFR0299 is not buzy.
  if (buttonsState > 0 && busyState == 1) {
    Serial.println("Play");
    
    // turn LED on for button pressed:
    for (int i=0; i < buttonsCount; i++) {
      // turn LEDs on if related button pressed:
      digitalWrite(ledsPin[i], (buttonsState & (1 << i)) >> i);
    }

    if (buttonsState == BUTTON1) {
      // Button 1 pressed
    } else if (buttonsState == BUTTON2) {
      // Button 2 pressed
    } else if (buttonsState == BUTTON3) {
      // Button 3 pressed
    } else if (buttonsState == BUTTON4) {
      // Button 4 pressed
    } else if (buttonsState == BUTTON1 + BUTTON2 + BUTTON3 + BUTTON4) {
      // All buttons pressed
    } else {
      // other
    }

    // play
    mp3_play(random(1, tracks + 1));
    set_leds(buttonsState);

    delay(200);
  
  // check if the a buzz buttons is pressed and DFR0299 is buzy.
  } else if (buttonsState > 0) {
    Serial.println("Stop");
    
    // stop
    mp3_stop();
    set_leds(0);

    delay(200);

  // check if the a buzz buttons is not pressed and DFR0299 is buzy.
  } else if (busyState == 1) {
    Serial.println("Off");
    
    set_leds(0);
  }

  delay(100);
}
