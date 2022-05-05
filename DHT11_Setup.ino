#include <DHT.h>

//Constants

#define DHTTYPE DHT22   // DHT22

DHT dht(DHTPIN, DHTTYPE); // Initialize DHT sensor for normal 16mhz Arduino

//Variables
float hum;  //Stores humidity value
float temp; //Stores temperature value


void setup_DHT(){
  dht.begin();
}

void DHT_read(){
  float converted = 0.00;

    digitalWrite(DHTPWR, HIGH);
    delay(2000);
    
    //Read data and store it to variables hum and temp
    hum = dht.readHumidity();
    temp= dht.readTemperature();

    digitalWrite(DHTPWR, LOW);

    Serial.print("Celsius = ");
    Serial.print(temp);
    Serial.println(" C");

    Serial.print("Humidity =");
    Serial.println(hum);
    
}
