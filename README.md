# tricorder-GUI
**I'm building a fully (esque) functional TOS-era [Tricorder](http://www.racprops.com/issue5/classictricorder/index.php) using Arduino C++ and assorted compatibles.** 

## Task List
- [X] Blinky Lights™
- [X] Get TFT working
- [X] Make Menu System
- [X] Make Stardate Calculator
- [ ] Connect Working RTC
- [X] Button 1 and Button 2 do things like back and forward
- [ ] Get I2C bus functioning
- [ ] Connect hood MCU to body MCU
- [ ] Get Navigation Encoder Connected and Working
	- [ ] ..Solder Encoder to Breakout Board
- [ ] Move components from breadboard to functional connects
- [ ] Solder switchs and Blink Lights™ to perf board then to hood MCU GPIO pins, or GPIO extender
- [ ] Solder TFT to hood MCU SPI pins
- [ ] Route power connections
- [ ] Get battery system working with low charge warning and charge status
- [ ] Print 3D parts.
- [ ] Standby mode
- [ ] Auto shutoff
- [ ] screen brightness
- [ ] audio
	- [ ] microphone and recording
	- [ ] speaker and audio playback
	- [ ] volume, gain, timbre(?)
- [ ] video
	- [ ] camera(s) and recording
	- [ ] video playback
- [ ] infrared camera and monitoring
- [ ] all the sensors
- [ ] Write the rest of this list
- [ ] Initiate scope creep.


## 2025-11-16
It all started when I started 3D printing a [Tricorder](https://cults3d.com/en/3d-model/various/tos-tricorder-with-empty-compartment) model and then thought to myself, "Hey, I should put a little screen along with an MCU and then make it do tricorder stuff." It kind of went from there. 

So far, I have a working first go at a GUI, which I had to take liberties with as there were very few times we ever saw the little screen on the hero models. I tried to keep a look and feel that I thought might be privelent in the mid-23rd century, and not go full LCARS with it. Personally, I don't like LCARS but that's me. I went with something a bit more minimalistic and easy to read on the small 320x240 sceen. Long, indented losenges.

I have stubs for some of the features. I did get a basic image veiwer working as well as a text reader. I also have blinky lights™. You can't have a tricorder without the blinky lights™, now can you? The answer is 'no' as if that question needs an answer. ALWAYS BLINKY LIGHTS™!

As of right now I plan on sticking with ESP32 processors as they seem to have the speed and memory to handle everything without issue. I currently have an [Adafruit Metro ESP32-S2](https://www.adafruit.com/product/2488) as the processor now, which is running the screen and the blinky lights™. I plan on using a smaller [Adafruit ESP32-S2 Feather](https://www.adafruit.com/product/5000) for the [Adafruit 2.2" ILI9341 based TFT](https://www.adafruit.com/product/1480), the blinky lights™ of course, and the buttons. All of these should fit up in the hood along with the screen, and require only power and I2C connections routed from the body. There will also probably be a microphone in the hood but I would prefer it be connected to the body processor which will most likely be the Adafruit Metro ESP32-S2 I already have in play. 


###Current Punchdown List

| Auxilliary Device | Pin On Aux | MCU            | Pin On MCU | Wire Color |
| :---              | :---       | :---           | :---       | :---       |
| 2.2" TFT          | FPC Conn   | EYESPI         | FPC Conn   |            |
| EYESPI            | Vin        | Metro ESP32-S2 | 3.3        |            |
| EYESPI            | Gnd        | Metro ESP32-S2 | GND        |            |
| EYESPI            | SCK        | Metro ESP32-S2 | SCK        | blue       |
| EYESPI            | MOSI       | Metro ESP32-S2 | MOSI       | orange     |
| EYESPI            | MISO       | Metro ESP32-S2 |            |            |
| EYESPI            | DC         | Metro ESP32-S2 | D9         | yellow     |
| EYESPI            | RST        | Metro ESP32-S2 | RST        | brown      |
| EYESPI            | TCS        | Metro ESP32-S2 | D10        | green      |
| EYESPI            | SDSC       | Metro ESP32-S2 | D12        | gray       |
| SWITCH1           | 1          | Metro ESP32-S2 | D7         | purple     |
| SWITCH1           | 2          | Metro ESP32-S2 | GND        | black      |
| SWITCH2           | 1          | Metro ESP32-S2 | D8         | yellow     |
| SWITCH2           | 2          | Metro ESP32-S2 | GND        | black      |
| SWITCH3           | 1          | Metro ESP32-S2 | D6         | orange     |
| SWITCH3           | 2          | Metro ESP32-S2 | GND        | black      |
| SWITCH4           | 1          | Metro ESP32-S2 | D5         | blue       |
| SWITCH5           | 2          | Metro ESP32-S2 | GND        | black      |
| BlinkyLight1      | Anode      | Metro ESP32-S2 | D13        | red        | 
| BlinkyLight1      | Cathode    | Metro ESP32-S2 | GND        | 10kΩ       |  
| BlinkyLight2      | Anode      | Metro ESP32-S2 | D14        | white      | 
| BlinkyLight2      | Cathode    | Metro ESP32-S2 | GND        | 10kΩ       |  
| BlinkyLight3      | Anode      | Metro ESP32-S2 | D15        | brown      | 
| BlinkyLight3      | Cathode    | Metro ESP32-S2 | GND        | 10kΩ       |  
|                   |            |                |            |  
