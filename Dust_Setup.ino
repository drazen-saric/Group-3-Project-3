char dustMsg[30];

int LDS_PM25 = 0;

const char* Dust_PM25 = "Group3/Project3/Dust/PM25";

const char* Status = "Group3/Projec3/Status";

//void LDS_Error_Handler() {
//  if (dustMsg[27] == 0) {
//    // Skip
//  }
//  else if (dustMsg[27] == 1) {
//    PublishMsg(Status, "Dust Sensor: Fan Speed Alarm");
//  }
//  else if (dustMsg[27] == 2) {
//    PublishMsg(Status, "Dust Sensor: Fan Speed Compensation Start");
//  }
//  else if (dustMsg[27] == 4) {
//    PublishMsg(Status, "Dust Sensor: Fan Error");
//  }
//  else if (dustMsg[27] == 8) {
//    PublishMsg(Status, "Dust Sensor: Low Temperature Alarm");
//  }
//  else if (dustMsg[27] == 16) {
//    PublishMsg(Status, "Dust Sensor: High Temperature alarm");
//  }
//  else if (dustMsg[27] == 32) {
//    PublishMsg(Status, "Dust Sensor: Laser Alarm");
//  }
//  else if (dustMsg[27] == 64) {
//    PublishMsg(Status, "Dust Sensor: Laser Error");
//  }
//}

void LDS_read() {


  if ( Serial1.read() == 0x42 && Serial1.read() == 0x4D ) {     // When starting bits are detected, start reading

    int msgIdx = 0;
    for ( int i = 0; i < 30; i++ ) {

      while ( !Serial1.available() );

      dustMsg[msgIdx++] = Serial1.read();

    }

    // Save the old data and calculate the new
    
    LDS_PM25 = (dustMsg[4] * 256) + dustMsg[5];

    Serial.print("PM2.5 = ");
    Serial.print(LDS_PM25);
    Serial.println(" ug/m3");

  }
}
