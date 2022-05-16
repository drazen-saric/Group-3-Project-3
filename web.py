import mariadb, sys
from flask import Flask, render_template

heading = ['Room number', 'Temperature','Humidity','Light','Dust','Time']

app = Flask(__name__)

@app.route('/')
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
    datasheet = []
    cur = conn.cursor()
    cur.execute("SELECT Temperature_Celsius, Humidity, Light, Dust, Time_stamps FROM room ORDER BY id DESC LIMIT 10;")
    for m in cur:
        datasheet.append(m)
    return render_template("webface.html", data=datasheet, headings=heading)
app.run(host='0.0.0.0', port=5000)
