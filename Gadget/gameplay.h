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
