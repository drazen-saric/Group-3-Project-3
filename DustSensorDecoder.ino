// Declaration of all necesairy variables
char dustMsg[30];

int Sdust25, preSdust25 = 100;

int Sdust10, preSdust10 = 0;

int Sdust100, preSdust100 = 0;

int Edust25, preEdust25 = 0;

int Edust10, preEdust10 = 0;

int Edust100, preEdust100 = 0;


void setup() {

Serial.begin(115200);

Serial2.begin(9600, SERIAL_8N1, 13, -1);    //Used for reading the UART data

}

void loop() {

if ( Serial2.read() == 0x42 && Serial2.read() == 0x4D ) {     // When starting bits are detected, start reading

  int msgIdx = 0;
  for( int i = 0; i<30; i++ ){

    while( !Serial2.available() );
    
    dustMsg[msgIdx++] = Serial2.read();
    
  }
  
  // Save the old data and calculate the new
  preSdust10 = Sdust10;
  preSdust25 = Sdust25;
  preSdust100 = Sdust100;
  Sdust10 = (dustMsg[2] * 256) + dustMsg[3];
  Sdust25 = (dustMsg[4] * 256) + dustMsg[5];
  Sdust100 = (dustMsg[6] * 256) + dustMsg[7];

  preEdust10 = Edust10;
  preEdust25 = Edust25;
  preEdust100 = Edust100;
  Edust10 = (dustMsg[8] * 256) + dustMsg[9];
  Edust25 = (dustMsg[10] * 256) + dustMsg[11];
  Edust100 = (dustMsg[12] * 256) + dustMsg[13];

  // Error message handler
  if (dustMsg[27] == 0){
    // Skip
  }
  else if (dustMsg[27] == 1){
    Serial.print("Error detected: Fan Speed Alarm");
    Serial.println("");
    Serial.println("");
  }
  else if (dustMsg[27] == 2){
    Serial.print("Error detected: Fan Speed Compensation Start");
    Serial.println("");
    Serial.println("");
  }
  else if (dustMsg[27] == 4){
    Serial.print("Error detected: Fan Error");
    Serial.println("");
    Serial.println("");
  }
  else if (dustMsg[27] == 8){
    Serial.print("Error detected: Low Temperature Alarm");
    Serial.println("");
    Serial.println("");
  }
  else if (dustMsg[27] == 16){
    Serial.print("Error detected: High Temperature alarm");
    Serial.println("");
    Serial.println("");
  }
  else if (dustMsg[27] ==32){
    Serial.print("Error detected: Laser Alarm");
    Serial.println("");
    Serial.println("");
  }
  else if (dustMsg[27] == 64){
    Serial.print("Error detected: Laser Error");
    Serial.println("");
    Serial.println("");
  }

  // Dust and Smoke message handler
  if (preSdust25 != Sdust25){
    Serial.print("PM2.5 (Smoke) = ");
    Serial.print(Sdust25);
    Serial.print(" ug/m3");
    Serial.println("");
    Serial.println("");
  }

  if (preSdust10 != Sdust10){
    Serial.print("PM1 (Smoke) = ");
    Serial.print(Sdust10);
    Serial.print(" ug/m3");
    Serial.println("");
    Serial.println("");
    }

  if (preSdust100 != Sdust100){
    Serial.print("PM10 (Smoke) = ");
    Serial.print(Sdust100);
    Serial.print(" ug/m3");
    Serial.println("");
    Serial.println("");
  }


  if (preEdust25 != Edust25){
    Serial.print("PM2.5 (Environment) = ");
    Serial.print(Edust25);
    Serial.print(" ug/m3");
    Serial.println("");
    Serial.println("");
  }

  if (preEdust10 != Edust10){
    Serial.print("PM1 (Environment) = ");
    Serial.print(Edust10);
    Serial.print(" ug/m3");
    Serial.println("");
    Serial.println("");
  }

  if (preEdust100 != Edust100){
    Serial.print("PM10 (Environment) = ");
    Serial.print(Edust100);
    Serial.print(" ug/m3");
    Serial.println("");
    Serial.println("");
  }

}

}
