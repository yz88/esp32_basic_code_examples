# ESP32 communication between two tasks

Web-Links

- [heise developer - ESP32 to go](https://www.heise.de/developer/artikel/ESP32-to-go-4452689.html?view=print)

Required material

- ESP23 module, in this case ESP-WROOM-32U
- Arduino IDE
- micro USB cable

The first example is instructive, but somewhat theoretical. In practice, one of the tasks could take over communication with the outside world, while the second reads readings from sensors.

In any case, it would be helpful if tasks would also exchange information. The following example demonstrates exactly this. Using xQueueCreate, the main program creates a queue with a slot of the data type unsigned long. This time, the method xTaskCreate is used to create the tasks. The program implements a simple producer-consumer constellation.

The call of vTaskDelay is the ESP32 counterpart of Arduino's delay, but it works with higher resolution, so we still have to divide the desired time by the constant portTICK_PERIOD_MS. The constant indicates how many clock cycles are performed per millisecond.

To make sure that a value in the queue exists and is unread or a new one can already be written, the producer queries queue to zero before writing. Accordingly, the consumer checks whether a value actually exists in the queue.

```C
// create queue
QueueHandle_t queue = NULL;

void setup()
{
    printf("Starting two taks and one queue /n /n");
    queue = xQueueCreate(20, sizeof(unsigned long));
    if (queue != NULL)
    {
        printf("queue created \n");
        vTaskDelay(1000 / portTICK_PERIOD_MS); // wait for one second
        xTaskCreate(&producer, "producer", 2048, NULL, 5, NULL);
        printf("producer started \n");
        xTaskCreate(&consumer, "consumer", 2048, NULL, 5, NULL);
        printf("consumer started \n");
    }
    else
    {
        printf("not able to create queue \n");
    }
}

void consumer(void *pvParameter)
{
    unsigned long counter;
    if (queue == NULL)
    {
        printf("consumer task: queue not ready \n");
        return;
    }
    while (1)
    {
        xQueueReceive(queue, &counter, (TickType_t)(1000 / portTICK_PERIOD_MS));
        printf("consumer task: receive value from queue: %lu \n", counter);

        vTaskDelay(500 / portTICK_PERIOD_MS); // wait for half a second
    }
}

void producer(void *pvParameter)
{
    unsigned long counter = 1;
    if (queue == NULL)
    {
        printf("producer task: queue not ready \n");
        return;
    }
    while (1)
    {
        printf("producer task: send value to queue: %lu \n", counter);
        // write value of counter to queue
        xQueueSend(queue, (void *)&counter, (TickType_t)0);
        counter++;
        vTaskDelay(500 / portTICK_PERIOD_MS); // wait for half a second
    }
}

void loop() 
{
    // unused
}
```