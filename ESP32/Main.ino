#define UART_LDS 32

#define DHTPIN 33

//Constants
//In Minutes
const int CycleTime = 5;
const int SleepTime = 20;

//In Seconds
const int MesureDelay = 2;

//Minimum light level for Sleep Mode
const int LightLimit = 150;

//Used for syncing with the database
int SyncVal = 0;


void setup() {
  //Serial Monitor
  Serial.begin(115200);

  //LDS Serial
  Serial1.begin(9600, SERIAL_8N1, UART_LDS, -1);

  //DHT11
  pinMode(DHTPIN, INPUT);

  //Sleep timer
  esp_sleep_enable_timer_wakeup(SleepTime * 60 * 1000 * 1000);

  connect_wifi();

  ////Used for syncing with the database
  connect_mqtt_initial();

  setup_Time();

  setup_DHT();

}

void loop() {
  //Waiting for database to send a "ready to recieve" message
  if ( SyncVal != 1 ) {
    client_loop();
  }
  else {
    Serial.println("Synchronization Success!");

    //If light below limit, sleep for SleepTime
    if (Photo_read() < LightLimit) {
      Serial.print("Bad light conditions, enetring sleep mode(");
      Serial.print(SleepTime);
      Serial.print(" min)");
      delay(1000);

      PublishMsg("group3/22/sync", "Bad light conditions, entering sleep mode (20 min)");
      delay(1000);

      esp_light_sleep_start();

      PublishMsg("group3/22/sync", "Exited sleep mode");

    }
    else {
      Serial.println("Good light conditions, getting values...");
      PublishMsg("group3/22/sync", "Good light conditions, getting values...");
      Serial.println("");

      delay(MesureDelay * 1000);

      get_Time();

      delay(MesureDelay * 1000);

      LDS_read();

      delay(MesureDelay * 1000);

      DHT_read();

      delay(MesureDelay * 1000);

      read_MAC();

      delay(MesureDelay * 1000);

      check_wifi();

      Serial.println("Cycle complete, waiting 5m before next cycle...");
      PublishMsg("group3/22/sync", "Cycle complete, waiting 5m before next cycle...");

      delay(CycleTime * 60 * 1000);

    }


  }


}
