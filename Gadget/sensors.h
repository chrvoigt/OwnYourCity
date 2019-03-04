#ifndef SENSORS_H
#define SENSORS_H


// Averaging filter variables - SENSOR
const int numReadings = 8;
int readings[numReadings];      // the readings from the analog input
int runningIndex = 0;                  // the index of the current reading
long unsigned int total = 0;                  // the running total
int average = 0;                // the average




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
const int sampleWindow = 100; // Sample window width in mS (50 mS = 20Hz)
unsigned int sample;
double noise; // ideally in dB
#define NOISEPIN A0



/////////////////////////////////////////////////////////////////
// AIR (CO2 levels)
/////////////////////////////////////////////////////////////////
// The sensor needs a load-resistor at the output to ground. Its value could be from
// 2kOhm to 47kOhm. The lower the value, the less sensitive is the sensor. The higher
// the value, the less accurate is sensor for higher concentrations of gas. If only one
// specific gas is measured, the load-resistor can be calibrated by applying a known
// concentration of that gas. If the sensor is used to measure any gas (like in a air
// quality detector) the load-resistor could be set for a value of about 1V output with
// clean air. Choosing a good value for the load-resistor is only valid after the burn-in time

#define AIRPIN A0
//VARIABLES
int airSensorValue;
float airVoltage;

float ppm; //particle per million
float ppm_m1; //particle per million


/////////////////////////////////////////////////////////////////
// WIND
/////////////////////////////////////////////////////////////////
// Modern Device Wind Sensor

#define WINDPIN A0
//VARIABLES
int windSensorValue = 0;
double windVoltage = 0.0;



int readSensor()
{
  // subtract the last reading:
  total = total - readings[runningIndex];
  // read from the sensor:
  readings[runningIndex] = analogRead(A0);
  // add the reading to the total:
  total = total + readings[runningIndex];
  // advance to the next position in the array:
  runningIndex = runningIndex + 1;

  // if we're at the end of the array...
  if (runningIndex >= numReadings)
    // ...wrap around to the beginning:
    runningIndex = 0;

  // calculate the average:
  average = total / numReadings;
  //if (DEBUG) Serial.println(average);
  return average;

  //sensorVoltage = float(average) / 1024.0 * 5.0;

}





void initSensors()
{
  for (int thisReading = 0; thisReading < numReadings; thisReading++)
    readings[thisReading] = 0;

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

  if (WIND)
  {
    //ppm = 0;
  }

  if (DEBUG) Serial.println("Sensors set up");
}








/////////////////////////////////////////////////////////////////
void readSolarLevels()
{

  //setup vars for solar sensor readings
  sensorValue = readSensor(); //analogRead(SOLARPIN);
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









double mapE(double x, double in_min, double in_max, double out_min, double out_max)
{
  return (x - in_min) * (out_max - out_min + 1) / (in_max - in_min + 1) + out_min;
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
  double volts = ((float(peakToPeak) * 3.3) / 1024.) * 0.707;  // convert to RMS voltage
  if (DEBUG) Serial.print(peakToPeak);
  if (DEBUG) Serial.print(" ");
  if (DEBUG) Serial.print(volts);
  if (DEBUG) Serial.print(" ");
  //double first = log10(volts / 0.00631) * 20;
  //double dB = //first + 94 - 44 - 25;
  double dB = 20 * log10(volts) + 60;

  //delay(500);
  //return (dB);
  // worst case:
  dB = 120.*volts / 3.3; //map(volts,0.0,3.3,30.0,120.0);
  dB = mapE(volts, 0.0, 2.29, 30, 120);
  if (DEBUG) Serial.println(dB);
  //noise = dB;
  noise = volts;
}



/////////////////////////////////////////////////////////////////
void readWindSpeed()
{
  windSensorValue = readSensor(); //analogRead(SOLARPIN);
  windVoltage = 2.0 * (float(windSensorValue) * 3.3 / 1023.0); //analogRead(SOLARPIN);
  if (DEBUG) Serial.println(windVoltage);
  //windSpeed =

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
   get the calibrated ro based upon read resistance, and a know ppm
*/
//long mq135_getro(long resvalue, double ppm) {
//return (long)(resvalue * exp( log(MQ135_SCALINGFACTOR/ppm) / MQ135_EXPONENT ));
//}

/*
   get the ppm concentration
*/
double mq135_getppm(long resvalue, long ro) {
  double ret = 0;
  double validinterval = 0;
  validinterval = resvalue / (double)ro;
  if (validinterval < MQ135_MAXRSRO && validinterval > MQ135_MINRSRO) {
    ret = (double)((double)MQ135_SCALINGFACTOR * pow( ((double)resvalue / ro), MQ135_EXPONENT));
  }
  return ret;
}


void readAirQuality()
{


  // 5V stepped up to senser, output divided in half,
  // output adjusted to hw voltage division and 3.3V logic.
  // wemos: values are 0-3.3V corresponding to 0-1023
  int rawSensorValue = readSensor();//analogRead(A0);
  int adjustedSensorValue = 2 * map(rawSensorValue, 0, 775, 0, 511);
  float rawVoltage = (float(rawSensorValue) * 3.3 / 1023.);
  float adjustedVoltage = 2 * rawVoltage;
  Serial.print ( "rawS:");
  Serial.print ( rawSensorValue);
  Serial.print ( "  ");
  Serial.print ( "adjS:");
  Serial.print ( adjustedSensorValue);
  Serial.print ( "  ");
  Serial.print ( "rawV:");
  Serial.print ( rawVoltage);
  Serial.print ( "  ");
  Serial.print ( "adjV:");
  Serial.print ( adjustedVoltage);
  Serial.print ( "  ");

  // sensor fed directly from 3.3V power and read resutls
  //   int rawSensorValue = analogRead(A0);
  //   float rawVoltage = (float(rawSensorValue)*3.3/1023.);

  airSensorValue = rawSensorValue;//adjustedSensorValue;//2*;// Get AIQ value, multiplied 2 due to hw voltage division in half
  airVoltage = rawVoltage;//2*(airSensorValue*3.3/1023.);

  // method 1
  //  float resistance = ((1023./(float)airSensorValue) - 1.)*RLOAD;
  //  ppm_m1 =  PARA * pow((resistance/RZERO), -PARB);
  //  if(DEBUG) Serial.println(ppm_m1);

  // method 2:
  double valr = (double)airSensorValue;
  //Serial.println(val);
  double val =  ((float)40000 * (1023 - valr) / valr);
  //during clean air calibration, read the Ro value and replace MQ135_DEFAULTRO value with it, you can even deactivate following function call.
  //mq135_ro = mq135_getro(val, MQ135_DEFAULTPPM);
  //convert to ppm (using default ro)
  //ppm = mq135_getppm(val, MQ135_DEFAULTRO);
  ppm =  int(val);
  //ppm = airSensorValue;


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
unsigned long readingInterval = 100;
unsigned long lastReadingTime = 0;
void readSensors()
{

  if (millis() - lastReadingTime > readingInterval)
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
    if (WIND)
    {
      readWindSpeed();
    }
    lastReadingTime = millis();
  }
}


#endif
