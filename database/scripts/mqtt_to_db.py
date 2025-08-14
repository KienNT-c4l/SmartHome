# Version5: Ghi log sensor + door_log
import paho.mqtt.client as mqtt
import mysql.connector

# Kết nối MySQL
db = mysql.connector.connect(
    host="localhost",
    user="root",
    password="pi",
    database="smarthome"
)
cursor = db.cursor()

# Lưu dữ liệu mới nhất của từng phòng
latest_data_per_room = {}

# Hàm tạo hoặc lấy room_id theo tên phòng
def get_or_create_room_id(name):
    cursor.execute("SELECT id FROM rooms WHERE name = %s", (name,))
    result = cursor.fetchone()
    if result:
        return result[0]

    cursor.execute("INSERT INTO rooms (name) VALUES (%s)", (name,))
    db.commit()

    cursor.execute("SELECT id FROM rooms WHERE name = %s", (name,))
    result = cursor.fetchone()
    return result[0]

# Khi kết nối MQTT thành công
def on_connect(client, userdata, flags, rc):
    print("[MQTT] Connected with result code", rc)
    client.subscribe("/home/+/temperature")
    client.subscribe("/home/+/humidity")
    client.subscribe("/home/+/gas")
    client.subscribe("/home/+/motion")
    client.subscribe("/home/door/timelog")  # Topic ghi log cửa

# Khi nhận được dữ liệu MQTT
def on_message(client, userdata, msg):
    topic = msg.topic
    payload = msg.payload.decode()

    try:
        parts = topic.split('/')

        # Ghi log cửa riêng nếu topic là /home/door/timelog
        if topic == "/home/door/timelog":
            room_name = "door"  # hoặc "main_gate" nếu bạn dùng riêng
            door_status = payload.strip().lower()
            valid_statuses = ["open", "closed", "open door"]

            # Nếu cần, chuyển "open door" → "open"
            if door_status == "open door":
                door_status = "open"

            if door_status in ["open", "closed"]:
                room_id = get_or_create_room_id(room_name)
                print(f"[DOOR] Room: {room_name} | Door status: {door_status}")
                cursor.execute("""
                    INSERT INTO door_log (room_id, door_status)
                    VALUES (%s, %s)
                """, (room_id, door_status))
                db.commit()
            else:
                print(f"[WARN] Unknown door status: '{payload}'")
            return  # kết thúc xử lý

        # Cảm biến môi trường
        room_name = parts[2]
        data_type = parts[3]
        room_id = get_or_create_room_id(room_name)

        if data_type in ["temperature", "humidity", "gas", "motion"]:
            if room_id not in latest_data_per_room:
                latest_data_per_room[room_id] = {
                    "temperature": None,
                    "humidity": None,
                    "gas_level": None,
                    "motion_detected": None
                }

            if data_type == "temperature":
                latest_data_per_room[room_id]["temperature"] = float(payload)
            elif data_type == "humidity":
                latest_data_per_room[room_id]["humidity"] = float(payload)
            elif data_type == "gas":
                latest_data_per_room[room_id]["gas_level"] = float(payload)
            elif data_type == "motion":
                latest_data_per_room[room_id]["motion_detected"] = bool(int(payload))

            data = latest_data_per_room[room_id]
            if data["temperature"] is not None and data["humidity"] is not None:
                print(f"[DATA] Room: {room_name} | Temp: {data['temperature']}, Humi: {data['humidity']}, "
                      f"Gas: {data['gas_level']}, Motion: {data['motion_detected']}")

                cursor.execute("""
                    INSERT INTO sensor_data (room_id, temperature, humidity, gas_level, motion_detected)
                    VALUES (%s, %s, %s, %s, %s)
                """, (
                    room_id,
                    data["temperature"],
                    data["humidity"],
                    data["gas_level"],
                    data["motion_detected"]
                ))
                db.commit()

                # Reset sau khi ghi xong
                latest_data_per_room[room_id] = {
                    "temperature": None,
                    "humidity": None,
                    "gas_level": None,
                    "motion_detected": None
                }

    except Exception as e:
        print(f"[ERROR] {e}")

# MQTT khởi tạo
client = mqtt.Client()
client.on_connect = on_connect
client.on_message = on_message
client.connect("localhost", 1883, 60)

client.loop_forever()

