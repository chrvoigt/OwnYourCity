#ifndef GEOLOCATION_H
#define GEOLOCATION

// Resources:
/* Google API
 *  https://developers.google.com/maps/documentation/geolocation/intro 
 *  
 *  cost: https://mapsplatformtransition.withgoogle.com/calculator 
 *  restrict the use of the key and bind it to specific IPs 
 *  wir müßten die Mobile Network Data von allen Providern finden MCC und MNC 
bap: https://www.austriahandyboerse.at/apn-einstellungen-oesterreich/ 

// needed to build the son string : 

Serial.println("{");
      Serial.println("\"homeMobileCountryCode\": 232,");  // check access point details 
      Serial.println("\"homeMobileNetworkCode\": 10,");    
      Serial.println("\"radioType\": \"lte\",");          
      Serial.println("\"carrier\": \"Drei\",");         

 *  
 *  
 *  use version 5.x of Arduinojson 
https://www.hackster.io/techiesms/geo-location-using-only-esp8266-without-gps-module-94defd

https://github.com/techiesms/Geolocation/blob/master/Geolocation_without_display.ino

Specialties:
#include <ArduinoJson.h> // make sure to install version 5.13.x 
and … 
//Credentials for Google GeoLocation API...
const char* Host = "www.googleapis.com";
String thisPage = "/geolocation/v1/geolocate?key=";
String key = “your_Google_API_key";

 



1        Code & Project description

https://www.hackster.io/techiesms/geo-location-using-only-esp8266-without-gps-module-94defd

https://github.com/techiesms/Geolocation/blob/master/Geolocation_without_display.ino

 

2        Products

https://developers.google.com/products/

GeoLocation API: https://developers.google.com/maps/documentation/geolocation/intro

Prices : https://mapsplatformtransition.withgoogle.com/calculator

Caping API usage: https://cloud.google.com/apis/docs/capping-api-usage

 

3        Access Point Names (APN)

https://www.austriahandyboerse.at/apn-einstellungen-oesterreich/

 

4        Geocode Conversion

https://www.latlong.net/Show-Latitude-Longitude.html


*/
#endif
