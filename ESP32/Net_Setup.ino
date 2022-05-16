#include <WiFi.h>
#include <PubSubClient.h>
#include <Husarnet.h>
#include <HusarnetClient.h>
#include <HusarnetServer.h>

//Wifi Credentials
const char* SSID = "ITEK 2nd";
const char* Password = "Four_Sprints_F21v";

const char* MQTT_Server = "broker.emqx.io";  //CHANGE TO LOCAL BROKER!!!

//VPN Credentials
const char* hostName = "robo_bitch_32";
const char* husarnetJoinCode = "fc94:b01d:1803:8dd8:b293:5c7d:7639:932a/dNAJvmgffPMucFjJWXjd4t";
const char* dashboardURL = "default";

WiFiClient espClient;
PubSubClient client(espClient);

int Strongest = 90;

void setup_vpn() {
  Husarnet.selfHostedSetup(dashboardURL);
  Husarnet.join(husarnetJoinCode, hostName);
  Husarnet.start();
}

void connect_wifi() { // Connects WiFi and MQTT
  delay(10);
  Serial.print("Attempting WiFi connection to ");
  Serial.println(SSID);

  WiFi.begin(SSID, Password);

  unsigned long TStamp = millis();

  while (WiFi.status() != WL_CONNECTED) {
    delay(200);
    Serial.print(".");
    if (millis() - TStamp > 5000) {
      break;
    }
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("WiFi connected!");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());

    Serial.println("Connecting to VPN...");
    setup_vpn();
    delay(5000);

    //MQTT Setup
    client.setServer(MQTT_Server, 1883);

  }
  else {
    Serial.println("Wifi unavailable!");
  }

}

void check_wifi() {
  if ((WiFi.status() != WL_CONNECTED)) {
    connect_wifi();
    Serial.println("Wifi still unavailable, incrementing local storage...");
    increment_pointer();
  }
  else {
    Serial.println("Wifi available, checking MQTT connection...");
    check_mqtt();
  }
}

void check_mqtt() {
  if (!client.connected()) {

    connect_mqtt();

  }
  else {
    Serial.println("");
    Serial.println("MQTT Connnected, sending data...");
    data_send();
  }
}


void connect_mqtt() {
  int counter = 0;
  while (!client.connected()) {
    Serial.println("Attempting MQTT connection...");

    String clientId = "ESP8266Client-"; // Create a random client ID
    clientId += String(random(0xffff), HEX);

    if (client.connect(clientId.c_str())) {
      Serial.println("Connected! Sending data...");

      data_send();

    }
    else {
      Serial.print("Failed reconnect, ERR_CODE = ");
      Serial.print(client.state());
      Serial.println("Trying again in 2 Seconds...");
      delay(2000);

      counter += 1;

      if (counter == 2) {
        counter = 0;
        Serial.println("MQTT connect failed after 3 attempts, switching to local storage");
        increment_pointer();
        break;
      }
    }
  }
}

void read_MAC() {
  int NumOfNets = WiFi.scanNetworks();

  if ((WiFi.status() != WL_CONNECTED)) {
    String temp = "Not Available";
    char Mac_Addr[temp.length() + 1];
    temp.toCharArray(Mac_Addr, temp.length() + 1);

    Serial.println("Closest AP is Not Available");

    store_MAC(Mac_Addr);
  }

  else {
    for (int Signal = 0; Signal < NumOfNets; ++Signal) {

      if (WiFi.SSID(Signal) == SSID)  {

        if (Signal <= Strongest) {
          Strongest = Signal;
          int Signal_Strength = WiFi.RSSI(Strongest);

          String temp = WiFi.BSSIDstr(Strongest);
          char Mac_Addr[temp.length() + 1];
          temp.toCharArray(Mac_Addr, temp.length() + 1);

          Serial.print("Closest AP is ");
          Serial.println(WiFi.BSSIDstr(Strongest));
          Serial.print("With a signal of ");
          Serial.println(WiFi.RSSI(Strongest));

          store_MAC(Mac_Addr);

        }
      }
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