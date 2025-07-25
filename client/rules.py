import json
from paho.mqtt import client as mqtt_client
import sys
import os
sys.path.append(os.path.abspath(os.path.join(os.path.dirname(__file__), '..')))
from server.notify import send_telegram_alert as sta

# --- Thông số HiveMQ ---
broker = 'broker.hivemq.com'
port = 1883
topic = 'home/temperature'  # sửa nếu topic khác
client_id = 'smarthome-rules'

# --- Ngưỡng cảnh báo ---
TEMP_THRESHOLD = 30.0  # °C

def on_connect(client, userdata, flags, rc):
    if rc == 0:
        print(" Kết nối thành công HiveMQ")
        client.subscribe(topic)
    else:
        print(f" Kết nối thất bại, mã lỗi {rc}")

def on_message(client, userdata, msg):
    try:
        payload = msg.payload.decode()
        data = json.loads(payload)  # ví dụ: {"temp": 31.2}

        temperature = data.get("temp")

        print(f" Nhiệt độ nhận được: {temperature}°C")

        if temperature > TEMP_THRESHOLD:
            message = f" Cảnh báo! Nhiệt độ hiện tại là {temperature}°C (vượt ngưỡng {TEMP_THRESHOLD}°C)"
            sta(message)

    except Exception as e:
        print(f" Lỗi khi xử lý dữ liệu: {e}")

def run():
    client = mqtt_client.Client(client_id)
    client.on_connect = on_connect
    client.on_message = on_message

    client.connect(broker, port)
    client.loop_forever()

if __name__ == '__main__':
    run()
