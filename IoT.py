import paho.mqtt.client as mqtt
import mariadb, sys

my_list=[]

def add_measurement_to_database(cur,g, t, h, l, d, a):
    cur.execute("INSERT INTO IoT.room_characteristics(GPS_location,Temperature_Celsius,Humidity,Light,Dust,Air_Quality) "
                "VALUES (?,?,?,?,?,?)",(g,t,h,l,d,a))
# The callback for when the client receives a CONNACK response from the server
def on_connect(client, userdata, flags, rc):
    print("Connected with result code "+str(rc))
    client.subscribe("group3/22/room_characteristics")

# The callback for when a PUBLISH message is received from the server
def on_message(client, userdata, msg):
    global my_list
    print(msg.topic+" "+str(msg.payload))
    x = msg.payload.decode('utf-8')
    y=x.strip()
    if len(my_list)<=5:
        my_list.append(y)
        print(my_list)
    else:
        add_measurement_to_database(cur, float(my_list[0]), float(my_list[1]), float(my_list[2]), int(my_list[3]),
                                    my_list[4], float(my_list[5]))
        my_list=[]
        my_list.append(y)




# Connect to MariaDB Platform
try:
    conn = mariadb.connect(
        user="root",
        password="test",
        host="127.0.0.1",
        port=3306,
        database="IoT",
        autocommit=True
    )
except mariadb.Error as e:
    print(f"Error connecting to MariaDB Platform: {e}")
    sys.exit(1)
# Get Cursor
cur = conn.cursor()
client = mqtt.Client()

client.on_connect = on_connect
client.on_message = on_message
client.connect("test.mosquitto.org")
client.loop_forever()