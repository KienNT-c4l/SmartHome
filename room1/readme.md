# ESP32 + DHT11 → MQTT + Fan Control + WiFiManager Configuration

## 📌 Module room1
Dự án này sử dụng **ESP32** để:
- Đọc nhiệt độ và độ ẩm từ cảm biến **DHT11**
- Gửi dữ liệu lên **MQTT Broker**
- Điều khiển quạt theo **2 chiều**:
  - Từ MQTT (nhận lệnh qua topic)
  - Từ nút bấm vật lý trên ESP32
- Cho phép cấu hình thông số WiFi & MQTT server qua **WiFiManager** mà không cần nạp lại code
- Lưu cấu hình vào **Preferences (EEPROM)** để giữ sau khi reset

---

## 🛠 Phần cứng
- **ESP32 DevKit**
- **DHT11** (đo nhiệt độ và độ ẩm)
- **Quạt** hoặc LED giả lập quạt (kết nối qua relay hoặc transistor)
- **Nút bấm**:
  - GPIO0: Điều khiển quạt
  - GPIO16: Kích hoạt chế độ cấu hình WiFi/MQTT

---

## 📡 Chức năng chính

### 1. Đọc cảm biến DHT11
- Sử dụng thư viện `DHT.h`
- Đọc nhiệt độ (°C) và độ ẩm (%)
- Gửi dữ liệu qua MQTT mỗi **10 giây**

### 2. Kết nối MQTT
- Sử dụng thư viện `PubSubClient`
- Gửi dữ liệu lên kênh:
  - `/home/room1/temperature` → Nhiệt độ
  - `/home/room1/humidity` → Độ ẩm
  - `/home/room1/fan` → Trạng thái quạt (`ON` hoặc `OFF`)
- Nhận lệnh qua topic `/home/room1/fan/set`
  - `ON` / `1` / `TRUE` → Bật quạt
  - `OFF` / `0` / `FALSE` → Tắt quạt
  - `TOGGLE` → Đảo trạng thái quạt

### 3. Điều khiển quạt
- **Từ MQTT**: Xử lý trong hàm `onMqttMessage()`
- **Từ nút bấm**: Xử lý trong `handleFanButton()` với chống rung nút (debounce)
- Tự động gửi trạng thái quạt hiện tại lên MQTT khi thay đổi

### 4. Cấu hình WiFi & MQTT qua WiFiManager
- Nếu nhấn giữ **GPIO16** khi khởi động → Mở cổng cấu hình WiFi + MQTT
- Truy cập SSID `ESP32_Config` để nhập thông tin
- Tự động lưu vào bộ nhớ `Preferences`
- Nếu không kết nối được WiFi sau 10 giây → Mở cổng cấu hình

### 5. Lưu và tải cấu hình
- Sử dụng `Preferences` để lưu `mqtt_server` và `mqtt_port`
- Lưu khi cài đặt xong WiFiManager
- Tải lại khi khởi động

---

## 📂 Cấu trúc code

| Phần | Mô tả |
|------|-------|
| **CONFIG** | Định nghĩa chân GPIO, thông số MQTT mặc định |
| **UTILS** | Hàm `blink()` nháy LED báo hiệu |
| **MQTT CALLBACK** | Hàm `onMqttMessage()` xử lý dữ liệu nhận được |
| **CONFIG FUNCTIONS** | `loadConfig()`, `saveConfig()`, `startConfigPortal()` |
| **BUTTON HANDLER** | Xử lý nút điều khiển quạt với chống rung |
| **NETWORK** | `mqttConnect()` kết nối lại nếu mất |
| **MAIN SETUP** | Khởi tạo, tải cấu hình, kiểm tra nút config, kết nối WiFi |
| **MAIN LOOP** | Xử lý MQTT, nút bấm, đọc cảm biến, gửi dữ liệu |

---

## 📋 Luồng hoạt động
1. **Khởi động**
   - Tải cấu hình từ bộ nhớ
   - Nếu giữ nút **Config** → Mở portal cấu hình WiFi + MQTT
   - Nếu không, kết nối WiFi đã lưu
2. **Kết nối MQTT**
   - Subscribe topic điều khiển quạt
   - Gửi trạng thái quạt ban đầu
3. **Trong vòng lặp**
   - Kiểm tra nút config (giữ 3 giây để mở portal)
   - Xử lý nút quạt
   - Đọc DHT11 mỗi 10 giây
   - Gửi dữ liệu lên MQTT

---

## 🔌 Sơ đồ kết nối

---

## 🖼 Lưu đồ thuật toán

