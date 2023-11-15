## What is new?
- ...


## What is coming next?
- Please let me know, if you have ideas or need improvements (open an issue)




# Jog for Estelcam

[Estelcam](https://www.estlcam.de) is a simple and powerful CNC Software with integrated CAM. You can control your homebuild CNC with Estelcam and different hardware. Especially if you use Arduino Uno or Arduino Nano, it is not possible to connect a jog wheel to the hardware. Instead you have to use the jog functions within Estelcam or keyboard/keypads.

To improve usability and get better a haptical feeling, I wanted to connect a simple joystick, rotary knobs and a linear slider directly to the PC as a simple jog.
The following functions are implemented:
- joystick for X/Y
- buttons for Z up/down
- slider for speed of the axis
- rotary knobs for feed and spindle speed

The Teensy LC is emulating the keyboard. The firmware is a simple scetch written in the Arduino IDE.


Hier BIld


# Hardware

- 1x Teensy LC or Teeny 3.2 or similar [Link](https://eu.robotshop.com/de/products/teensy-lc-usb-microcontroller-development-board)
- 2x rotary encoder KY-040 or similar [Link](https://www.amazon.de/s?k=rotary+encoder+ky-040&__mk_de_DE=ÅMÅŽÕÑ&crid=94ARQ2T8IVQT&sprefix=rotary+encoder+ky-040%2Caps%2C101&ref=nb_sb_noss_1)
- 1x potentiometer 10k.... 100k [Link](https://www.amazon.de/RoboMall-Linear-Poti-Schiebepotentiometer-2-Fach/dp/B01KZM135I/ref=sr_1_5?__mk_de_DE=ÅMÅŽÕÑ&crid=33N7B9JL6OH87&keywords=linear+poti&qid=1700078468&sprefix=linear+poti%2Caps%2C137&sr=8-5)
- 2x push button [Link](https://www.amazon.de/RUNCCI-YUN-Wasserdicht-Druckknopf-Autotrompete-Haustürklingel/dp/B08P4CC8V6/ref=sr_1_22_sspa?__mk_de_DE=ÅMÅŽÕÑ&crid=Y0GVKG4ZISSB&keywords=drucktaster&qid=1700078583&sprefix=drucktaster%2Caps%2C123&sr=8-22-spons&sp_csd=d2lkZ2V0TmFtZT1zcF9tdGY&th=1)
- 1x joystick Ky-023 [Link](https://www.amazon.de/AZDelivery-Joystick-KY-023-Arduino-gratis/dp/B07V3HQSVY/ref=sr_1_2_sspa?__mk_de_DE=ÅMÅŽÕÑ&crid=1EICNRSPPHWXX&keywords=joystick+arduino&qid=1700078535&sprefix=joystickl+arduino%2Caps%2C101&sr=8-2-spons&sp_csd=d2lkZ2V0TmFtZT1zcF9hdGY&psc=1)

# Wiring

# Firmware

- the Firmware is written as a scetch in the Arduino IDE
- more information about Teensy LC [Link](https://www.prjc.com/teensy)   
- download and update the Arduino IDE >2.0.x with Teensyduino to get hardware support and additional USB functionality [Link](https://www.pjrc.com/teensy/td_download.html)
- open Arduino IDE
- open the scetch
- select Teensy LC as board
- select Tools > USB Type > Keyboard in Arduino IDE to enable Keyboard HID
- select Tools > Keyboard Layout < Germany 
- use verify in the Arduino IDE to compile
- the Teensy Loader opens and requests a short push of the Reset button on the teensy board


# How to start
In this chapter you find some recommendations how to get started and how to verify your project.

## Connecting and testing the joystick
- connect the joystick as shown under wiring
- uncomment the #define #define _DebugJoystick in the scetch. this enables the debugging of the joystick in the serial monitor
- verify the scetch
- download the scetch with a short push of the reset button on the Teensy board
- pressing the joystick will witch on the onboard LED
- moving the joystick will send codes to the serial monitor inside the Arduino IDE (15200 baud)
- left should show 0x20 as POS
- right should show 0x10 as POS
- up should show 0x01 as POS
- down should show 0x02 as POS
- center should show 0x00 as POS
Trouble shooting: 
- if you get no messages, check the wireing
- if you don't get these values at the positions, change the following defines
- - #define Center          0x00
- - #define Left            0x20
- - #define Right           0x10     
- - #define Up              0x01 
- - #define Down            0x02




