# SmartHome - IoT Smart Home System

## 📌 Giới thiệu

SmartHome là hệ thống nhà thông minh mô phỏng bằng ESP32 và các thiết bị LED/motor, với các tính năng:

- Giám sát nhiệt độ và độ ẩm (room1) theo thời gian thực trong các phòng (living room, bedroom…) bằng cảm biến DHT11.

- Giám sát bếp (kitchen) với cảm biến DHT11 và cảm biến gas để cảnh báo cháy/rò rỉ gas. Khi nhiệt độ hoặc gas vượt mức cho phép, hệ thống gửi thông báo Telegram.

- Điều khiển thiết bị mô phỏng: LED đại diện đèn, motor đại diện quạt.

- Mở cửa bằng nhập pass (module door), không sử dụng cảm biến PIR hiện tại.

Hệ thống thể hiện khả năng thiết kế IoT mô phỏng, phát triển firmware ESP32, tích hợp MQTT, quản lý database MariaDB, và hiển thị/điều khiển qua OpenHAB.
Chi tiết về đề tài tại:[wiki page](https://github.com/KienNT-c4l/SmartHome/wiki) 

![Mô hình hệ thống SmartHome](https://github.com/KienNT-c4l/SmartHome/blob/main/assets/demo1.png)



## 🚀 Tính năng

- **room1 module (living room, bedroom, …):**
  - DHT11: đo nhiệt độ & độ ẩm.
  - Điều khiển LED (đèn) và motor (quạt) dựa trên điều kiện nhiệt độ/độ ẩm.

- **kitchen module:**
  - DHT11: đo nhiệt độ & độ ẩm.
  - Cảm biến gas: phát hiện rò rỉ gas.
  - Cảnh báo Telegram khi nhiệt độ/gas vượt ngưỡng quy định.

- **Door module:**
  - Mở cửa bằng passcode, mô phỏng bảo mật.

- **Database:** MariaDB lưu trữ dữ liệu cảm biến và trạng thái thiết bị.

- **Front-end:** OpenHAB để hiển thị trạng thái và điều khiển thiết bị mô phỏng.



## 🛠️ Công nghệ sử dụng

- **Phần cứng:** ESP32, Raspberry Pi 5, DHT11, cảm biến gas, LED, motor
- **Firmware:** Arduino IDE
- **Giao thức:** MQTT
- **Server & DB:** Raspberry Pi 5, Python, MariaDB
- **Front-end:** OpenHAB


## 📂 Cấu trúc dự án
```text
SmartHome/
├── Door/                  # Module door: mở cửa bằng passcode
├── kitchen/               # Module kitchen: DHT11 + gas sensor + Telegram alert
├── room1/                 # Module room: DHT11 + LED/motor mô phỏng phòng
├── database/              # Quản lý dữ liệu cảm biến và trạng thái thiết bị
├── openHAB/               # Cấu hình OpenHAB để visualize & điều khiển
├── assets/                # Hình ảnh, GIF, video demo
└── README.md
```


## 🔧 Hướng dẫn triển khai

1. **Cài đặt môi trường Python**  

2. **Thiết lập MariaDB**  
   - Tạo database và bảng cần thiết cho lưu trữ dữ liệu cảm biến và trạng thái thiết bị.  
   - Cấu hình thông tin kết nối trong script Python.  

3. **Firmware ESP32**  
   - Flash ESP32 với firmware tương ứng (Arduino IDE).  
   - ESP32 gửi dữ liệu DHT11 và gas sensor qua MQTT tới server Raspberry Pi.  

4. **Chạy MQTT-to-DB script**  
```bash
cd database
python3 mqtt_to_db.py
```
Script nhận dữ liệu MQTT từ ESP32 và lưu vào MariaDB theo thời gian thực.
5. **Visualize & điều khiển**
   - Cấu hình OpenHAB kết nối với MQTT broker và MariaDB.
   - Tạo dashboard hiển thị: nhiệt độ, độ ẩm, trạng thái gas, LED, motor.
   - Người dùng có thể điều khiển đèn/quạt mô phỏng trực tiếp từ OpenHAB.


## 🤝 Đóng góp

Hoan nghênh các pull request để:

- Thêm module mới (sensor, actuator).
- Tối ưu database, cải thiện giao diện OpenHAB.
- Mở rộng chức năng cảnh báo qua Telegram.


