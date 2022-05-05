#include <WiFi.h>
#include <PubSubClient.h>

const char* SSID = "ITEK 2nd";
const char* Password = "Four_Sprints_F21v";
const char* MQTT_Server = "test.mosquitto.org";

WiFiClient espClient;
PubSubClient client(espClient);

long lastMsg = 0;
char msg[50];
int value = 0;


void setup_wifi() { // Sets up initial WiFi and provides feedback to the user
  delay(10);
  Serial.print("Connecting to ");
  Serial.println(SSID);

  WiFi.begin(SSID, Password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  randomSeed(micros());

  Serial.println("WiFi connected!");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  //Wait for connect
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);

    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");

  //MQTT Setup
  client.setServer(MQTT_Server, 1883);

  check_connect();
}


void check_connect() {
  if (!client.connected()) {

    reconnect();

  }
}


void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");

    String clientId = "ESP8266Client-"; // Create a random client ID
    clientId += String(random(0xffff), HEX);

    if (client.connect(clientId.c_str())) {
      Serial.println("Connected!");

    }
    else {
      Serial.print("Failed reconnect, ERR_CODE = ");
      Serial.print(client.state());
      Serial.println("Trying again in 5 Seconds...");
      delay(5000);
    }
  }
}


void DataToMsg(const char* Topic, float Value) {

  String temp = String(Value);

  char Pub_Msg[temp.length() + 1];

  temp.toCharArray(Pub_Msg, temp.length() + 1);

  client.publish(Topic, Pub_Msg);
}

void PublishMsg(const char* Topic, const char* Msg) {
  client.publish(Topic, Msg);
}
