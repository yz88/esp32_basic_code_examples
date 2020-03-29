# ESP32 basic MQTT connection

The following skecth will demonstrate a basic MQTT connection from A ESP32 to a broker. Only the puyblish function is used, the subscription is not covered.

As long the MQTT connection state is `MQTT_CONNECTED` LED_PIN_2 is high as an indicator for that connection. Additionally LED_PN_1 is high in case of a WiFi connction.

## MQTT

- [pubsubclient API](https://pubsubclient.knolleary.net/api.html#connected)

### int connected ()

Checks whether the client is connected to the server.

#### Returns connected()

| value | state                             |
| ----- | --------------------------------- |
| false | the client is no longer connected |
| true  | the client is still connected     |

### int state ()

Returns the current state of the client. If a connection attempt fails, this can be used to get more information about the failure. Constants defined in `PubSubClient.h`

#### Returns state()

| int | state                        | description                                              |
| --- | ---------------------------- | -------------------------------------------------------- |
| -4  | MQTT_CONNECTION_TIMEOUT      | the server didn't respond within the keepalive time      |
| -3  | MQTT_CONNECTION_LOST         | the network connection was broken                        |
| -2  | MQTT_CONNECT_FAILED          | the network connection failed                            |
| -1  | MQTT_DISCONNECTED            | the client is disconnected cleanly                       |
| 0   | MQTT_CONNECTED               | the client is connected                                  |
| 1   | MQTT_CONNECT_BAD_PROTOCOL    | the server doesn't support the requested version of MQTT |
| 2   | MQTT_CONNECT_BAD_CLIENT_ID   | the server rejected the client identifier                |
| 3   | MQTT_CONNECT_UNAVAILABLE     | the server was unable to accept the connection           |
| 4   | MQTT_CONNECT_BAD_CREDENTIALS | the username/password were rejected                      |
| 5   | MQTT_CONNECT_UNAUTHORIZED    | the client was not authorized to connect                 |

## The code

```c
/*
ESP-WROOM-32U
Chip is ESP32D0WDQ6 (revision 1)
Features: WiFi, BT, Dual Core, 240MHz, VRef calibration in efuse, Coding Scheme None
MAC: 24:0a:c4:c5:4e:c0
*/

#include <WiFi.h>         // Wifi built-in by aArduino
#include <PubSubClient.h> // PubSubClient library by Nick O'Leary
#include "env.h"          // include custom header located in same directory

#ifndef WIFI_SSID
#define WIFI_SSID "your-wifi-ssid"    //wifi ssid
#define WIFI_PASSWORD "your-wifi-psk" //wifi password
#endif
WiFiClient wifi_client;

#define mqtt_server "192.168.178.25"   // MQTT server name or IP
#define mqtt_client_name "ESP32Client" // MQTT client name
PubSubClient mqtt_client(wifi_client);

#define LED_PIN_1 23 // GPIO pin for LED_1 (wifi status)
#define LED_PIN_2 22 // GPIP pin for LED_2 (mqtt status)

void setup()
{
    Serial.begin(115200);

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
            Serial.println();
            Serial.println("WiFi is connected");
            Serial.print("MAC address:   ");
            Serial.println(WiFi.macAddress()); //ESP's mac address
            Serial.print("IP address:    ");
            Serial.println(WiFi.localIP()); //ESP's IP address
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
        // Create a random client ID
        String mqtt_clientId = "ESP32_";
        mqtt_clientId += WiFi.macAddress();
        // Attempt to connect
        if (mqtt_client.connect(mqtt_clientId.c_str()))
        {
            Serial.println("MQTT connection OK");
            digitalWrite(LED_PIN_2, HIGH);
        }
        else
        {
            Serial.print("MQTT connection failed, Return Code: ");
            Serial.print(mqtt_client.state());
            Serial.println(" try again in 5 seconds");
            // Wait 5 seconds before retrying
            digitalWrite(LED_PIN_2, LOW);
            delay(5000);
        }
    }
    return 1;
}

void loop()
{
    Serial.print("WiFi status: ");
    Serial.println(WiFi.status());
    //check_wifi_connection();

    Serial.print("MQTT status: ");
    Serial.println(mqtt_client.state());

    Serial.print("MQTT connected(): ");
    Serial.println(mqtt_client.connected());

    // Publish values to MQTT topics
    if (check_mqtt_connection())
    {
        mqtt_client.publish("topic_test", "message_test");
    }

    delay(5000);
}
```
