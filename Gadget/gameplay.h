#ifndef GAMEPLAY_H
#define GAMEPLAY


//STACK: player has limited amount of data saving storage,
// when all storage is filled with sensor reading (eg one button interface)
// the player must return to wifi hotspot
//#define STACK true
//#define STACK_SIZE 20

// Solar:
// solar battery
double WattSec;
double chargedWattSec;
unsigned long lastPointUpdate;
unsigned long pointUpdateInterval = 1000;  // 1 second / every second
unsigned long lastNoiseLevelUpdate;
unsigned long noiseLevelUpdateInterval = 3000;  
bool resetCharge = false;
// Noise
int noiseLimit = 0;
bool gameOver = false;

void initGame()
{
  WattSec = 0.0;
  chargedWattSec = 0.0;
}

void makeGame()
{
  if (millis() - lastPointUpdate > pointUpdateInterval)
  {
    // update points
    if (SOLAR)
    {
      WattSec = solarPower;
      chargedWattSec += WattSec;
      lastPointUpdate = millis();
      if(resetCharge)
      {
        chargedWattSec = 0;
        resetCharge = false;
        }
      
    }

    if (NOISE)
    {
      if (millis() - lastNoiseLevelUpdate > noiseLevelUpdateInterval)
      {
        // check if noise
        if (noise > 1.0)
        {
          noiseLimit++;
          if (noiseLimit >= 5) 
          {
            // max reached
            noiseLimit = 5;
            gameOver = true;
            // fix value.  
          }
        }
        else
        {
          noiseLimit--;
          if (noiseLimit < 0) noiseLimit = 0;
        }
        lastNoiseLevelUpdate = millis();
      }

    }

  }

}


#endif


//CACHE_SIZE: player has limited amount of data saving storage,
// when all storage is filled with sensor reading (eg one button interface)
// the player must return to wifi hotspot
//#define CACHE_SIZE true
//#define CACHE_SIZE 20

//STACK: player has limited amount of data saving storage,
// when all storage is filled with sensor reading (eg one button interface)
// the player must return to wifi hotspot
//#define STACK true
//#define STACK_SIZE 20
