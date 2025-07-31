import mysql.connector

conn = mysql.connector.connect(
    host = "localhost",
    user = "root",
    password = "mobius",
    database = "smarthome"
)
cursor = conn.cursor()

for script in ["schema/create_tables.sql", "schema/sample_data.sql"]:
    with open(script, "r") as f:
        cursor.execute(f.read(), multi = True)

conn.commit()
conn.close()