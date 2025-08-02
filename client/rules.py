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
from config_enum import MQTT_Info, THRESOLD


# --- Thông số HiveMQ ---
broker = MQTT_Info.broker
port = MQTT_Info.port
topic = MQTT_Info.topic # sửa nếu topic khác
client_id = MQTT_Info.client_id

# --- Ngưỡng cảnh báo ---
TEMP_THRESHOLD = THRESOLD.TEMP_THRESHOLD  # °C
GAS_THRESHOLD = THRESOLD.GAS_THRESHOLD


def temperature_to_notify(temp):
    if temp > TEMP_THRESHOLD:
        message = f" Cảnh báo! Nhiệt độ hiện tại là {temp}°C (vượt ngưỡng {TEMP_THRESHOLD}°C)"
        asyncio.run_coroutine_threadsafe(send_telegram_alert(message), loop)

def gas_to_notify(gas):
    if gas > GAS_THRESHOLD:
        message = f" Cảnh báo! Khí ga hiện tại là {gas} (vượt ngưỡng {GAS_THRESHOLD})"
        asyncio.run_coroutine_threadsafe(send_telegram_alert(message), loop)

def on_connect(client, userdata, flags, rc):
    if rc == 0:
        print(" Kết nối thành công HiveMQ")
        client.subscribe(topic)
    else:
        print(f" Kết nối thất bại, mã lỗi {rc}")

def on_message(client, userdata, msg):
    try:
        payload = msg.payload.decode()
        data = json.loads(payload)
        temp = data.get('temperature')
        hum = data.get('humidity')
        gas = data.get('gas')

        print(f" Nhiệt độ: {temp}°C, Độ ẩm: {hum}%, Không khí: {gas}")

        temperature_to_notify(temp)
        gas_to_notify(gas)

    except Exception as e:
        pass

def main():
    client = mqtt_client.Client(client_id)
    client.on_connect = on_connect
    client.on_message = on_message

    client.connect(broker, port)
    client.loop_forever()

if __name__ == '__main__':
    main()
