#include <DHT.h>

#define DHTTYPE DHT22   // DHT22

//Value variables
float hum = 0.0;  //Stores humidity value
float temp = 0.0; //Stores temperature value

DHT dht(DHTPIN, DHTTYPE);

void setup_DHT() {
  dht.begin();
}

void DHT_read() {
  float converted = 0.00;

  //Read sensor and store it to variables hum and temp
  hum = dht.readHumidity();
  temp = dht.readTemperature();


  Serial.print("Temperature = ");
  Serial.print(temp);
  Serial.println(" C");
  Serial.println("");

  Serial.print("Humidity = ");
  Serial.println(hum);
  Serial.println("");

  store_Temp(temp);
  store_Humid(hum);

}

