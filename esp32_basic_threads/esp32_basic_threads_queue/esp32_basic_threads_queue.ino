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