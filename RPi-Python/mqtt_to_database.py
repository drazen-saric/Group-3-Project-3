import paho.mqtt.client as mqtt
import mariadb, sys

my_list=[] #dynamic list
i=0
#insert data in database function
def add_measurement_to_database(cur,room, temp, hum, light, dust, times):
    cur.execute("INSERT INTO IoT.room(Room_number,Temperature_Celsius,Humidity,Light,Dust,Time_stamps) "
                "VALUES (?,?,?,?,?,?)",(room,temp,hum,light,dust,times))

#check database for room number based on MAC address
def check_database(cur,m):
    cur.execute("SELECT CLASS FROM MAC WHERE Mac = '" + m + "';")

    result = cur.fetchone()
    result_str = str(result)
    result_decode = result_str.strip("(',')")
    print(result)

    if result == None:
        client.publish("group3/22/room_characteristics", "None")
    else:
        client.publish("group3/22/room_characteristics", result_decode)


#subscribe to topic mqtt
def on_connect(client, userdata, flags, rc):

    client.subscribe("group3/22/room_characteristics")
    client.subscribe("group3/22/mac")
    client.publish("group3/22/sync", "0")

#callback function when message is received/data list append
def on_message(client, userdata, msg):
    topic1 = "group3/22/room_characteristics"
    topic2 = "group3/22/mac"

    if msg.topic == topic1:
        global my_list
        print(msg.topic+" "+str(msg.payload))
        msg_nondecode = msg.payload.decode('utf-8')
        msg_strip=msg_nondecode.strip()
        if len(my_list)<=5:
            my_list.append(msg_strip)
            print(my_list)
        else:
            add_measurement_to_database(cur, my_list[5], my_list[0] + ' °C', my_list[1] + ' %', my_list[2], my_list[3] + ' P/M',
                                        my_list[4])
            my_list = []
            my_list.append(msg_strip)
    if msg.topic == topic2:
        msg_nonstrip = msg.payload.decode('utf-8')
        msg_strip = msg_nonstrip.strip()
        check_database(cur, msg_strip)


#mariadb connect
try:
    conn = mariadb.connect(
        user="root",
        password="1111",
        host="localhost",
        port=3306,
        database="IoT",
        autocommit=True
    )
except mariadb.Error as e:
    print(f"Error connecting to MariaDB Platform: {e}")
    sys.exit(1)

#get cursor and mqtt connect
cur = conn.cursor()
client = mqtt.Client()
client.on_connect = on_connect
client.on_message = on_message
client.connect("10.120.0.68", port=1883)
while True:
    client.loop()
