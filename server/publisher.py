import json
from paho.mqtt import client as mqtt_client
import time
import random

broker = 'broker.hivemq.com'
port = 1883
topic = 'home/temperature'
client_id = 'test-publisher'

def connect_mqtt():
    client = mqtt_client.Client(client_id)
    client.connect(broker, port)
    return client

import random

def fake_temp():
    return round(random.uniform(28, 42), 1)


def publish(client):
    temp_value = fake_temp()  # giá trị bạn muốn test
    payload = json.dumps({"temp": temp_value})
    client.publish(topic, payload)
    print(f" Đã gửi: {payload}")
    
def main():
    client = connect_mqtt()
    while True:
        publish(client)
        time.sleep(2)

if __name__ == '__main__':
    main()
