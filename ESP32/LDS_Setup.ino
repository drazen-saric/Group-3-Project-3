char dustMsg[30];

int LDS_PM25 = 0;

void LDS_read() {

  if ( Serial1.read() == 0x42 && Serial1.read() == 0x4D ) {     // When starting bits are detected, start reading

    int msgIdx = 0;
    for ( int i = 0; i < 30; i++ ) {

      while ( !Serial1.available() );

      dustMsg[msgIdx++] = Serial1.read();

    }

    //Calculate dust value
    LDS_PM25 = (dustMsg[4] * 256) + dustMsg[5];

    Serial.println("");
    Serial.print("PM2.5 = ");
    Serial.print(LDS_PM25);
    Serial.println(" ug/m3");
    Serial.println("");

    //Store value to local storage
    store_PM25(LDS_PM25);

  }
}
