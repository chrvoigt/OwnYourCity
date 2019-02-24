#ifndef COMMUNICATIONS_H
#define COMMUNICATIONS

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266HTTPClient.h>
ESP8266WiFiMulti WiFiMulti;

const char* ssid     = "whiterabbit";
const char* password = "salvador";

//VIA
// get this from the wia dashboard. it should start with `d_sk`
const char* device_secret_key = "d_sk_TSgqDz6aLeUbEELyK4oi0tUI";


// GOOGLE SPREADSHEETS




void initComm()
{
  if(DEBUG) Serial.print("Connecting to WiFi");
  if(OLED)
  {
    display.clearDisplay();
      display.setCursor(0, 0);
      display.setTextSize(1);
      display.setTextColor(WHITE);
      display.print("Connecting to WiFi");
      display.display();
    }
  WiFi.mode(WIFI_STA);
    WiFiMulti.addAP(ssid, password);
    
    while ((WiFiMulti.run() != WL_CONNECTED))
    {
      delay(500);
      if(DEBUG) Serial.print(".");
      if(OLED) 
      {
        display.print(".");
        display.display();
      }
      
      }
    //delay(3000);
    if(DEBUG) Serial.println("connected!");
    if(OLED) display.println("connected!");
  
//  if(VIA)
//  {}
  if (SPREADSHEET)
  {}
  if(DEBUG) Serial.println("Communications set up");
}


void communicate()
{
  
  }




#endif
