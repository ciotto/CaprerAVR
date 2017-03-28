#include <DFRobotDFPlayerMini.h>

#ifndef CaprerAVR
#define CaprerAVR

// Costants
const byte BUTTONS_COUNT = 4;                              // the number of the buzz button
const byte MODES_COUNT = 4;                                // the number of the buzz button
const byte BUSY_PIN = 6;                                   // Arduino pin wired to DFR0299 16 pin

// Buttons
const byte BUTTON1 = B0001;
const byte BUTTON2 = B0010;
const byte BUTTON3 = B0100;
const byte BUTTON4 = B1000;
const byte BUTTONS[BUTTONS_COUNT] = {BUTTON1, BUTTON2, BUTTON3, BUTTON4};

// Folders
const byte BUTTON1_FOLDER = 1;
const byte BUTTON2_FOLDER = 2;
const byte BUTTON3_FOLDER = 3;
const byte BUTTON4_FOLDER = 4;
const byte MODE_FOLDER = 5;
const byte SOUND_FOLDER = 6;

// Sounds
const byte VOLUME_SOUND = 1;

// Modes
const byte MODE1 = B0000;
const byte MODE2 = B1000;      // Prevent play folder 04
const byte MODE3 = B0111;      // Play only folder 04
const byte MODE4 = B0010;      // Play random folder
const byte MODES[MODES_COUNT] = {MODE1, MODE2, MODE3, MODE4};
const byte MODE_SWITCH = BUTTON1 + BUTTON2 + BUTTON3 + BUTTON4;
const byte DEFAULT_MODE = MODE2;

// Other commands
const byte VOLUME_UP = BUTTON1 + BUTTON3 + BUTTON2;
const byte VOLUME_DOWN = BUTTON1 + BUTTON3 + BUTTON4;


const byte BUTTONS_PIN[BUTTONS_COUNT] = {2, 3, 4, 5};       // the number of the buzz buttons pin
const byte LEDS_PIN[BUTTONS_COUNT] = {8, 9, 12, 13};        // the number of the buzz buttons LED pin

// DFR0299 comunication
const boolean MP3_BUSY = false;       // DFR0299 busy pin level
const byte MP3_SERIAL_TX = 11;
const byte MP3_SERIAL_RX = 10;

// ESP8266 comunication
const byte WIFI_SERIAL_TX = 19;
const byte WIFI_SERIAL_RX = 18;

// Serial commands
const byte COMMANDS_ARGUMENT_LENGTH = 4;
const byte COMMANDS_SIZE = COMMANDS_ARGUMENT_LENGTH + BUTTONS_COUNT;

#endif

