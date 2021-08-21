#include <Arduino.h>
#include <WiFiManager.h>
#include <ESPmDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>

//variabls for blinking an LED with Millis
const int led = 2; // ESP32 Pin to which onboard LED is connected
unsigned long previousMillis = 0;  // will store last time LED was updated
long interval = 50;  // interval at which to blink (milliseconds)
int ledState = LOW;  // ledState used to set the LED

void setup() {
    WiFi.mode(WIFI_STA); // explicitly set mode, esp defaults to STA+AP
    Serial.begin(115200);
    pinMode(led, OUTPUT);
 
//    //WiFiManager, Local intialization. Once its business is done, there is no need to keep it around
    WiFiManager wm;

//    //reset settings - wipe credentials for testing
//    wm.resetSettings();

//     Automatically connect using saved credentials,
//     if connection fails, it starts an access point with the specified name ( "AutoConnectAP"),
//     if empty will auto generate SSID, if password is blank it will be anonymous AP (wm.autoConnect())
//     then goes into a blocking loop awaiting configuration and will return success result

    bool res;
//    res = wm.autoConnect(); // auto generated AP name from chipid
//    res = wm.autoConnect("ESP32_AP"); // anonymous ap
    res = wm.autoConnect("ESP32_AP","password"); // password protected ap

    if(!res) {
        Serial.println("Failed to connect");
        ESP.restart();
    } 
    else {
//        if you get here you have connected to the WiFi    
        Serial.println("Connected!");
    }

//    OTA Intialize
   ArduinoOTA.setHostname("esp32");
   ArduinoOTA
    .onStart([]() {
      String type;
      if (ArduinoOTA.getCommand() == U_FLASH)
        type = "sketch";
      else // U_SPIFFS
        type = "filesystem";

      // NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
      Serial.println("Start updating " + type);
    })
    .onEnd([]() {
      Serial.println("\nEnd");
    })
    .onProgress([](unsigned int progress, unsigned int total) {
      Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
    })
    .onError([](ota_error_t error) {
      Serial.printf("Error[%u]: ", error);
      if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
      else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
      else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
      else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
      else if (error == OTA_END_ERROR) Serial.println("End Failed");
    });

  ArduinoOTA.begin();

  Serial.println("Ready");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

}

void loop() {
//  Leave in loop for OTA
ArduinoOTA.handle();
  
  //loop to blink without delay
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
  // save the last time you blinked the LED
  previousMillis = currentMillis;
  // if the LED is off turn it on and vice-versa:
  ledState = not(ledState);
  // set the LED with the ledState of the variable:
  digitalWrite(led,  ledState);
  interval = random(50,600);
  }
    
}