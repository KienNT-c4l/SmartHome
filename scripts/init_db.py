import mysql.connector

# Connect to MariaDB
conn = mysql.connector.connect(
    host="localhost",
    user="root",
    password="pi",
    database="smarthome"
)

cursor = conn.cursor()

for script_path in ["/home/pi/Project/SmartHome/database/schema/create_table.sql", "/home/pi/Project/SmartHome/database/schema/config_data.sql"]:
    with open(script_path, "r") as f:
        sql_script = f.read()

        statements = sql_script.split(';')
        for statement in statements:
            statement = statement.strip()
            if statement:
                try:
                    cursor.execute(statement)
                except mysql.connector.Error as err:
                    print(f"[ERROR] Unable to execute the command: {statement}")
                    print(err)

conn.commit()
conn.close()
print("Database initialization completed.")
