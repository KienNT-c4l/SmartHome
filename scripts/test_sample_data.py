import mysql.connector

# Connect to MariaDB
conn = mysql.connector.connect(
    host="localhost",
    user="root",
    password="pi",
    database="smarthome"
)
cursor = conn.cursor()

with open("../schema/sample_data.sql", "r") as f:
    sql_commands = f.read()
    for command in sql_commands.split(";"):
        command = command.strip()
        if command:
            cursor.execute(command)

conn.commit()
print("Data was successfully inserted")

# Select data from sensor_data
cursor.execute("SELECT * FROM sensor_data")
rows = cursor.fetchall()

# Visualize the data
print("\nData from sensor_data:")
for row in rows:
    print(row)

# Stop the connection
cursor.close()
conn.close()
