#ifndef SENSORS_H
#define SENSORS_H

// hardware connections:
#define SOLARPIN A0

// variables
// SOLAR
int sensorValue;
float solarVoltage;
int solarCapacity; // between 0-100, from 0 to 5V

// NOISE (Sound levels)
// https://learn.adafruit.com/adafruit-microphone-amplifier-breakout/measuring-sound-levels
/****************************************
Example Sound Level Sketch for the 
Adafruit Microphone Amplifier
****************************************/
 
const int sampleWindow = 50; // Sample window width in mS (50 mS = 20Hz)
unsigned int sample;


void initSensors()
{
  if(SOLAR)
  {
    //setup vars for solar sensor readings
    solarVoltage = 0.0;
    solarCapacity = 0;
    }
    if(DEBUG) Serial.println("Sensors set up");
  }

void readSensors()
{
  if(SOLAR)
  {
    readSolarLevel();
    }
    if(NOISE)
    {
      readSoundLevel();
      }
  }
void readSolarLevel()
{
      //setup vars for solar sensor readings
    sensorValue = analogRead(SOLARPIN);
    //if(DEBUG) Serial.println(sensorValue);
    // map 1023 input to 3.3V 
    solarVoltage = 2*(float(sensorValue)*3.3/1023.0);
    //solarVoltage = 2*map(float(sensorValue),0,1023,0,3.3);
    if(DEBUG) Serial.println(solarVoltage);
    solarCapacity = int(solarVoltage*100/5);
    if(DEBUG) Serial.println(solarCapacity);
  }
void readSoundLevel() 
{
   unsigned long startMillis= millis();  // Start of sample window
   unsigned int peakToPeak = 0;   // peak-to-peak level
 
   unsigned int signalMax = 0;
   unsigned int signalMin = 1024;
 
   // collect data for 50 mS
   while (millis() - startMillis < sampleWindow)
   {
      sample = analogRead(0);
      if (sample < 1024)  // toss out spurious readings
      {
         if (sample > signalMax)
         {
            signalMax = sample;  // save just the max levels
         }
         else if (sample < signalMin)
         {
            signalMin = sample;  // save just the min levels
         }
      }
   }
   peakToPeak = signalMax - signalMin;  // max - min = peak-peak amplitude
   double volts = (peakToPeak * 5.0) / 1024;  // convert to volts
 
   Serial.println(volts);
}


#endif
