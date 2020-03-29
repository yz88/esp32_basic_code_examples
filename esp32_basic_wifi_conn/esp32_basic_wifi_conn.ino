/*
ESP-WROOM-32U
Chip is ESP32D0WDQ6 (revision 1)
Features: WiFi, BT, Dual Core, 240MHz, VRef calibration in efuse, Coding Scheme None
MAC: 24:0a:c4:c5:4e:c0

// Possible values for WiFi status()
typedef enum
{
    WL_NO_SHIELD = 255,     // for compatibility with WiFi Shield library
    WL_IDLE_STATUS = 0,     // it is a temporary status assigned when WiFi.begin() is called and remains active until the number of attempts expires (resulting in WL_CONNECT_FAILED) or a connection is established (resulting in WL_CONNECTED);
    WL_NO_SSID_AVAIL = 1,   // assigned when no SSID are available
    WL_SCAN_COMPLETED = 2,  // assigned when the scan networks is completed
    WL_CONNECTED = 3,       // assigned when connected to a WiFi network
    WL_CONNECT_FAILED = 4,  // assigned when the connection fails for all the attempts
    WL_CONNECTION_LOST = 5, // assigned when the connection is lost
    WL_DISCONNECTED = 6     // assigned when disconnected from a network
} wl_status_t;
*/

#include <WiFi.h> // include Arduino Wifi library
#include "env.h"  //include custom header located in same directory

#ifndef WIFI_SSID
#define WIFI_SSID "your-wifi-ssid"    //wifi ssid
#define WIFI_PASSWORD "your-wifi-psk" //wifi password
#endif

#define LED_PIN_1 23 // GPIO pin for LED_1 (wifi status)

    void setup()
{
    Serial.begin(115200);
    delay(2000);                //wait some seconds to have time to start serial monitor to check output
    pinMode(LED_PIN_1, OUTPUT); // Initialize the LED pin as an output
    check_wifi_connection();    //Connect to Wifi network
}

//Setup connection to wifi
void check_wifi_connection()
{
    int wifi_retry = 0;
    while (WiFi.status() != WL_CONNECTED && wifi_retry < 10) // Wifi is not connected
    {
        digitalWrite(LED_PIN_1, LOW);
        wifi_retry++;
        Serial.println("WiFi not connected. Try to (re)connect");
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
}

void loop()
{
    check_wifi_connection();
}