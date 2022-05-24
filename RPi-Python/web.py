import mariadb
import  sys
from flask import Flask, render_template

#static list of table headings
heading = ['Room Number', 'Temperature','Humidity','Light','Dust','Time']

app = Flask(__name__)

@app.route('/')
#mariadb connect
def dyna():
    try:
        conn = mariadb.connect(
            user="root",
            password="1111",
            host="127.0.0.1",
            port=3306,
            database="IoT"
        )
    except mariadb.Error as e:
        print(f"Error connecting to MariaDB Platform: {e}")
        sys.exit(1)

#dynamic list of table data
    datasheet = []
#query and appending to list
    cur = conn.cursor()
    cur.execute("SELECT Room_number, Temperature_Celsius, Humidity, Light, Dust, Time_stamps FROM room ORDER BY id DESC LIMIT 10;")
    for m in cur:
        datasheet.append(m)
#render html template with list attributes
    return render_template("webface.html", data=datasheet, headings=heading)
#run app
app.run(host='0.0.0.0', port=5000)
