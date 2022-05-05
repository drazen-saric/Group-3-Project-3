#define UART_LDS 32

#define DHTPIN 33
#define DHTPWR 19


void setup()
{
  
  //Serial Monitor
  Serial.begin(115200);

    //Dust Serial
  Serial1.begin(9600, SERIAL_8N1, UART_LDS, -1);

  pinMode(DHTPIN, INPUT);
  pinMode(DHTPWR, OUTPUT);

  pinMode(32, INPUT);

  setup_DHT();

  setup_wifi();
}


void loop()
{

  check_connect();
  delay(1000);

  //LDS_read();
  //delay(1000);

  //DHT_read();
  //delay(1000);

  Photo_read();
  delay(1000);

}
