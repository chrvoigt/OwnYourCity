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




void initInteractions()
{
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
        ButtonClickedB = true;
        display.clearDisplay();
        display.setCursor(0, 0);
      //display.setTextSize(1);
        display.println("Uploading");
        
        display.display();
    }
    //display sth
    if (SOLAR)
    {
      
      if(ButtonClickedB)
      {
        
        
        display.println("SolarCap:");
        display.print(solarCapacity);
        display.display();
        }
        else
        {
          //        #define BLACK    0x0000
      //#define BLUE     0x001F
      //#define RED      0xF800
      //#define GREEN    0x07E0
      //#define CYAN     0x07FF
      //#define MAGENTA  0xF81F
      //#define YELLOW   0xFFE0
      //#define WHITE    0xFFFF
      // display solar var
      display.clearDisplay();
      display.setCursor(0, 0);
      display.setTextSize(1);
      display.setTextColor(WHITE);

      display.println("SolarVolt:");
      display.println(solarVoltage);
      display.println();
      //display.setTextColor(WHITE);
      display.println("SolarCap:");
      display.print(solarCapacity);
      display.display();
          }
    }
    


  }

  //  if (BUTTON)
  //  {
  //
  //  }
}


#endif
