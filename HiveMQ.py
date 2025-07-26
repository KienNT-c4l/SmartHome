class HiveMQ_Info:
    # broker = 'broker.hivemq.com'
    # port = 1883
    # topic = 'home/temperature'  # sửa nếu topic khác
    # client_id = 'smarthome-rules'
    TEMP_THRESHOLD = 30.0  # °C
    
    broker = '192.168.1.38'           # ✅ IP của RPi chạy MQTT local
    port = 1883
    topic = '/home/room1/temperature' # ✅ ESP32 đang gửi nhiệt độ vào topic này
    client_id = 'rpi_rules_subscriber'  # ✅ tên khác với ESP32 là được

    