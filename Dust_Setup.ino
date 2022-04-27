char dustMsg[30];

int LDS_PM10, preLDS_PM10 = 0;
int LDS_PM25, preLDS_PM25 = 0;
int LDS_PM100, preLDS_PM100 = 0;

const char* Dust_PM10 = "Group3/Projec3/Dust/PM10";
const char* Dust_PM25 = "Group3/Project3/Dust/PM25";
const char* Dust_PM100 = "Group3/Project3/Dust/PM100";

const char* Status = "Group3/Projec3/Status";


void LDS_Data_Handler() {

  if (preLDS_PM10 != LDS_PM10) {

    DataToMsg(Dust_PM10, LDS_PM10);

    Serial.print("PM2.5 (Smoke) = ");
    Serial.print(LDS_PM10);
    Serial.print(" ug/m3");
    Serial.println("");
  }

  if (preLDS_PM25 != LDS_PM25) {

    DataToMsg(Dust_PM25, LDS_PM25);

    Serial.print("PM2.5 (Smoke) = ");
    Serial.print(LDS_PM25);
    Serial.print(" ug/m3");
    Serial.println("");
  }

  if (preLDS_PM100 != LDS_PM100) {

    DataToMsg(Dust_PM100, LDS_PM100);

    Serial.print("PM10 (Smoke) = ");
    Serial.print(LDS_PM100);
    Serial.print(" ug/m3");
    Serial.println("");
  }
}

void LDS_Error_Handler() {
  if (dustMsg[27] == 0) {
    // Skip
  }
  else if (dustMsg[27] == 1) {
    PublishMsg(Status, "Dust Sensor: Fan Speed Alarm");
  }
  else if (dustMsg[27] == 2) {
    PublishMsg(Status, "Dust Sensor: Fan Speed Compensation Start");
  }
  else if (dustMsg[27] == 4) {
    PublishMsg(Status, "Dust Sensor: Fan Error");
  }
  else if (dustMsg[27] == 8) {
    PublishMsg(Status, "Dust Sensor: Low Temperature Alarm");
  }
  else if (dustMsg[27] == 16) {
    PublishMsg(Status, "Dust Sensor: High Temperature alarm");
  }
  else if (dustMsg[27] == 32) {
    PublishMsg(Status, "Dust Sensor: Laser Alarm");
  }
  else if (dustMsg[27] == 64) {
    PublishMsg(Status, "Dust Sensor: Laser Error");
  }
}

void LDS() {


  if ( Serial2.read() == 0x42 && Serial2.read() == 0x4D ) {     // When starting bits are detected, start reading

    int msgIdx = 0;
    for ( int i = 0; i < 30; i++ ) {

      while ( !Serial2.available() );

      dustMsg[msgIdx++] = Serial2.read();

    }

    // Save the old data and calculate the new
    preLDS_PM10 = LDS_PM10;
    preLDS_PM25 = LDS_PM25;
    preLDS_PM100 = LDS_PM100;
    
    LDS_PM10 = (dustMsg[2] * 256) + dustMsg[3];
    LDS_PM25 = (dustMsg[4] * 256) + dustMsg[5];
    LDS_PM100 = (dustMsg[6] * 256) + dustMsg[7];

    LDS_Error_Handler();
    LDS_Data_Handler();

  }
}
