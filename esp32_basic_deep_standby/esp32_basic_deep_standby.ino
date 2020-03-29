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