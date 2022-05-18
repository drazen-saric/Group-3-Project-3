const int PhotoPin = 34; // Pin connected to sensor

float PhotoVal; // Analog value from the sensor

float Photo_read() {
  PhotoVal = analogRead(PhotoPin);

  Serial.println("");
  Serial.print("Raw Photo = ");
  Serial.println(PhotoVal); // the analog reading
  Serial.println("");

  store_Light(PhotoVal);

  //Return used for sleep check
  return PhotoVal;

}
