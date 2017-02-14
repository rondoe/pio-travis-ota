#include "Arduino.h"
#include <FS.h>                   //this needs to be first, or it all crashes and burns...

#include <ESP8266WiFi.h>          //https://github.com/esp8266/Arduino
#include <ESP8266HTTPClient.h>
#include <ESP8266httpUpdate.h>

//needed for library
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>          //https://github.com/tzapu/WiFiManager


#include "Timer.h"
Timer t;


void checkUpdate() {
        Serial.println("Check for updates");
        t_httpUpdate_return ret = ESPhttpUpdate.update("https://upthrow.rondoe.com/update/esp?token=xhestcal842myu1ewrupfgpxsy1b352cq2psfohs2nrdxmvsxu0036pw9tdh5f4b", "", "A8 A8 4C 5D C7 5D 03 BB 1A 4C 47 13 B9 33 4C A6 4D AF 4A 63");

        switch(ret) {
        case HTTP_UPDATE_FAILED:
                Serial.printf("HTTP_UPDATE_FAILD Error (%d): %s", ESPhttpUpdate.getLastError(), ESPhttpUpdate.getLastErrorString().c_str());
                Serial.println();
                break;

        case HTTP_UPDATE_NO_UPDATES:
                Serial.println("HTTP_UPDATE_NO_UPDATES");
                break;

        case HTTP_UPDATE_OK:
                Serial.println("HTTP_UPDATE_OK");
                break;
        }
}

// the setup function runs once when you press reset or power the board
void setup() {
        Serial.begin(115200);

        //WiFiManager
        //Local intialization. Once its business is done, there is no need to keep it around
        WiFiManager wifiManager;

        //exit after config instead of connecting
        wifiManager.setBreakAfterConfig(true);


        //tries to connect to last known settings
        if (!wifiManager.autoConnect("AutoConnectAP", "password")) {
                Serial.println("failed to connect, we should reset as see if it connects");
                delay(3000);
                ESP.reset();
                delay(5000);
        }

        //if you get here you have connected to the WiFi
        Serial.println("connected...yeey :)");


        Serial.println("local ip");
        Serial.println(WiFi.localIP());

        checkUpdate();
        // check for software update every minute
        t.every(1000 * 60, checkUpdate);

        // initialize digital pin 13 as an output.
        pinMode(2, OUTPUT);
}

// the loop function runs over and over again forever
void loop() {
        t.update();
        digitalWrite(2, HIGH); // turn the LED on (HIGH is the voltage level)
        delay(100);        // wait for a second
        digitalWrite(2, LOW); // turn the LED off by making the voltage LOW
        delay(100);        // wait for a second
}
