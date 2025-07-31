# 🏠 Smart Home Database

Thư mục `database/` chứa toàn bộ các tệp phục vụ cho việc khởi tạo, quản lý và truy vấn cơ sở dữ liệu của hệ thống mô phỏng nhà thông minh (smart-home-iot).

---

## 📁 Cấu trúc thư mục

database/
├── README.md                               # Tài liệu hướng dẫn
├── schema                                  # Cấu trúc và dữ liệu ban đầu
│   ├── create_table.sql                    # Tạo các bảng dữ liệu chính
│   ├── config_data.sql                     # Dữ liệu cấu hình đơn vị đo
│   └── sample_data.sql                     # Dữ liệu mẫu để test hệ thống
├── queries                                 # Các truy vấn thường dùng
│   ├── device_control_log.sql
│   ├── devices_currently_on.sql
│   ├── get_all_rooms.sql
│   ├── get_avg_sensor_today.sql
│   ├── get_devices_by_room.sql
│   ├── latest_sensor_data_per_room.sql
│   └── select_units.sql
└── scripts
    └── init_db.py                          # Script Python khởi tạo CSDL


---

## 🧱 1. Cấu trúc bảng dữ liệu

### `rooms`  
Thông tin các phòng trong nhà.  
- `id`:                 INT, khóa chính  
- `name`:               tên phòng (VD: Kitchen, Living Room)

### `devices`  
Thông tin các thiết bị (đèn, quạt...)  
- `id`:                 INT, khóa chính  
- `room_id`:            liên kết đến phòng  
- `name`:               tên thiết bị  
- `status`:             trạng thái hiện tại ('on', 'off')

### `sensor_data`  
Lưu dữ liệu cảm biến từng thời điểm  
- `id`:                 khóa chính  
- `room_id`:            phòng tương ứng  
- `temperature`
- `humidity`
- `gas_level`  
- `motion_detected`:    BOOLEAN  
- `timestamp`:          thời gian ghi nhận

### `control_log`  
Lịch sử điều khiển thiết bị  
- `device_id`:          thiết bị được điều khiển  
- `name`:               tên thiết bị  
- `status`:             'on', 'off'  
- `sender`:             người gửi lệnh (user / auto)  
- `timestamp`:          thời gian điều khiển

### `units`  
Thông tin đơn vị đo  
- `name`:               loại dữ liệu (temperature, humidity…)  
- `symbol`:             đơn vị (°C, %, ppm…)  
- `description`:        mô tả

---

## 📜 2. Các truy vấn SQL (`queries/`)

|               Tệp                 |               Mô tả                   |
|-----------------------------------|---------------------------------------|
| `get_all_rooms.sql`               | Lấy danh sách tất cả các phòng        |
| `get_devices_by_room.sql`         | Lấy thiết bị theo từng phòng          |
| `devices_currently_on.sql`        | Danh sách thiết bị đang bật           |
| `device_control_log.sql`          | Lịch sử điều khiển thiết bị           |
| `get_avg_sensor_today.sql`        | Trung bình nhiệt độ/độ ẩm hôm nay     |
| `latest_sensor_data_per_room.sql` | Dữ liệu cảm biến mới nhất mỗi phòng   |
| `select_units.sql`                | Lấy thông tin các đơn vị đo           |

---

## ⚙️ 3. Khởi tạo CSDL bằng Python

Bạn có thể khởi tạo nhanh CSDL bằng script **`scripts/init_db.py`**.

### ✅ Yêu cầu:
- Python 3.x
- Thư viện `mysql-connector-python`  
  Cài đặt bằng:
  ```bash
  pip install mysql-connector-python
  ```
- MySQL đã cài dặt với thông tin đăng nhập: (tùy theo mình đã cài đặt)
  Host:             localhost
  User:             root
  Password:         mobius
  Database:         smarthome


### ▶️ Cách chạy:

Từ thư mục gốc dự án (nơi chứa thư mục `database/`), mở Terminal (hoặc CMD) và chạy lệnh sau:

```bash
python database/scripts/init_db.py
```
Script sẽ thực hiện các bước sau:
1. Kết nối tới MySQL server
2. Tạo cơ sở dữ liệu 'smarthome' nếu chưa tồn tại
3. Tạo các bảng từ file: schema/create_table.sql
4. Thêm dữ liệu mẫu từ file: schema/sample_data.sql

---

## 🧪 4. Kiểm tra kết quả

Sau khi chạy `init_db.py`, bạn có thể mở MySQL Workbench hoặc terminal để xác minh:

```sql
-- Kiểm tra các bảng đã tạo
SHOW TABLES;

-- Xem dữ liệu phòng
SELECT * FROM rooms;

-- Xem dữ liệu cảm biến
SELECT * FROM sensor_data;

-- Xem lịch sử điều khiển
SELECT * FROM control_log;
