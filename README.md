# CaprerAVR

![CaprerAVR](images/capreravr.jpg)

**CaprerAVR** is a crazy project for office survival, when you are in difficult, push the buzz button and **CaprerAVR** helps you!!

![Dai dai dai](http://www.serialminds.com/wp-content/uploads/2016/03/dai-dai-dai.gif)

### Breadboar Schematic

![CaprerAVR](fritzing/capreravr_bb.png)


## Info

**CaprerAVR** is made using a **DFR0299** module drived by **Arduino** using the **[DFPlayer-Mini-mp3 library](https://github.com/DFRobot/DFPlayer-Mini-mp3/)** (if some functions are missing, try using [my version](https://github.com/ciotto/DFPlayer-Mini-mp3/))


## What you need

 - *1 x* [Arduino or compatible](http://www.aliexpress.com/item/UNO-R3-CH340G-ATmega328P-compatible-for-Arduino-UNO-R3/32523366108.html?spm=2114.01010208.3.132.ps5asV&ws_ab_test=searchweb201556_8,searchweb201602_5_10037_10017_405_507_10032,searchweb201603_11&btsid=4d84a66e-7b4c-4053-977e-ca0880699bd8)
 - *1 x* [DFR0299](http://it.aliexpress.com/item/1PCS-Mini-MP3-Player-Module-with-Simplified-Output-Speaker-for-Arduino-UNO/32271693515.html)
 - *4 x* [Buzz Button](http://it.aliexpress.com/item/1PC-LED-Light-60MM-Big-Round-Arcade-Video-Game-Player-Push-Button-Switch-Red-Plunger-Size/32645265359.html)
 - *1 x* microSD card
 - *2 x* 1k resistors
 - *4 x* 10k resistor
 - *4 x* ~100nF capacitator
 - *1 x* speaker
 - *some* mp3 samples

## ESP8266

It's possible to add networking capabilities to this awesome device thank to the ``ESP8266`` module.

Follow instruction from this [repository](https://github.com/esp8266/Arduino) to enable the board on the Arduino IDE.

In order to flash the firmware you need to set in bootloader mode the module: connect the ``GPIO0`` pin to ``GND``
and re-power. For normal operation instead connect ``GPIO0`` to ``VCC``.

At the ``arduino/WifiWebServer/`` path you can find a proof of concept for a web service that acts on the GPIO pins.

From the serial console you can observe this output

```
Connecting to wifi-essid
..
WiFi connected
Server started
Use this URL to connect: http://192.168.1.11/
```
 
## Links

 - [DFR0299 datasheet 1](http://www.picaxe.com/docs/spe033.pdf)
 - [DFR0299 datasheet 2](http://www.trainelectronics.com/Arduino/MP3Sound/TalkingTemperature/FN-M16P%20Embedded%20MP3%20Audio%20Module%20Datasheet.pdf)
 - [DFR0299 doc](http://www.dfrobot.com/wiki/index.php/DFPlayer_Mini_SKU:DFR0299)
 - [DFPlayer-Mini-mp3 library](https://github.com/DFRobot/DFPlayer-Mini-mp3/archive/master.zip)
 - [DFPlayer-Mini-mp3 library (my version)](https://github.com/ciotto/DFPlayer-Mini-mp3/archive/master.zip)
