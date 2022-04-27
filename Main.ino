#define UART_GPS 12
#define UART_LDS 13

void setup()
{
  
  //Serial Monitor
  Serial.begin(9600);

    //Dust Serial
  Serial1.begin(9600, SERIAL_8N1, UART_LDS, -1);
  
  //GPS Serial
  Serial2.begin(9600, SERIAL_8N1, UART_GPS, -1);



  setup_wifi();
}


void loop()
{

  check_connect();

  gps_check();
  
  GPS_display();

  LDS();

}
