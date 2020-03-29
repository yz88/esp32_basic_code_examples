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
  for (;;) // 'for' loop without initialization. Therefore, for (;;) is a kind of infinite loop that is equivalent to 'while' (true) as there is no needed test condition
  {
    digitalWrite(ledPIN, HIGH);
    delay(500);
    digitalWrite(ledPIN, LOW);
    delay(500);
  }
}

// taskTwo: serial output
void taskTwo(void *optionalArgs)
{
  Serial.print("Task 2 runs on core ");
  Serial.println(xPortGetCoreID());
  for (;;) // infinite loop
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