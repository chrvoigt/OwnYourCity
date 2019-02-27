#ifndef INTERACTION_H
#define INTERACTION


// OLED
#if OLED
#include <Wire.h> // SCL: D1 (GPIO5); SDA: D2 (GPIO4)
#include <Adafruit_SSD1306.h>
#define OLED_RESET 0  // GPIO0
Adafruit_SSD1306 display(OLED_RESET);
#include <LOLIN_I2C_BUTTON.h> // Buttons: A: D3 (GPIO0); B: D4 (GPIO2)
I2C_BUTTON button(DEFAULT_I2C_BUTTON_ADDRESS); //I2C Address 0x31
#include <Adafruit_GFX.h>
bool ButtonClickedB = false;
#endif

bool uploadRequest;



void initInteractions()
{
  uploadRequest = false;
  if (OLED)
  {
    display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3C (for the 64x48)
    display.display();

    display.clearDisplay();
    pinMode(D4, INPUT); // Setting D4 B Button as input
    pinMode(D3, INPUT); // Setting D3 A Button as input

  }
  //if(BUTTON)

  if (DEBUG) Serial.println("Interaction methods set up");
}
void processInteractions()
{
  if (OLED)
  {
    
    // handle buttons
    if (digitalRead(D3) == LOW) //A
    {

    }
    if (digitalRead(D4) == LOW) //B
    {

      uploadRequest = true;
      ButtonClickedB = true;

    }
    //display values
    display.clearDisplay();
    display.setCursor(0, 0);
      display.setTextSize(1);
    if(offline) display.println("OFFLINE");
    
    // DISPLAY
    if (uploadRequest)
    {
      // show upload screen
      
      display.setTextSize(1);
      display.println("Uploading");
      if (SOLAR)
      {
        display.println("Charge:");
        display.setTextSize(2);
        display.println(chargedWattSec);
        display.setTextSize(1);
        display.println("Ws");
      }
      if (NOISE)
      {
        display.println("NoiseLevel");
        display.setTextSize(2);
        display.print((int)noise);
        display.println("dB");
      }
      if (AIR)
      {
        //display.println("AirQuality");
        //display.setTextSize(2);

        display.print(ppm);
        display.println("ppm");
      }
      display.display();
      ButtonClickedB = false;
    }
    else
    {
      // display sensor values, etc.
      
      //display.setCursor(0, 0);
      display.setTextSize(1);
      display.setTextColor(WHITE);

      if (SOLAR)
      {
        
        display.setTextSize(1);
//        display.println("SolarVolt:");
//        display.print(solarVoltage);
//        display.println("V");
//        display.println();
        //display.setTextColor(WHITE);
        display.println("Solar");
        display.println("Potential:");
        display.print(solarPower);
        display.println("W");
        display.println("Charged:");
        display.print(chargedWattSec);
        display.println(" Ws");
        
        
      }
      if (NOISE)
      {
        display.setTextSize(1);
        display.println("NoiseLevel");
        display.setTextSize(2);
        display.print((int)noise);
        display.println("dB");
        
      }
      if (AIR)
      {
        
        display.print(airSensorValue);
        display.println("");
        display.print(airVoltage);
        display.println("V");
        
        //display.println("AirQuality");
        display.setTextSize(1);
        display.print(ppm);
        display.println("ppm");
//        display.print(ppm_m1);
//        display.println("ppm_m1");
      }
      
    }
    display.display();
  }

  //  if (BUTTON)
  //  {
  //
  //  }
}


#endif
