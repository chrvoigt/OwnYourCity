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
  //#include <WiFiClientSecure.h>
  //const char* host = "script.google.com";
  //const int httpsPort = 443;
  //// Use WiFiClientSecure class to create TLS connection
  //WiFiClientSecure client;
  //// SHA1 fingerprint of the certificate, don't care with your GAS service
  //const char* fingerprint = "46 B2 C3 44 9C 59 09 8B 01 B6 F8 BD 4C FB 00 74 91 2F EF F6";
  ////String GAS_ID = "";   // Replace by your GAS service id           !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
  ////int it;
  ////int ih;
  
  #include "HTTPSRedirect.h"
  
  
  // The ID below comes from Google Sheets.
  // Towards the bottom of this page, it will explain how this can be obtained
  const char *GScriptId = "AKfycbz5tC_UlYMB8yrYruS4_F90e7sBZnUejku_2_-HYgVRyiqcAXU5";
  
  // Push data on this interval
  //const int dataPostDelay = 900000;  // 15 minutes = 15 * 60 * 1000
  
  const char* host = "script.google.com";
  const char* googleRedirHost = "script.googleusercontent.com";
  
  const int httpsPort =  443;
  HTTPSRedirect client(httpsPort);
  
  // Prepare the url (without the varying data)
  String url = String("/macros/s/") + GScriptId + "/exec?";
  
  // echo | openssl s_client -connect script.google.com:443 | openssl x509 -fingerprint -noout
  const char* fingerprint = "E4:A8:7B:F5:3E:9A:17:4A:E2:9F:26:8F:81:23:78:E3:15:08:85:99";
  
  
  void initComm()
  {
    if (DEBUG) Serial.print("Connecting to WiFi");
    if (OLED)
    {
      display.clearDisplay();
      display.setCursor(0, 0);
      display.setTextSize(1);
      display.setTextColor(WHITE);
      display.print("Connecting to WiFi");
      display.display();
    }
  
    //  WiFi.begin(ssid, password);
    //  while (WiFi.status() != WL_CONNECTED) {
    //    delay(500);
    //    Serial.print(".");
    //  }
  
  
  
    WiFi.mode(WIFI_STA);
    WiFiMulti.addAP(ssid, password);
  
    while ((WiFiMulti.run() != WL_CONNECTED))
    {
      delay(1000);
      if (DEBUG) Serial.print(".");
      if (OLED)
      {
        display.print(".");
        display.display();
      }
  
    }
    //Serial.println(WiFi.localIP());
    //delay(3000);
  
    if (DEBUG) Serial.println("connected!");
    if (OLED) display.println("connected!");
    if (DEBUG) Serial.println(" IP address: ");
    if (DEBUG) Serial.println(WiFi.localIP());
  
    if (SPREADSHEET)
    {
      if (DEBUG)
      {
        Serial.print(String("Connecting to "));
        Serial.println(host);
  
      }
      bool flag = false;
      for (int i = 0; i < 5; i++) {
        int retval = client.connect(host, httpsPort);
        if (retval == 1) {
          flag = true;
          break;
        }
        else if (DEBUG) Serial.println("Connection failed. Retrying...");
      }
      if (DEBUG)
      {
        // Connection Status, 1 = Connected, 0 is not.
        Serial.println("Connection Status: " + String(client.connected()));
        Serial.flush();
  
        if (!flag) {
          Serial.print("Could not connect to server: ");
          Serial.println(host);
          Serial.println("Exiting...");
          Serial.flush();
          return;
        }
        // Data will still be pushed even certification don't match.
        if (client.verify(fingerprint, host)) {
          Serial.println("Certificate match.");
        } else {
          Serial.println("Certificate mis-match");
        }
  
      }
  
    }
  
    //  if(VIA)
    //  {}
  
    if (DEBUG) Serial.println("Communications set up");
  }
  
  // This is the main method where data gets pushed to the Google sheet
  void postSheetData(String team, String tag, float value) {
    if (!client.connected()) {
      Serial.println("Connecting to client again...");
      client.connect(host, httpsPort);
    }
    String urlFinal = url + "team=" + team + "&tag=" + tag + "&value=" + String(value);
    client.printRedir(urlFinal, host, googleRedirHost);
  }
  
  unsigned long prevMillis = 0;
  void communicate()
  {
    //if (SPREADSHEET) sendSheetData();
    if (UPLOAD_TIMER)
    {
      if (millis() - prevMillis > (UPLOAD_TIMER_SECONDS * 1000) )
      {
        // upload data
        if (SPREADSHEET) 
        {
          if(SOLAR) 
          {
            // check for button click
            if(ButtonClickedB == true)
            {
              postSheetData(TEAM_NAME,TAG_NAME,solarCapacity);
            if (DEBUG) Serial.print("SOLAR uploaded to SHEET: ");
            if (DEBUG) Serial.println(solarCapacity);
            
            
              ButtonClickedB = false;
              prevMillis = millis();
              }
            
            
          }
        }
        
      }
    }
  
  
  
  }
  
  
  
  
  #endif
