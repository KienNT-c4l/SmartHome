import sys
import os
sys.path.append(os.path.abspath(os.path.join(os.path.dirname(__file__), '..')))

def get_ipv4_address():
    import socket
    try:
        s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
        s.connect(('8.8.8.8', 80))
        ip_address = s.getsockname()[0]
        s.close()
        return ip_address
    except Exception as e:
        print(f"Error getting IPv4 address: {e}")
        return None

class MQTT_Info:
    broker_main = get_ipv4_address()
    broker_fallback = 'broker.emqx.io'
    port = 1883
    topic_temperature = 'home/kitchen/temperature' 
    topic_humidity = 'home/kitchen/humidity'
    topic_gas = 'home/kitchen/gas'
    client_id = 'smarthome-rules'

class THRESOLD:
    TEMP_THRESHOLD = 40.0  # °C
    GAS_THRESHOLD = 100
    


    