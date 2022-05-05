const int PhotoPin = 34; // Pin connected to sensor

int PhotoVal; // Analog value from the sensor

void Photo_read(){
  PhotoVal = analogRead(PhotoPin);
  
  Serial.print("Raw Photo = ");
  Serial.println(PhotoVal); // the analog reading
  
}
