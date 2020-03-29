# ESP32 publish DHT sensor data via MQTT

Web-Links

- [Arduino WiFi library](https://www.arduino.cc/en/Reference/WiFi)
- [Adafruit DHT sensor library](https://github.com/adafruit/DHT-sensor-library)
- [mqtt pubsubclient](https://github.com/knolleary/pubsubclient)

<https://www.pubnub.com/blog/pubsub-nodemcu-32s-esp32-mqtt-pubnub-arduino-sdk/>

## project description

Use an ESP32 board to send DHT values (temperature and hunidity) to a MQTT broker via a WiFi connection.

Two LEDs are used as status indicators for WiFi and MQTT connection status.

## wiring diagram

![Fritzing wiring diagram](https://github.com/yz88/esp32_basic_code_examples/blob/master/esp32_basic_dht_mqtt/fritzing/esp32_basic_dht_breadboard.png)

## Program code

```c
/*
ESP-WROOM-32U
Chip is ESP32D0WDQ6 (revision 1)
Features: WiFi, BT, Dual Core, 240MHz, VRef calibration in efuse, Coding Scheme None
MAC: 24:0a:c4:c5:4e:c0
*/

#include <WiFi.h>         // Wifi built-in by aArduino
#include <DHT.h>          // DHT sensor library by Adafruit
#include <PubSubClient.h> // PubSubClient library by Nick O'Leary
#include "env.h"          // include custom header located in same directory

#ifndef WIFI_SSID
#define WIFI_SSID "your-wifi-ssid"    //wifi ssid
#define WIFI_PASSWORD "your-wifi-psk" //wifi password
#endif
WiFiClient wifi_client;

#define mqtt_server "192.168.178.25" // MQTT server name or IP
PubSubClient mqtt_client(wifi_client);

#define DHTTYPE DHT22 // DHT 22 (AM2302)
#define DHT_PIN_1 15  // GPIO pin for DHT sensor DHT22_1
#define DHT_PIN_2 2   // GPIO pin for DHT sensor DHT22_2
DHT dht22_1(DHT_PIN_1, DHTTYPE);
DHT dht22_2(DHT_PIN_2, DHTTYPE);

#define LED_PIN_1 23 // GPIO pin for LED_1 (wifi status)
#define LED_PIN_2 22 // GPIP pin for LED_2 (mqtt status)

#define DEBUG 1

void setup()
{
    Serial.begin(115200);

    dht22_1.begin();
    dht22_2.begin();

    pinMode(LED_PIN_1, OUTPUT); // Initialize the LED pin as an output
    pinMode(LED_PIN_2, OUTPUT); // Initialize the LED pin as an output

    delay(2000);             // wait some seconds to have enough time to start serial monitor to check output
    check_wifi_connection(); //Connect to Wifi network
    delay(2000);
    mqtt_client.setServer(mqtt_server, 1883); // Configure MQTT connection, change port if needed
}

//Setup connection to wifi
boolean check_wifi_connection()
{
    int wifi_retry = 0;
    while (WiFi.status() != WL_CONNECTED && wifi_retry < 10) // Wifi is not connected
    {
        digitalWrite(LED_PIN_1, LOW);
        wifi_retry++;
        Serial.println("WiFi not connected. Try to (re)connect...");
        WiFi.disconnect();
        WiFi.mode(WIFI_OFF);
        WiFi.mode(WIFI_STA);
        WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
        Serial.printf("Connecting to %s with password %s\n", WIFI_SSID, WIFI_PASSWORD);
        delay(1000);
        if (WiFi.status() == WL_CONNECTED) // WiFi is connected now, print info
        {
            Serial.println("WiFi is connected");
            Serial.print("MAC address:   ");
            Serial.println(WiFi.macAddress()); //ESP's mac address
            Serial.print("IP address:    ");
            Serial.println(WiFi.localIP()); //ESP's IP address

#ifdef DEBUG
            Serial.print("Subnet mask:   ");
            Serial.println(WiFi.subnetMask()); //ESP's IP subnet mask
            Serial.print("Defaut GW:     ");
            Serial.println(WiFi.gatewayIP()); //ESP'S IP default gateway
            Serial.print("WiFi SSID:     ");
            Serial.println(WiFi.SSID());
            Serial.print("WiFi PSK:      ");
            Serial.println(WiFi.psk());
            Serial.print("WiFi RSSI:     ");
            Serial.println(WiFi.RSSI()); //ESP's WiFi Receive Signal Strenght
            Serial.print("WiFi Tx Power: ");
            Serial.println(WiFi.getTxPower()); //ESP's WiFi Tx Power
            Serial.print("BSSI:          ");
            Serial.println(WiFi.BSSIDstr()); //BSSI the ESP is connected to
            Serial.print("AutoConnect:   ");
            Serial.println(WiFi.getAutoConnect()); //ESP's WiFi autoconnect on start
            Serial.print("ReConnect:     ");
            Serial.println(WiFi.getAutoReconnect()); //ESP's WiFi reconnect mode
#endif

            digitalWrite(LED_PIN_1, HIGH);
        }
    }
    if (wifi_retry >= 10)
    {
        Serial.println("\nReboot ESP because of failed WiFi connection");
        ESP.restart();
    }
    return 1;
}

boolean check_mqtt_connection()
{
    // Loop until we're reconnected
    while (!mqtt_client.connected())
    {
        Serial.println("MQTT not connected. Try to (re)connect...");
        // Create a MQTT client ID from WiFi MAC
        String mqtt_client_id = "ESP32_";
        mqtt_client_id += WiFi.macAddress();
        // Attempt to connect
        if (mqtt_client.connect(mqtt_client_id.c_str()))
        {
            Serial.println("MQTT connection OK");
            digitalWrite(LED_PIN_2, HIGH);
        }
        else
        {
            Serial.print("MQTT connection failed, Return Code : ");
            Serial.print(mqtt_client.state());
            Serial.println(" try again in 5 seconds");
            // Wait 5 seconds before retrying
            delay(5000);
        }
    }
    return 1;
}

void loop()
{
    // read sensor values from sensor 1
    float humidity_dht22_1 = dht22_1.readHumidity();
    float temperature_dht22_1 = dht22_1.readTemperature();

    if (isnan(humidity_dht22_1) || isnan(temperature_dht22_1)) // check wether values is not a number (isnan) -> failure
    {
        Serial.println("DHT22_1: Failed to read from DHT sensor!");
    }
    else
    {
        Serial.print("DHT22_1: Humidity: ");
        Serial.print(humidity_dht22_1);
        Serial.print("%  Temperature: ");
        Serial.print(temperature_dht22_1);
        Serial.println("°C ");

        // Publish values to MQTT topics
        if (mqtt_client.connected())
        {
            mqtt_client.publish("sensors/esp32/temperature_dht22_1", String(temperature_dht22_1).c_str()); // Publish temperature
            delay(50);
            mqtt_client.publish("sensors/esp32/humidity_dht22_1", String(humidity_dht22_1).c_str()); // Publish humidity
            delay(50);
        }
        else // MQTT connection failure
        {
            if (WiFi.status() == WL_CONNECTED) // if WiFI connection OK check MQTT connection
            {
                check_mqtt_connection();
            }
            else // else check WIFI connection
            {
                check_wifi_connection();
            }
        }
    }

    // read sensor values from sensor 2
    float humidity_dht22_2 = dht22_2.readHumidity();
    float temperature_dht22_2 = dht22_2.readTemperature();

    if (isnan(humidity_dht22_2) || isnan(temperature_dht22_2)) // check wether values is not a number (isnan) -> failure
    {
        Serial.println("DHT22_2: Failed to read from DHT sensor!");
    }
    else
    {
#ifdef DEBUG
        Serial.print("DHT22_2: Humidity: ");
        Serial.print(humidity_dht22_2);
        Serial.print("%  Temperature: ");
        Serial.print(temperature_dht22_2);
        Serial.println("°C ");
#endif

        // Publish values to MQTT topics
        if (mqtt_client.connected())
        {
            mqtt_client.publish("sensors/esp32/temperature_dht22_2", String(temperature_dht22_2).c_str()); // Publish temperature
            delay(1000);
            mqtt_client.publish("sensors/esp32/humidity_dht22_2", String(humidity_dht22_2).c_str()); // Publish humidity
            delay(1000);
        }
        else // MQTT connection failure
        {
            if (WiFi.status() == WL_CONNECTED) // if WiFI connection OK check MQTT connection
            {
                check_mqtt_connection();
            }
            else // else check WIFI connection
            {
                check_wifi_connection();
            }
        }
    }

    delay(15000);
}
```
