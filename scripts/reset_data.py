# scripts/clear_data.py
import mysql.connector

conn = mysql.connector.connect(
    host="localhost",
    user="root",
    password="pi",
    database="smarthome"
)
cursor = conn.cursor()

with open("/home/pi/Project/SmartHome/database/schema/reset_data.sql", "r") as f:
    sql_commands = f.read().split(';')

for command in sql_commands:
    command = command.strip()
    if command:
        cursor.execute(command)

conn.commit()
conn.close()
print("Sample data cleared.")
