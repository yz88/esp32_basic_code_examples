# ESP32 and threads

Web-Links

- [heise developer - ESP32 to go](https://www.heise.de/developer/artikel/ESP32-to-go-4452689.html?view=print)

Required material

- ESP23 module, in this case ESP-WROOM-32U
- Arduino IDE
- micro USB cable

Because ESP32 has a multi-core architecture, truly parallel threads or tasks can be used. The following program uses this possibility as an example to create tasks using xTaskCreatePinnedToCore() and bind them to one of the two cores. The first task taskOne runs on the first kernel 0, the second taskTwo on the second kernel 1. Both tasks have priority 1 and receive 10,000 bytes stack size.

taskOne makes the LED connect to GPIO 23 flash every half second, while taskTwo outputs text on the serial monitor every half second.

The method loop() remains unused in this case because the event loop takes place in the endlessly running tasks anyway.

```C
// each task needs a task handler
TaskHandle_t task1;
TaskHandle_t task2;

// LED Pin, use GPIO numbering
const int ledPIN = 23;

void setup()
{
  Serial.begin(115200);
  pinMode(ledPIN, OUTPUT);

  // taskOne() on first core (Core 0), Prio: 1
  xTaskCreatePinnedToCore(
      taskOne,   // task function -> taskOne()
      "TaskOne", // name of the task
      10000,     // stack size in words
      NULL,      // task input parameter
      1,         // priority
      &task1,    // task handler
      0);        // task should use fisrt core (core 0)
  delay(500);

  // taskTwo() on second Core (Core 1), Prio: 1
  xTaskCreatePinnedToCore(
      taskTwo,   // task function -> taskTow()
      "TaskTwo", // name of the task
      10000,     // stack size in words
      NULL,      // task input parameter
      1,         // priority
      &task2,    // task handler
      1);        // task should use second core (core 1)
  delay(500);
}

// taskOne: LED should blink
void taskOne(void *optionalArgs)
{
  for (;;)
  {
    digitalWrite(ledPIN, HIGH);
    delay(500);
    digitalWrite(ledPIN, LOW);
    delay(500);
  }
}

// taskTwo: serial output every 100 msec
void taskTwo(void *optionalArgs)
{
  Serial.print("Task 2 runs on core ");
  Serial.println(xPortGetCoreID());
  for (;;)
  {
    Serial.println("LED on");
    delay(500);
    Serial.println("LED off");
    delay(500);
  }
}
void loop()
{
  // not needed
}
```