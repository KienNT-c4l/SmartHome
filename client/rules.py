import json
from paho.mqtt import client as mqtt_client

import sys
import os
sys.path.append(os.path.abspath(os.path.join(os.path.dirname(__file__), '..')))

import asyncio
import threading
loop = asyncio.new_event_loop()
asyncio_thread = threading.Thread(target=loop.run_forever, daemon=True)
asyncio_thread.start()

from server.notify import send_telegram_alert
from HiveMQ import HiveMQ_Info


# --- Thông số HiveMQ ---
broker = HiveMQ_Info.broker
port = HiveMQ_Info.port
topic = HiveMQ_Info.topic # sửa nếu topic khác
client_id = HiveMQ_Info.client_id

# --- Ngưỡng cảnh báo ---
TEMP_THRESHOLD = HiveMQ_Info.TEMP_THRESHOLD

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
        temp = data.get("temperature", 'N/A')  # nếu không có, mặc định là "N/A"
        hum = data.get("humidity", "N/A")  # nếu không có, mặc định là "N/A"

        print(f" Nhiệt độ: {temp}°C, độ ẩm: {hum}%")

        if temp > TEMP_THRESHOLD:
            message = f" Cảnh báo! Nhiệt độ hiện tại là {temp}°C (vượt ngưỡng {TEMP_THRESHOLD}°C)"
            asyncio.run_coroutine_threadsafe(send_telegram_alert(message), loop)

    except Exception as e:
        print(f" Lỗi khi xử lý dữ liệu: {e}")

def main():
    client = mqtt_client.Client(client_id)
    client.on_connect = on_connect
    client.on_message = on_message

    client.connect(broker, port)
    client.loop_forever()

if __name__ == '__main__':
    main()
