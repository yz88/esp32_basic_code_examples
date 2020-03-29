# ESP32 deep sleep

Web-Links

- [heise developer - ESP32 to go](https://www.heise.de/developer/artikel/ESP32-to-go-4452689.html?view=print)

Required material

- ESP23 module, in this case ESP-WROOM-32U
- Arduino IDE
- micro USB cable

## deep sleep

An ESP32 board already needs a few mA when active, on average about 150 to 260 mA. In active mode, including all components such as WiFi and Bluetooth, this can even be up to 800 mA at peak loads. Within IoT applications, however, it is quite common for devices to take short-term measurements using sensors only every minute or even every hour, and then transmit them to the outside world via communication protocol. It therefore makes no sense to have an embedded controller working actively all the time. In the case of an autonomous, battery-powered device, the battery would otherwise have to be replaced every few hours. If you imagine that the device is mounted on a tree at great height, the practical dimension of this problem should be clear.

That's why the ESP32 supports several economy modes. In deep sleep mode, only the RTC (real-time clock) and the ULP (Ultra Low Power) coprocessor are active. In this case, the required residual current is just 2.5 micro Amps. This is a good compromise.

There are three options for waking up from deep sleep:

- Pressing a touch pin
- event on an external pin (= signal edge)
- time-controlled wake-up using the ULP co-processor

The following example illustrates the latter case.

However, deep sleep also leads to a kind of digital dementia because the data (= variables) are lost with one exception. All data declared with the key attribute `RTC_ATTR_DATA` is retained. After all, this persistent memory contains about four times as much data as an ordinary Arduino Uno/Mega/Nano memory in total.

In the following program, the variable `sleppCounter` is located in said memory. It simply counts how often a restart has already been performed. At the first start `(sleepCounter == 0)` the built-in LED flashes briefly. If several restarts have already taken place after deep sleep phases `(sleepCounter != 0)`, the LED flashes more frequently.

The initiation of a time-controlled deep sleep begins with the call to `esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP * uS_TO_S_FACTOR)`. `TIME_TO_SLEEP` gives the time in seconds. Since the calculation is carried out internally in microseconds, the conversion factor `uS_TO_S_FACTOR` must be multiplied by this. The actual deep sleep is initiated by the following call: `esp_deep_sleep_start()`:

```c++
#define uS_TO_S_FACTOR 1000000 // 1 second in microseconds
#define TIME_TO_SLEEP 5        // sleep duration in seconds
#define LEDPIN 23              // LED connected to GPIO 23
#define BLINK_DELAY 200        // blinking frequency = 200 / BLINK_DELAY

RTC_DATA_ATTR int sleepCounter = 0;

void setup()
{
    Serial.begin(115200);
    Serial.println("Deep Sleep via timer - Demo");
    pinMode(LEDPIN, OUTPUT); // LED

    delay(500); // wait for half a second

    if (sleepCounter == 0) // first time
    {
        Serial.println("Initial start");
        digitalWrite(LEDPIN, HIGH); // LED on
        delay(10 * BLINK_DELAY);
        digitalWrite(LEDPIN, LOW); // LED off
        sleepCounter++;
    }
    else // Waking up from sleep a few times already
    {
        Serial.println("Waking up from sleep a few times already: " + String(sleepCounter));
        flashLED(sleepCounter);
        sleepCounter++;
    }

    // In Tiefschlaf versetzen
    Serial.print("Deep sleep is initiated for " + String(TIME_TO_SLEEP));
    Serial.println(" seconds");
    esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP * uS_TO_S_FACTOR);
    esp_deep_sleep_start(); // starting deep standby
}

void flashLED(byte n)
{
    // LED is flashing n-times with a pause of  BLINK_DELAY in msec
    Serial.println("ESP is awake");
    for (int i = 0; i < n; i++)
    {
        Serial.println("LED on");
        digitalWrite(LEDPIN, HIGH);
        delay(BLINK_DELAY);
        Serial.println("LED off");
        digitalWrite(LEDPIN, LOW);
        delay(BLINK_DELAY);
    }
}

void loop()
{
    Serial.println("unused code");
}
```
