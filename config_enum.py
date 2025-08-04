import sys
import os
sys.path.append(os.path.abspath(os.path.join(os.path.dirname(__file__), '..')))

class MQTT_Info:
    broker = "broker.emqx.io"
    port = 1883
    topic_temperature = 'home/kitchen/temperature' 
    topic_humidity = 'home/kitchen/humidity'
    topic_gas = 'home/kitchen/gas'
    client_id = 'smarthome-rules'

class THRESOLD:
    TEMP_THRESHOLD = 40.0  # °C
    GAS_THRESHOLD = 600


    