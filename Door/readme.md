# Hệ thống mở cửa bằng bàn phím ma trận, LCD, Servo và MQTT (ESP32)

## 📌 Giới thiệu
Dự án này sử dụng **ESP32** để điều khiển servo mở/đóng cửa dựa trên mật khẩu nhập từ bàn phím ma trận.  
Ngoài ra hệ thống có:
- **LCD I2C** hiển thị trạng thái
- Chức năng **đổi mật khẩu** và **reset mật khẩu về mặc định**
- Kết nối **WiFi + MQTT** để gửi log mở cửa lên server
- **EEPROM** để lưu mật khẩu

## ⚙️ Phần cứng
- ESP32 DevKit
- Servo SG90
- LCD 16x2 I2C (địa chỉ `0x27`)
- Keypad ma trận 4x4
- Dây nối dupont

## 📡 Sơ đồ chân kết nối
| Thiết bị       | Chân ESP32 |
|----------------|-----------|
| **Servo SG90** | GPIO 16   |
| **Keypad**     | R1 → GPIO 14  
|                | R2 → GPIO 27  
|                | R3 → GPIO 26  
|                | R4 → GPIO 25  
|                | C1 → GPIO 17  
|                | C2 → GPIO 32  
|                | C3 → GPIO 18  
|                | C4 → GPIO 19 |
| **LCD I2C**    | SDA → GPIO 21  
|                | SCL → GPIO 22 |

<img width="1001" height="484" alt="image" src="https://github.com/user-attachments/assets/8a04d684-cbf6-41fd-8bd6-f527e207cbae" />

## 📦 Thư viện cần cài
Vào **Arduino IDE** → **Sketch → Include Library → Manage Libraries** và tìm cài:
- `LiquidCrystal_I2C` (LCD I2C)
- `Keypad` (Bàn phím ma trận)
- `ESP32Servo` (Điều khiển Servo)
- `EEPROM` (Lưu mật khẩu)
- `WiFi` (Kết nối WiFi)
- `PubSubClient` (MQTT)

## 🔑 Mật khẩu mặc định & chế độ đặc biệt
- **Mật khẩu mặc định:** `12345`
- **Đổi mật khẩu:** nhập `*#01#`
- **Reset mật khẩu:** nhập `*#02#`

## 📜 Cách hoạt động
1. Hệ thống hiển thị "Enter Password" trên LCD.
2. Người dùng nhập mật khẩu:
   - Nếu đúng → mở cửa (servo quay 180°), gửi log MQTT.
   - Nếu sai → báo lỗi. Sai 3 lần → khóa 1 phút.
3. Nếu nhập lệnh đặc biệt:
   - `*#01#` → vào chế độ đổi mật khẩu.
   - `*#02#` → vào chế độ reset mật khẩu.
4. Mật khẩu mới sẽ được lưu vào EEPROM nên không bị mất khi khởi động lại.

## 📡 Cấu hình WiFi & MQTT
Trong code, chỉnh thông tin:
```cpp
const char* ssid = "Tên_WiFi";
const char* password_wifi = "Mật_khẩu_WiFi";

const char* mqtt_server = "192.168.xxx.xxx"; // IP MQTT broker
const int mqtt_port = 1883;
