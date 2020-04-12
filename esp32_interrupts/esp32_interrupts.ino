/*
ESP-WROOM-32U
Chip is ESP32D0WDQ6 (revision 1)
Features: WiFi, BT, Dual Core, 240MHz, VRef calibration in efuse, Coding Scheme None
MAC: 24:0a:c4:c5:4e:c0
*/

struct Button
{
    const uint8_t PIN;
    uint32_t numberKeyPresses;
    bool pressed;
};

Button button1 = {32, 0, false};

void IRAM_ATTR isr()
{
    button1.numberKeyPresses += 1;
    button1.pressed = true;
}

void setup()
{
    Serial.begin(115200);
    Serial.printf("Setting up parameters.\n");
    Serial.printf("Button PIN: %u\n", button1.PIN);
    pinMode(button1.PIN, INPUT_PULLUP);
    attachInterrupt(button1.PIN, isr, FALLING);
}

void loop()
{
    if (button1.pressed)
    {
        Serial.printf("Button 1 has been pressed %u times\n", button1.numberKeyPresses);
        button1.pressed = false;
    }

    // Detach Interrupt after 1 Minute
    // check the number of milliseconds that have passed since the program first started using millis() function
    static uint32_t lastMillis = 0;
    if (millis() - lastMillis > 60000)
    {
        lastMillis = millis();
        detachInterrupt(button1.PIN);
        Serial.println("Interrupt Detached!");
    }
}
