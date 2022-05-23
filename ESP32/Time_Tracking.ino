#include "time.h"

//Time server and offsets
const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = 3600;
const int   daylightOffset_sec = 3600;

int LocalMin, LocalHr;
int tempHr, tempMin, tempSec = 0;

unsigned long int oldMilliTime, MilliTime = 0;

const char* Separation = ":";
const char* Zero = "0";

void setup_Time() {
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
}

void get_Time() {
  struct tm timeinfo;
  //If time can't be updated localy, switch to local time tracking
  if (!getLocalTime(&timeinfo)) {
    Serial.println("Failed to obtain time, calculating localy...");

    //Check how much time has passed from last timestamp
    oldMilliTime = MilliTime;
    MilliTime = millis();

    tempSec = (MilliTime - oldMilliTime) / 1000;

    tempHr = tempSec / 3600;

    tempMin = ( tempSec - (3600 * tempHr) ) / 60;

    //Update the local time (numeric values)
    if ( (tempMin + LocalMin) >= 60) {
      tempHr += 1;
      LocalMin += tempMin;
      LocalMin -= 60;
    }
    else {
      LocalMin += tempMin;
    }

    if ( (tempHr + LocalHr) >= 24) {
      LocalHr += tempHr;
      LocalHr -= 24;
    }
    else {
      LocalHr += tempHr;

    }

    //Update the local time (char values)
    char LocalStamp[6] = {};

    String temp1 = String(LocalHr);
    char timeHourLocal[temp1.length() + 1];
    temp1.toCharArray(timeHourLocal, temp1.length() + 1);

    String temp2 = String(LocalMin);
    char timeMinLocal[temp2.length() + 1];
    temp2.toCharArray(timeMinLocal, temp2.length() + 1);

    //Add 0 in front of numbers if needed to preserve HH:MM format
    if (LocalHr <= 9) {
      strcpy(LocalStamp, Zero);
      strcat(LocalStamp, timeHourLocal);
    }
    else {
      strcpy(LocalStamp, timeHourLocal);
    }

    if (LocalMin <= 9) {
      strcat(LocalStamp, Separation);
      strcat(LocalStamp, Zero);
      strcat(LocalStamp, timeMinLocal);
    }
    else {
      strcat(LocalStamp, Separation);
      strcat(LocalStamp, timeMinLocal);
    }

    Serial.println(LocalStamp);

    store_Time(LocalStamp);

  }

  else {

    //Update the local timestamp
    oldMilliTime = MilliTime;
    MilliTime = millis();

    //Update char time values from online time
    char timeHour[3];
    char timeMinutes[3];
    strftime(timeHour, 3, "%H", &timeinfo);
    strftime(timeMinutes, 3, "%M", &timeinfo);

    //Save to local numeric time
    LocalHr = atoi(timeHour);
    LocalMin = atoi(timeMinutes);

    Serial.print("Time: ");

    //Save to local char time
    char LocalStamp[6] = {};

    strcpy(LocalStamp, timeHour);
    strcat(LocalStamp, Separation);
    strcat(LocalStamp, timeMinutes);

    Serial.println(LocalStamp);

    store_Time(LocalStamp);

  }
}
