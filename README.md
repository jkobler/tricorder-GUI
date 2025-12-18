# tricorder-GUI
**I'm building a fully (esque) functional TOS-era [Tricorder](http://www.racprops.com/issue5/classictricorder/index.php) using Arduino C++ and assorted compatibles.** 

## Task List
- [X] Blinky Lights™
- [X] Get TFT working
- [X] Make Menu System
- [X] Make Stardate Calculator
- [ ] Connect Working RTC
- [X] Button 1 and Button 2 do things like back and forward
- [X] Get I2C bus functioning
- [ ] Connect hood MCU to body MCU
- [X] Get Navigation Encoder Connected and Working
	- [X] ..Solder Encoder to Breakout Board
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

### Punchdown List

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



#### Display Punchdown List

| Auxilliary Device | Pin On Aux | MCU            | Pin On MCU | Wire Color |
| :---              | :---       | :---           | :---       | :---       |
| 2.2" TFT          | GND        | Metro ESP32-S2 | GND        | black      |
| 2.2" TFT          | VIN        | Metro ESP32-S2 | 3.3        | brown      |
| 2.2" TFT          | D/C        | Metro ESP32-S2 | D9         | red        |
| 2.2" TFT          | RST        | Metro ESP32-S2 | RST        | orange     |
| 2.2" TFT          | SDCS       | Metro ESP32-S2 | D12        | yellow     |
| 2.2" TFT          | CS         | Metro ESP32-S2 | D10        | green      |
| 2.2" TFT          | MOSI       | Metro ESP32-S2 | MOSI       | blue       |
| 2.2" TFT          | MISO       | Metro ESP32-S2 | MISO       | purple     |
| 2.2" TFT          | SCK        | Metro ESP32-S2 | SCK        | gray       |
| 2.2" TFT          | BL         | Metro ESP32-S2 | XXXXXXXXXX | white      |


#### Switch Array Punchdown List

| Auxilliary Device | Pin On Aux | MCU            | Pin On MCU | Wire Color |
| :---              | :---       | :---           | :---       | :---       |
| SWITCH1           |            | Metro ESP32-S2 | D7         | purple     |
| SWITCH2           |            | Metro ESP32-S2 | D8         | green      |
| GND               |            | Metro ESP32-S2 | GND        | blue       |


#### Blinky Lights™ Array Punchdown List

| Auxilliary Device | Pin On Aux | MCU            | Pin On MCU | Wire Color |
| :---              | :---       | :---           | :---       | :---       |
| LED1              |            | Metro ESP32-S2 | D13        | purple     |
| LED2              |            | Metro ESP32-S2 | D14        | green      |
| LED3              |            | Metro ESP32-S2 | D15        | blue       |
| GND               |            | Metro ESP32-S2 | GND        | black      |



## 2025-12-17

Had to take break to print parts, order parts, and work on other projects, but I was able to hook up the blinky lights™ on a [Adafruit GPIO Extender](https://www.adafruit.com/product/4886) and the encoder with an [Adafruit Rotary Encoder Breakout](https://www.adafruit.com/product/4991) to the [Adafruit ESP32-S2 Feather](https://www.adafruit.com/product/5000). That works so much nicer. The lights can fade in and out which is classy af.

I almost forgot to mention I bought I new 3D-printer which is what brought me back into this full berries. My Creality Ender 3 S1 Plus has unfortunately ended up with a warped hot bed, I imagine because it is rather wide and long, but also perhaps because I didn't let it cool slower. Just theories, but I can't find a replacement hotbed for it, and since we have the laser cutter attachment which isn't so sensitive to tram and level issues, it will be for laser cutting from here on out. I replaced it with an Anycubic Kobra S1 combo kit with the ACE filament box which allows me to do multicolor. So far it has been wonderful. I mean, I was printing right out of the box which I couldn't do with the Creality.

While the Ender 3 was I struggle sometimes, I don't regret getting it. I learned a lot, and when it worked, it worked well. For the price point, it worked well. I will still be able to use it as mentioned, for laser cutting. I would still be using it if Creality hadn't abandon making replacement parts for it a year after I bought it, but such are things with bleeding edge technology I guess.

Nonetheless, I can now print my tricorder parts after making changes necessary for fitting the parts.

Ok, back to the Tricorder.

I also picked up a couple of [Adafruit 2.0" 320x240 TFT Displays](https://www.adafruit.com/product/4311) from Digikey which will provide better fitment. The goal is to get one working with the ESP32-S2 Feather along with everything else so the buttons and encoder can interact with the menu system. I am going to ditch the EYESPI board. It's great for debugging and prototyping but room is a factor and the ribbon cable doesn't fit so I will have to direct solder, maybe with a different connector between for easy replacement. Adafruit has more of the 2.0" TFTs back in stock so I'll probably order more from there. It's nice to have spares, not to mention like all projects this is merely the prototype. Actually, the next 5 will mostly be prototypes too if I'm being honest here.

I printed a handy test stand for my hood parts (STL is included in this repo) that will assist me in prototype and development.

I'm going to have to change to the buttons inputting to and the LEDs fed from the MCU directly. There just isn't going to be room in the hood for the GPIO Extender, not to mention it really didn't spare me any hassle as I still had to solder. I intend on using pulse width modulation to get the same fade in and out effect though.

For switches I found these awesome badboys on DigiKey, [Pushbutton Switch SPST-NO](https://www.digikey.com/en/products/detail/e-switch/800AWSP9M2QE/502069). Not tactile but they fit well. I re-make the train wheel buttons to fit over these. I'll probably pull them down to ground with 10KΩ resisters.

At this point I need to decide how I want to connect the blinky lights™, switches, and TFT to the MCU, and find connectors that will work for this setup.  


### Punchdown List (expected)

| Auxilliary Device | Pin On Aux | MCU              | Pin On MCU | Pull Up/Down  | Wire Color |
| :---              | :---       | :---             | :---       | :---          | :---       |
| 2.0" TFT          | Vin        | ESP32-S2 Feather | 3.3        |               |            |
| 2.0" TFT          | 3Vo        |                  |            |               |            |
| 2.0" TFT          | Gnd        | ESP32-S2 Feather | GND        |               |            |
| 2.0" TFT          | SCK        | ESP32-S2 Feather | SCK        |               |            |
| 2.0" TFT          | MISO       | ESP32-S2 Feather | MI         |               |            |
| 2.0" TFT          | MOSI       | ESP32-S2 Feather | MO         |               |            |
| 2.0" TFT          | CS         | ESP32-S2 Feather | D13        |               |            |
| 2.0" TFT          | RST        | ESP32-S2 Feather | D12        |               |            |
| 2.0" TFT          | D/C        | ESP32-S2 Feather | D11        |               |            |
| 2.0" TFT          | SDSC       | ESP32-S2 Feather | D10        |               |            |
| 2.0" TFT          | BL         | ESP32-S2 Feather |            |               |            |
| SWITCH1           | 1          | ESP32-S2 Feather | D09        | 10kΩ to GND   |            |
| SWITCH1           | 2          | ESP32-S2 Feather | 3.3V       |               |            |
| SWITCH2           | 1          | ESP32-S2 Feather | D06        | 10kΩ to GND   |            |
| SWITCH2           | 2          | ESP32-S2 Feather | 3.3V       |               |            |
| SWITCH3           | 1          | ESP32-S2 Feather | D05        | 10kΩ to GND   |            |
| SWITCH3           | 2          | ESP32-S2 Feather | 3.3V       |               |            |
| BlinkyLight1      | Anode      | ESP32-S2 Feather | A05        |               |            | 
| BlinkyLight1      | Cathode    | ESP32-S2 Feather | GND        | 10kΩ to GND   |            |  
| BlinkyLight2      | Anode      | ESP32-S2 Feather | A04        |               |            | 
| BlinkyLight2      | Cathode    | ESP32-S2 Feather | GND        | 10kΩ to GND   |            |  
| BlinkyLight3      | Anode      | ESP32-S2 Feather | A03        |               |            | 
| BlinkyLight3      | Cathode    | ESP32-S2 Feather | GND        | 10kΩ to GND   |            |  



#### I2C Bus

| Auxilliary Device | Address    | MCU              | Constant Name | 
| :---              | :---       | :---             | :---          | 
| Encoder Breakout  | 0x36       | ESP32-S2 Feather | SEESAW_ADDR   | 


