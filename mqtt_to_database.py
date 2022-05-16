import paho.mqtt.client as mqtt
import mariadb, sys

my_list=[]
i=0
def add_measurement_to_database(cur, room, temp, hum, light, dust, times):
    cur.execute("INSERT INTO IoT.room(GPS_location,Temperature_Celsius,Humidity,Light,Dust,Air_Quality) "
                "VALUES (?,?,?,?,?,?)",(room,temp,hum,light,dust,times))

def check_database(cur,m):
    cur.execute("SELECT CLASS FROM MAC WHERE Mac = '" + m + "';")

    result = cur.fetchone()
    result_str = str(result)
    result_decode = result_str.strip("(',')")
    print(result)

    if result == None:
        client.publish("group3/22/room_characteristics", "Class not found. Update Database. ")
    else:
        client.publish("group3/22/room_characteristics", result_decode)



def on_connect(client, userdata, flags, rc):

    client.subscribe("group3/22/room_characteristics")
    client.subscribe("group3/22/mac")


# The callback for when a PUBLISH message is received from the server
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
            add_measurement_to_database(cur, my_list[5], my_list[0], my_list[1] + ' °C', my_list[2] + ' %', my_list[3],
                                        my_list[4] + ' pm')
            my_list = []
            my_list.append(msg_strip)
    if msg.topic == topic2:
        msg_nonstrip = msg.payload.decode('utf-8')
        msg_strip = msg_nonstrip.strip()
        check_database(cur, msg_strip)


# Connect to MariaDB Platform
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
# Get Cursor
cur = conn.cursor()
client = mqtt.Client()
client.on_connect = on_connect
client.on_message = on_message
client.connect("test.mosquitto.org")
client.loop_forever()