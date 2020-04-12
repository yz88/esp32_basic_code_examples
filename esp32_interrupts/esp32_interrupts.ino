/*
ESP-WROOM-32U
Chip is ESP32D0WDQ6 (revision 1)
Features: WiFi, BT, Dual Core, 240MHz, VRef calibration in efuse, Coding Scheme None
MAC: 24:0a:c4:c5:4e:c0
*/

#define LED_GPIO_PIN GPIO_NUM_23

bool led_state = 0;

void led_toggle()
{
    // set GPIO value to the opposite of the current value
    digitalWrite(LED_GPIO_PIN, !digitalRead(LED_GPIO_PIN));
}

struct Button
{
    const uint8_t PIN;
    uint32_t numberKeyPresses;
    bool pressed;
};

Button button_1 = {32, 0, false};

void IRAM_ATTR button_isr()
{
    volatile static unsigned long last_interrupt_time = 0;
    unsigned long interrupt_time = millis();
    // software debouncing
    if (interrupt_time - last_interrupt_time > 200UL) // ignores interupts for 200 milliseconds, UL stands for unsigned long
    {
        last_interrupt_time = interrupt_time;
        button_1.numberKeyPresses += 1;
        button_1.pressed = true;
        led_toggle();
    }
}

void setup()
{
    Serial.begin(115200);
    Serial.printf("Setting up ....\n");
    Serial.printf("Button PIN: %u\n", button_1.PIN);
    pinMode(button_1.PIN, INPUT_PULLUP);
    attachInterrupt(button_1.PIN, button_isr, FALLING);
    // init LED GPIO
    Serial.printf("LED PIN: %u\n", LED_GPIO_PIN);
    pinMode(LED_GPIO_PIN, OUTPUT);
    digitalWrite(LED_GPIO_PIN, 0); // instead of 0 you can also use LOW
}

void loop()
{
    if (button_1.pressed)
    {
        Serial.printf("Button 1 has been pressed %u times\n", button_1.numberKeyPresses);
        button_1.pressed = false;
    }

    // Detach Interrupt after 1 Minute
    // check the number of milliseconds that have passed since the program first started using millis() function
    static uint32_t lastMillis = 0;
    if (millis() - lastMillis > 60000)
    {
        lastMillis = millis();
        detachInterrupt(button_1.PIN);
        Serial.println("Interrupt Detached!");
    }

    // the following statements are used to show that loop is still working 
    // when button is pressed and hold, remove if you want to check software debouncing
    Serial.printf("void loop() is still doing something!\n");
    delay(2000);
}
