#include <TinyGPSPlus.h>

float LAT_POS, LNG_POS;

//WiFi and MQTT Setup Vars

const char* LAT_Top = "ESP32/GPS/LAT";
const char* LNG_Top = "ESP32/GPS/LNG";


// The TinyGPSPlus object
TinyGPSPlus gps;


void gps_check(){
  if (millis() > 5000 && gps.charsProcessed() < 10)
  {
    Serial.println(F("No GPS detected: check wiring."));
    while(true);
  }
}


void displayInfo()
{
//  Serial.print(F("Location: ")); 
  if (gps.location.isValid())
  {
    LAT_POS = gps.location.lat();
    LNG_POS = gps.location.lng();
//    Serial.print(LAT_POS, 6);
//    Serial.print(F(" , "));
//    Serial.println(LNG_POS, 6);

    DataToMsg(LAT_Top, LAT_POS);
    DataToMsg(LNG_Top, LNG_POS);
  }
  
}

void GPS_display(){
  
  // This sketch displays information every time a new sentence is correctly encoded.
  while (Serial2.available() > 0)
    if (gps.encode(Serial2.read()))
      displayInfo();
      
}
