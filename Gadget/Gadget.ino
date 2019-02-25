// gadget code with sensor and interaction

#define DEBUG true

// GAMEPLAY

//STACK: player has limited amount of data saving storage, 
// when all storage is filled with sensor reading (eg one button interface) 
// the player must return to wifi hotspot 
//#define STACK true
//#define STACK_SIZE 20

//TIMER: player has predefined rest time after taking a sensor reading:
// eg: after the palyer takes a reading, he needs to wait 5 mins to be able to take another one. 
#define UPLOAD_TIMER true
#define UPLOAD_TIMER_SECONDS 3

#define TEAM_NAME "redRabbit"
#define TAG_NAME "Solar"

// depending on hardware connections, define the following 
// precompiler directives as true or false

// IX
// inputs:
//#define BUTTON false
// outputs:
//#define LEDS false
#define OLED true
//#define BUZZER false

// SENSORS:
#define SOLAR true
//#define AIR false
#define NOISE false

// COMMUNICATION
//#define WIA false
#define SPREADSHEET true


#include "sensors.h" 
#include "interaction.h" 
#include "communications.h" 






void setup() {
  if(DEBUG) Serial.begin(115200);
  // set up screens, buttons, leds first to get feedback over other domains
  initInteractions();
  initSensors();
  initComm();
  //initGeoloc();
}

void loop() {
  // put your main code here, to run repeatedly:
  readSensors();
  processInteractions();
  communicate();
  //geolocate();
}
