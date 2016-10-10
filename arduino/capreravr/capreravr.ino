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
#include "capreravr.h"
#include "CapreraUtilities.h"

SoftwareSerial mp3Serial(MP3_SERIAL_RX, MP3_SERIAL_TX);
SoftwareSerial wiFiSerial(WIFI_SERIAL_RX, WIFI_SERIAL_TX);

// variables
byte currentState = 0;                       // variable for storing the command/state
byte currentParameters = 0;                  // variable for storing extra data
boolean currentBusyState = !MP3_BUSY;
byte ledsState = 0;                          // variable for storing the LEDs status
byte tracks[BUTTONS_COUNT] = {0, 0, 0, 0};   // tracks in microSD
byte volume = 22;                            // the volume level
byte mode;                                   // the box mode

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
      digitalWrite(LEDS_PIN[i], (state & (1 << i)) >> i);
    }
  }
}

void setup() {
  // Init serial
  Serial.begin (9600);
  while (!Serial);

  mp3Serial.begin (9600);
  wiFiSerial.begin (9600);

  Serial.println("Starting CaprerAVR...\n");

  mp3Serial.listen();
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
    pinMode(LEDS_PIN[i], OUTPUT);
    // initialize the buzz button pin as an input:
    pinMode(BUTTONS_PIN[i], INPUT);
    
    // Query the total number of microSD card files
    mp3_get_folder_sum(i + 1);
    tracks[i] = mp3_wait_folder_sum();

    Serial.print("Find ");
    Serial.print(tracks[i]);
    Serial.print(" tracks in folder 0");
    Serial.print(i + 1);
    Serial.println(".");
  }
  wiFiSerial.listen();

  // Set default mode
  set_mode(DEFAULT_MODE);

  // Blink LEDs
  delay(100);
  set_leds(B1111);
  delay(500);
  set_leds(B0000);
}

void loop() {
  // Read busy state
  boolean newBusyState = digitalRead(BUSY_PIN);

  // Read serial command
  byte serialCommand = 0;
  byte serialParameters = 0;
  char buffer[9] = {'0', '0', '0', '0', '0', '0', '0', '0', '\0'};

  if (Serial.available() >= COMMANDS_SIZE) {
    // USB serial
    for (byte i=0; i < COMMANDS_SIZE; i++) {
      // read the incoming byte:
      buffer[i] = Serial.read();
    }

    Serial.print("Serial command received: ");
    Serial.println(buffer);
  }else if (wiFiSerial.available() >= COMMANDS_SIZE) {
    // WiFI serial
    for (byte i=0; i < COMMANDS_SIZE; i++) {
      // read the incoming byte:
      buffer[i] = wiFiSerial.read();
    }

    Serial.print("WiFi command received: ");
    Serial.println(buffer);
  } 

  serialCommand = bitString2Int(buffer, COMMANDS_SIZE + 1);
  serialParameters = serialCommand >> 4;
  serialCommand &= B00001111;

  // Read buttons state
  byte buttonsState = 0;
  for (byte i=0; i < BUTTONS_COUNT; i++) {
    // read the state of the buzz button
    byte buttonState = digitalRead(BUTTONS_PIN[i]);
    buttonsState |= (buttonState << i);
  }

  if (buttonsState) {
    currentState = buttonsState;
  } else {
    currentParameters = serialParameters;
    currentState = serialCommand;
  }

  byte bitsCount = number_of_set_bits(currentState);

  // Check if the a buzz buttons is pressed
  if (bitsCount == 1) { 
    if (newBusyState == !MP3_BUSY) {
      // MODE2 prevent play folder 4
      if (currentState == BUTTON4 && mode == MODE2) {
        currentState = BUTTON3;

      // MODE3 play only folder 4
      }else if (bitsCount == 1 && mode == MODE3) {
        currentState = BUTTON4;

      // MODE4 play random folder
      }else if (bitsCount == 1 && mode == MODE4) {
        currentState = BUTTONS[random(0, BUTTONS_COUNT)];
      }

      // Button 1 pressed
      //   X  O
      //   O  O
      if (currentState == BUTTON1) {
        play_folder(1);
        
      // Button 2 pressed
      //   O  X
      //   O  O
      } else if (currentState == BUTTON2) {
        play_folder(2);
        
      // Button 3 pressed
      //   O  O
      //   X  O
      } else if (currentState == BUTTON3) {
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
  } else if (bitsCount > 1) {
    // Volume UP
    //   X  X
    //   X  O
    if (currentState == VOLUME_UP) {
      if (volume < 30) {
        volume++;
      }
    
      set_volume(volume);
      if (newBusyState == !MP3_BUSY) {
        mp3_play_file_in_folder(SOUND_FOLDER, VOLUME_SOUND);
      }

    // Volume DOWN
    //   X  O
    //   X  X
    } else if (currentState == VOLUME_DOWN) {
      if (volume > 0) {
        volume--;
      }
    
      set_volume(volume);
      if (newBusyState == !MP3_BUSY) {
        mp3_play_file_in_folder(SOUND_FOLDER, VOLUME_SOUND);
      }

    // Change mode
    //   X  X
    //   X  X
    } else if (currentState == MODE_SWITCH && newBusyState == !MP3_BUSY) {
      byte m = MODE1;
      if (currentParameters && currentParameters <= MODES_COUNT) {
        m = MODES[currentParameters - 1];
      } else {
        if (mode == MODE1) {
          m = MODE2;
        } else if (mode == MODE2) {
          m = MODE3;
        } else if (mode == MODE3) {
          m = MODE4;
        }
      }
      set_mode(m);
    }

    delay(500);
  }

  // Check if DFR0299 busy state change to not busy
  if (newBusyState == !MP3_BUSY && currentBusyState != newBusyState) {
    set_leds(0);
  }
  currentBusyState = newBusyState;

  delay(100);
}
