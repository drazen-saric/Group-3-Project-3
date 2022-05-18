//Lists used for local storage
int PM25_Values[100] = {};
float Light_Values[100] = {};
float Temp_Values[100] = {};
float Humid_Values[100] = {};

//2D lists, 100 values of a certain length
char MAC_Values[100][18] = {};
char Time_Values[100][5] = {};

const char* Data_Topic = "group3/22/room_characteristics";
const char* MAC_Topic = "group3/22/mac";

//Counters used for sending the data in a proper order
int pointer = 0;
int round_count = 0;

//Functions store the measured values
void store_PM25(float Data) {
  PM25_Values[pointer] = Data;
}

void store_Light(float Data) {
  Light_Values[pointer] = Data;
}

void store_Temp(float Data) {
  Temp_Values[pointer] = Data;
}

void store_Humid(float Data) {
  Humid_Values[pointer] = Data;
}

void store_MAC(char* Data) {
  strcpy(MAC_Values[pointer], Data);
}

void store_Time(char* Data) {
  strcpy(Time_Values[pointer], Data);
}

//If the local storage fills up, it starts overwriting data from index 0
//When the data needs to be uploaded, it starts uploading from index 99 (round_count)
void increment_pointer() {
  if (pointer >= 99) {
    pointer = 0;
    round_count = 1;
  }
  else {
    pointer += 1;
  }
}

void data_send() {
  if (round_count == 1) {
    pointer = 99;
  }

  while (pointer != -1) {

    DataToMsg(Data_Topic, Temp_Values[pointer]);
    DataToMsg(Data_Topic, Humid_Values[pointer]);
    DataToMsg(Data_Topic, Light_Values[pointer]);
    DataToMsg(Data_Topic, PM25_Values[pointer]);
    PublishMsg(Data_Topic, Time_Values[pointer]);
    PublishMsg(MAC_Topic, MAC_Values[pointer]);

    Serial.print("Sent data on pointer nr. ");
    Serial.println(pointer);

    pointer -= 1;

  }
  //Set pointer and round_count to 0
  increment_pointer();
  round_count = 0;
  Serial.println("Complete send!");
}
