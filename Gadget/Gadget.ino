// gadget code with sensor and interaction



// GAMEPLAY
#define TEAM_NAME "groupName" 

// SENSORS:
#define SOLAR false
#define AIR false
#define WIND true
#define NOISE false







#define DEBUG true



// IX
#define OLED true
// COMMUNICATION
#define SPREADSHEET true
#define LED true
bool offline = true;

#include "sensors.h" 
#include "gameplay.h"
#include "interaction.h" 
#include "communications.h" 
 






void setup() {
  if(DEBUG) Serial.begin(115200);
  // set up screens, buttons, leds first to get feedback over other domains
  initInteractions();
  initSensors();
  initComm();
  initGame();
  //initGeoloc();
}

void loop() {
  // put your main code here, to run repeatedly:
  readSensors();
  processInteractions();
  makeGame();
  communicate();
  //geolocate();
}
