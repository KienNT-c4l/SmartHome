Smart Door Lock với ESP32, LCD, Keypad, Servo & MQTT
📖 Giới thiệu

Dự án này là một hệ thống khóa cửa thông minh sử dụng ESP32 kết hợp:

Màn hình LCD I2C để hiển thị trạng thái.

Keypad 4x4 để nhập mật khẩu.

Servo SG90 để điều khiển khóa.

EEPROM để lưu trữ mật khẩu.

WiFi + MQTT để gửi log mở cửa lên server.

Hệ thống hỗ trợ:

Mở cửa bằng mật khẩu.

Đổi mật khẩu.

Reset mật khẩu về mặc định.

Chặn truy cập nếu nhập sai quá 3 lần.

Gửi thông báo mở cửa qua MQTT.

🛠️ Phần cứng cần chuẩn bị

ESP32

LCD 16x2 I2C (địa chỉ 0x27)

Keypad 4x4

Servo SG90

Nguồn 5V

Dây cắm & Breadboard

📂 Thư viện sử dụng

Cài đặt các thư viện trong Arduino IDE:

#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#include <Keypad.h>
#include <ESP32Servo.h>
#include <EEPROM.h>
#include <WiFi.h>
#include <PubSubClient.h>

⚙️ Cấu hình mạng & MQTT

Thay đổi thông tin mạng WiFi và server MQTT trong code:

const char* ssid = "SSIoT-02";
const char* password_wifi = "SSIoT-02";

const char* mqtt_server = "192.168.72.193";
const int mqtt_port = 1883;

🔑 Mật khẩu mặc định & chế độ đặc biệt

Mật khẩu mặc định: 12345

Đổi mật khẩu: nhập *#01#

Reset mật khẩu: nhập *#02#

🖥️ Chức năng chính
1. Nhập mật khẩu mở cửa

Nếu đúng → mở cửa (servo quay 180°).

Gửi log MQTT: topic /home/door/timelog, payload "open door".

Tự đóng cửa sau 5 giây.

2. Đổi mật khẩu

Nhập *#01#

Nhập mật khẩu mới 2 lần để xác nhận.

Lưu vào EEPROM.

3. Reset mật khẩu

Nhập *#02#

Xác nhận YES để khôi phục 12345.

4. Bảo vệ khi nhập sai

Nhập sai 3 lần liên tiếp → khóa 1 phút.

🖼️ Sơ đồ nối dây
<img width="1001" height="484" alt="image" src="https://github.com/user-attachments/assets/b32206f5-bcbd-4007-842c-4940580f2b80" />
Lưu đồ thuật toán
<img width="359" height="178" alt="image" src="https://github.com/user-attachments/assets/71bc2318-d6bf-4e9d-a63c-77b0d868e28c" />
