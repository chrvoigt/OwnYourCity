// gadget code with sensor and interaction

#define DEBUG true

// GAMEPLAY
#define TEAM_NAME "airRabbit1" 






// IX
#define OLED true


// SENSORS:
#define SOLAR true
#define AIR false
#define NOISE false



// COMMUNICATION
#define SPREADSHEET true

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
