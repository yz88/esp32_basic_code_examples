# ESP32 - Getting started with ESP32 using Arduino IDE

Web-Links

- [c't Make: ESP32 SPECIAL 2019 page 6](https://shop.heise.de/katalog/make-esp32-special)
- [ESP32-WROOM-32U datasheet](https://www.espressif.com/sites/default/files/documentation/esp32-wroom-32d_esp32-wroom-32u_datasheet_en.pdf)
- [Installation instructions using Arduino IDE Boards Manager](https://github.com/espressif/arduino-esp32/blob/master/docs/arduino-ide/boards_manager.md)

Required material

- ESP23 module, in this case ESP-WROOM-32U
- Arduino IDE
- micro USB cable
  
## ESP-WROOM-32U pinout

![ESP-WROOM-32U pinout](https://github.com/yz88/esp32_basic_blink/blob/master/fritzing/esp32-DevKitC-pinout.png)

## Circuit and program code

![Fitzing Circuit](https://github.com/yz88/esp32_basic_blink/blob/master/fritzing/esp32_basic_blink_Steckplatine.png)

```cpp
const uint8_t LED_PIN = 23;

void setup() {
pinMode (LED_PIN, OUTPUT);
}

void loop() {
digitalWrite(LED_PIN, HIGH);
delay(1000);
digitalWrite(LED_PIN, LOW);
delay(1000);
}
```

## Preparing the Arduino IDE

There are diiferent ways to prepare the Arduino IDE to support ESP32 boards. One of the simplest way is described below:

- download and install Arduino IDE from <https://www.arduino.cc/en/Main/Software>
- start Arduino IDE
- under `File > Preferences >> Settings >> Additional Boards Manager URLs` add `https://dl.espressif.com/dl/package_esp32_index.json` and click `ÒK`
- install esp32 board package via `Tools > Board: "xxxx" > Boards Manager ...`
- in the new window search for `esp32 by Espressif` and install package and `close` the windows
- navigate to `Tools > Boards` and select `ESP32 Dev Module`

The Arduino IDE is prepared to work with ESP32!

## Programming ESP32 with Arduino IDE

- Connect the ESP32 board to ythe our computer through the micro-USB cable. Make sure the red LED goes high on the module to ensure power supply.
- Open the Windows device manager and check to which COM port your ESP32 is connected to
- Go back to Arduino IDE and under `Tools > Port` select the Port to which the ESP is connected to. Once selected verify this on the bottom left corner of the IDE.
- now upload the code to the ESP
