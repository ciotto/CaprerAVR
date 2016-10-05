/*
  CaprerAVR

  Play a random mp3 sample in a DFR0299 microSD card slot.
  
  
  The circuit:
    * button LED attached from pin 13, 8, 9, 10 to ground
    * buzz buttons attached to pin 2, 3, 4, 5 from +5V
    * 10K resistors attached to pin 2, 3, 4, 5 from ground
    * DFR0299 mp3 player attached to pin 0, 1 (serial), 6 (busy) and power
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
#include <DFPlayer_Mini_Mp3.h>

// costants
const int BUTTONS_COUNT = 4;                              // the number of the buzz button
const int BUSY_PIN = 6;                                    // Arduino pin wired to DFR0299 16 pin
const int buttonsPin[BUTTONS_COUNT] = {2, 3, 4, 5};       // the number of the buzz buttons pin
const int ledsPin[BUTTONS_COUNT] = {8, 13, 9, 12};        // the number of the buzz buttons LED pin

// Buttons
const int BUTTON1 = B0001;
const int BUTTON2 = B0010;
const int BUTTON3 = B0100;
const int BUTTON4 = B1000;
const int BUTTONS[BUTTONS_COUNT] = {BUTTON1, BUTTON2, BUTTON3, BUTTON4};

// Folders
const int BUTTON1_FOLDER = 1;
const int BUTTON2_FOLDER = 2;
const int BUTTON3_FOLDER = 3;
const int BUTTON4_FOLDER = 4;
const int MODE_FOLDER = 5;
const int SOUND_FOLDER = 6;

// Sounds
const int VOLUME_SOUND = 1;

// Modes
const int MODE0 = B0000;
const int MODE1 = B1000;      // Prevent play folder 04
const int MODE2 = B0111;      // Play only folder 04
const int MODE3 = B0010;      // Play random folder 04

const boolean BUSY = 0;       // DFR0299 busy pin level

SoftwareSerial mp3Serial(10, 11);

// variables
int buttonsState = 0;         // variable for reading the buzz button status
int ledsState = 0;         // variable for storing the LEDs status
int tracks[BUTTONS_COUNT] = {0, 0, 0, 0};              // tracks in microSD
int volume = 22;              // the volume level
int mode;             // the box mode
boolean busyState = !BUSY;

// http://stackoverflow.com/questions/109023/how-to-count-the-number-of-set-bits-in-a-32-bit-integer
int number_of_set_bits(int state) {
     state = state - ((state >> 1) & 0x55555555);
     state = (state & 0x33333333) + ((state >> 2) & 0x33333333);
     return (((state + (state >> 4)) & 0x0F0F0F0F) * 0x01010101) >> 24;
}

void play_folder (int folder) {
  int track = random(1, tracks[folder - 1] + 1);
  
  Serial.print("Play track ");
  Serial.print(track);
  Serial.print(" in folder ");
  Serial.println(folder);
  
  mp3_play_file_in_folder(folder, track);
  
  // set LEDs state
  set_leds(BUTTONS[folder - 1]);
}

void stop () {
  Serial.println("Stop");
      
  // stop
  mp3_stop();

  // set LEDs state
  set_leds(0);
}

void set_mode (int m) {
  mode = m;
  
  // Play mode sound
  mp3_play_file_in_folder(MODE_FOLDER, mode);
  
  Serial.print("Select mode ");
  Serial.println(mode, BIN);
}

void set_volume(int v) {
  Serial.print("Set volume to ");
  Serial.println(v);
  
  // Set volume (value 0~30)
  mp3_set_volume(v);
}

int set_leds (int state) {
  if (state == 0) {
    state = mode;
  }

  if (ledsState != state) {
    ledsState = state;
    
    for (int i=0; i < BUTTONS_COUNT; i++) {
      // turn LEDs on if related button pressed:
      digitalWrite(ledsPin[i], (state & (1 << i)) >> i);
    }
  }
}

void setup() {
  // Init serial
  mp3Serial.begin (9600);
  Serial.begin (9600);
  while (!Serial);

  // Set Serial for DFPlayer-mini mp3 module
  mp3_set_serial(mp3Serial);
  // Set logging Serial
  mp3_set_debug_serial(Serial);
  // Set volume (value 0~30)
  mp3_set_volume(volume);
  // Set device to microSD
  mp3_set_device(2);
  
  for (int i=0; i < BUTTONS_COUNT; i++) {
    // initialize the LED pin as an output:
    pinMode(ledsPin[i], OUTPUT);
    // initialize the buzz button pin as an input:
    pinMode(buttonsPin[i], INPUT);
    
    // Query the total number of microSD card files
    mp3_get_folder_sum(i + 1);
    tracks[i] = mp3_wait_folder_sum();

    Serial.print("Find ");
    Serial.print(tracks[i]);
    Serial.print(" tracks in folder 0");
    Serial.print(i + 1);
    Serial.println(".");
  }

  // Set default mode
  set_mode(MODE1);

  // Blink LEDs
  delay(100);
  set_leds(B1111);
  delay(500);
  set_leds(B0000);
}

void loop() {
  // Read busy state
  boolean newBusyState = digitalRead(BUSY_PIN);

  // Read buttons state
  buttonsState = 0;
  for (int i=0; i < BUTTONS_COUNT; i++) {
    // read the state of the buzz button value:
    int state = digitalRead(buttonsPin[i]);
    buttonsState = buttonsState | (state << i);
  }
  int pressedButtons = number_of_set_bits(buttonsState);

  // Check if the a buzz buttons is pressed
  if (pressedButtons == 1) { 
    if (busyState == 1) {
      // MODE1 prevent play folder 4
      if (buttonsState == BUTTON4 && mode == MODE1) {
        buttonsState = BUTTON3;

      // MODE2 play only folder 4
      }else if (pressedButtons == 1 && mode == MODE2) {
        buttonsState = BUTTON4;

      // MODE3 play random folder
      }else if (pressedButtons == 1 && mode == MODE3) {
        buttonsState = BUTTONS[random(0, BUTTONS_COUNT)];
      }

      // Button 1 pressed
      //   X  O
      //   O  O
      if (buttonsState == BUTTON1) {
        play_folder(1);
        
      // Button 2 pressed
      //   O  X
      //   O  O
      } else if (buttonsState == BUTTON2) {
        play_folder(2);
        
      // Button 3 pressed
      //   O  O
      //   X  O
      } else if (buttonsState == BUTTON3) {
        play_folder(3);

      // Button 4 pressed
      //   O  O
      //   O  X
      } else {
        play_folder(4);
      }
    
    // Stop if playing
    } else {
      stop();
    }

    delay(200);
    
  // More that one button pressed (special controls)
  } else if (pressedButtons > 1) {
    // Volume UP
    //   X  X
    //   X  O
    if (buttonsState == BUTTON1 + BUTTON3 + BUTTON2) {
      if (volume < 30) {
        volume++;
      }
    
      set_volume(volume);
      if (newBusyState == !BUSY) {
        mp3_play_file_in_folder(SOUND_FOLDER, VOLUME_SOUND);
      }

    // Volume DOWN
    //   X  O
    //   X  X
    } else if (buttonsState == BUTTON1 + BUTTON3 + BUTTON4) {
      if (volume > 0) {
        volume--;
      }
    
      set_volume(volume);
      if (newBusyState == !BUSY) {
        mp3_play_file_in_folder(SOUND_FOLDER, VOLUME_SOUND);
      }

    // Change mode
    //   X  X
    //   X  X
    } else if (buttonsState == BUTTON1 + BUTTON2 + BUTTON3 + BUTTON4) {
      int m = MODE0;
      if (mode == MODE0) {
        m = MODE1;
      } else if (mode == MODE1) {
        m = MODE2;
      } else if (mode == MODE2) {
        m = MODE3;
    } else if (currentState == MODE_SWITCH && newBusyState == !MP3_BUSY) {
      }
      set_mode(m);
    }

    delay(500);
  }

  // Check if DFR0299 busy state change to not busy
  if (newBusyState == !BUSY && busyState != newBusyState) {
    set_leds(0);
  }
  busyState = newBusyState;

  delay(100);
}
