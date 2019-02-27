#ifndef SENSORS_H
#define SENSORS_H


/////////////////////////////////////////////////////////////////
// SOLAR
/////////////////////////////////////////////////////////////////
// variables
int sensorValue;
float solarVoltage;
float solarPower;
int solarCapacity; // between 0-100, from 0 to 5V
// hardware connections:
#define SOLARPIN A0
// solar panel used in games: 5 V/81 mA 
// power -> 5*0.081 = 0.405W energy producer at max potential: 100%
// collecting Wattsecs

/////////////////////////////////////////////////////////////////
// NOISE (Sound levels)
/////////////////////////////////////////////////////////////////
// references:
// https://blog.yavilevich.com/2016/08/arduino-sound-level-meter-and-spectrum-analyzer/
// https://circuitdigest.com/microcontroller-projects/arduino-sound-level-measurement
// https://learn.adafruit.com/adafruit-microphone-amplifier-breakout/measuring-sound-levels
// variables
const int sampleWindow = 50; // Sample window width in mS (50 mS = 20Hz)
unsigned int sample;
double noise; // ideally in dB
#define NOISEPIN A0

/////////////////////////////////////////////////////////////////
// AIR (CO2 levels)
/////////////////////////////////////////////////////////////////

#define AIRPIN A0
//VARIABLES
int airSensorValue;
float airVoltage;

float ppm; //particle per million
float ppm_m1; //particle per million





void initSensors()
{
  if (SOLAR)
  {
    //setup vars for solar sensor readings
    solarVoltage = 0.0;
    solarCapacity = 0;
  }
  if (NOISE)
  {
    noise = 0.0;
  }
  if (AIR)
  {
    ppm = 0;
    }
    
  if (DEBUG) Serial.println("Sensors set up");
}








/////////////////////////////////////////////////////////////////
void readSolarLevels()
{
  
  //setup vars for solar sensor readings
  sensorValue = analogRead(SOLARPIN);
  //if(DEBUG) Serial.println(sensorValue);
  // map 1023 input to 3.3V.
  // A0 is connected to voltage divider in half of solar panel, so multiply by 2 to adjust back.
  solarVoltage = 2 * (float(sensorValue) * 3.3 / 1023.0);
  //if(DEBUG) Serial.println(solarVoltage);
  solarCapacity = int(solarVoltage * 100 / 5);
  //if(DEBUG) Serial.println(solarCapacity);
  
  // solar panel used in games: 5 V/81 mA 
// power -> 5*0.081 = 0.405W energy producer at max potential: 100%
// Calculate Watt sec: the solar cell provides steady current through most of the voltage outptu
   solarPower = solarVoltage * 0.081; // in WAtts
   
}








/////////////////////////////////////////////////////////////////
void readMicLevel()
{
  unsigned long startMillis = millis(); // Start of sample window
  unsigned int peakToPeak = 0;   // peak-to-peak level

  unsigned int signalMax = 0;
  unsigned int signalMin = 1024;

  // collect data for 50 mS
  while (millis() - startMillis < sampleWindow)
  {
    sample = analogRead(NOISEPIN);
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
  //double volts = (peakToPeak * 3.3) / 1024;  // convert to volts
  double volts = ((peakToPeak * 3.3) / 1024) * 0.707;  // convert to RMS voltage
  double first = log10(volts / 0.00631) * 20;
  double dB = first + 94 - 44 - 25;
  if (DEBUG) Serial.println(dB);
  //delay(500);
  //return (dB);
  noise = dB;
}










/////////////////////////////////////////////////////////////////




// METHOD 1
// https://github.com/GeorgK/MQ135
///// The load resistance on the board
//#define RLOAD 10.0
///// Calibration resistance at atmospheric CO2 level
//#define RZERO 76.63
///// Parameters for calculating ppm of CO2 from sensor resistance
//#define PARA 116.6020682
//#define PARB 2.769034857
///// Atmospheric CO2 level for calibration purposes
////#define ATMOCO2 397.13


// METHOD 2
// https://github.com/empierre/arduino/blob/master/AirQuality-MQ135.ino
//  co2 levels: https://www.kane.co.uk/knowledge-centre/what-are-safe-levels-of-co-and-co2-in-rooms
#define MQ135_DEFAULTPPM 399 //default ppm of CO2 for calibration
#define MQ135_DEFAULTRO 497432 // 68550  //467046.0 //default Ro for MQ135_DEFAULTPPM ppm of CO2
#define MQ135_SCALINGFACTOR 116.6020682 //CO2 gas value
#define MQ135_EXPONENT -2.769034857 //CO2 gas value
#define MQ135_MAXRSRO 2.428 //for CO2
#define MQ135_MINRSRO 0.358 //for CO2
double mq135_ro = 10000;    // this has to be tuned 10K Ohm
double val = 0;                 // variable to store the value coming from the sensor

/*
 * get the calibrated ro based upon read resistance, and a know ppm
 */
//long mq135_getro(long resvalue, double ppm) {
//return (long)(resvalue * exp( log(MQ135_SCALINGFACTOR/ppm) / MQ135_EXPONENT ));
//}

/*
 * get the ppm concentration
 */
double mq135_getppm(long resvalue, long ro) {
double ret = 0;
double validinterval = 0;
validinterval = resvalue/(double)ro;
if(validinterval<MQ135_MAXRSRO && validinterval>MQ135_MINRSRO) {
ret = (double)((double)MQ135_SCALINGFACTOR * pow( ((double)resvalue/ro), MQ135_EXPONENT));
}
return ret;
}

void readAirQuality()
{
  // adjusted to hw voltage division 
  int rawSensorValue = analogRead(A0);
  int adjustedSensorValue = 2*map(rawSensorValue,0,775,0,511);
  float rawVoltage = (float(rawSensorValue)*3.3/1023.);
  float adjustedVoltage = 2*rawVoltage;
  airSensorValue = adjustedSensorValue;//2*;// Get AIQ value, multiplied 2 due to hw voltage division in half
  airVoltage = adjustedVoltage;//2*(airSensorValue*3.3/1023.);
  
  // method 1
//  float resistance = ((1023./(float)airSensorValue) - 1.)*RLOAD;
//  ppm_m1 =  PARA * pow((resistance/RZERO), -PARB);
//  if(DEBUG) Serial.println(ppm_m1);

  // method 2:
double valr = (double)airSensorValue;
  //Serial.println(val);
  double val =  ((float)22000*(1023-valr)/valr); 
  //during clean air calibration, read the Ro value and replace MQ135_DEFAULTRO value with it, you can even deactivate following function call.
  //mq135_ro = mq135_getro(val, MQ135_DEFAULTPPM);
  //convert to ppm (using default ro)
  ppm = mq135_getppm(val, MQ135_DEFAULTRO);
Serial.print ( "airSensorValue:");
  Serial.print ( airSensorValue);
  Serial.print ( "  ");
  Serial.print ( "Val / Ro / ppm:");
  Serial.print ( val);
  Serial.print ( " / ");
  //Serial.print ( mq135_ro);
  Serial.print ( " / ");
  Serial.println ( ppm);

  
}
unsigned long readingInterval = 250;
unsigned long lastReadingTime = 0;
void readSensors()
{
  
  if(millis()-lastReadingTime > readingInterval)
  {
    
  if (SOLAR)
  {
    readSolarLevels();
  }
  if (NOISE)
  {
    readMicLevel();
  }
  if (AIR)
  {
    readAirQuality();
  }
  lastReadingTime = millis();
  }
}


#endif
