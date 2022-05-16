#define UART_LDS 32

#define DHTPIN 33

//Constants
//In Minutes
const int CycleTime = 5;
const int SleepTime = 20;

//In Seconds
const int MesureDelay = 2;

const int LightLimit = 150;


void setup() {
  //Serial Monitor
  Serial.begin(115200);

  //LDS Serial
  Serial1.begin(9600, SERIAL_8N1, UART_LDS, -1);

  //DHT11
  pinMode(DHTPIN, INPUT);

  //Deep sleep timer
  esp_sleep_enable_timer_wakeup(SleepTime * 60 * 1000 * 1000);

  connect_wifi();

  setup_Time();

  setup_DHT();

}

void loop() {
  //If light below limit, sleep for SleepTime
  if (Photo_read() < LightLimit){
    Serial.print("Not good light conditions, enetring deep sleep (");
    Serial.print(SleepTime);
    Serial.print(" min)");
    esp_deep_sleep_start();
  }
  else {
    Serial.println("Good light conditions, getting values...");
  }

  delay(MesureDelay * 1000);

  get_Time();

  delay(MesureDelay * 1000);

  LDS_read();

  delay(MesureDelay * 1000);

  DHT_read();

  delay(MesureDelay * 1000);

  read_MAC();

  delay(MesureDelay * 1000);

  //When all mesurements are collected, WiFi connection is confirmed and the data send is initiated
  check_wifi();

  Serial.println("Cycle complete, waiting 5m before next cycle...");
  //delay(CycleTime * 60 * 1000);
  delay(10000);




}
