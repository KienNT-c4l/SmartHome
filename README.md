# 🏠 Smart Home Database

Thư mục `database/` chứa toàn bộ các tệp phục vụ cho việc khởi tạo, quản lý và truy vấn cơ sở dữ liệu của hệ thống mô phỏng nhà thông minh (smart-home-iot).

---

## 📁 Cấu trúc thư mục

```text
database/
├── README.md                       # Tài liệu hướng dẫn
├── schema                          # Cấu trúc và dữ liệu ban đầu
│   ├── create_table.sql             # Tạo các bảng dữ liệu chính
│   ├── config_data.sql              # Dữ liệu cấu hình đơn vị đo (lưu thông tin đơn vị)
│   ├── sample_data.sql              # Dữ liệu mẫu để test hệ thống
│   └── reset_data.sql               # Xóa dữ liệu mẫu
├── queries                         # Các truy vấn thường dùng
│   ├── insert_device_control_log.sql  # Lịch sử điều khiển thiết bị
│   ├── select_devices_on.sql          # Danh sách thiết bị đang bật
│   ├── select_sensor_data.sql         # Lấy danh sách dữ liệu cảm biến
│   ├── select_door_log.sql            # Lịch sử mở cửa
│   ├── select_all_rooms.sql           # Lấy danh sách tất cả các phòng
│   ├── select_avg_sensor_today.sql    # Trung bình nhiệt độ/độ ẩm hôm nay
│   ├── select_devices_by_room.sql     # Lấy thiết bị theo từng phòng
│   ├── select_latest_sensor_per_room.sql  # Dữ liệu cảm biến mới nhất mỗi phòng
│   └── select_units.sql               # Lấy thông tin các đơn vị đo
└── scripts                         # Scripts Python hỗ trợ
    ├── init_db.py                   # Chạy schema/create_table.sql khởi tạo CSDL
    ├── test_sample_data.py          # Chạy schema/sample_data.sql tạo dữ liệu mẫu
    ├── mqtt_to_db.py                # Script lưu dữ liệu vào database
    └── reset_data.py                # Chạy schema/reset_data.sql xóa dữ liệu mẫu
```
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

### `door_log`
Lịch sử mở cửa
- `id`:                 khóa chính
- `room_id`:            phòng có cửa
- `door_status`:        trạng thái đóng mở
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

|               Tệp                   |               Mô tả                   |
|-------------------------------------|---------------------------------------|
| `select_all_rooms.sql`              | Lấy danh sách tất cả các phòng        |
| `select_sensor_data.sql`            | Lấy dữ liệu cảm biến                  |
| `select_door_log.sql`               | Lịch sử mở cửa                        |
| `select_devices_by_room.sql`        | Lấy thiết bị theo từng phòng          |
| `select_devices_on.sql`             | Danh sách thiết bị đang bật           |
| `insert_device_control_log.sql`     | Lịch sử điều khiển thiết bị           |
| `select_avg_sensor_today.sql`       | Trung bình nhiệt độ/độ ẩm hôm nay     |
| `select_latest_sensor_per_room.sql` | Dữ liệu cảm biến mới nhất mỗi phòng   |
| `select_units.sql`                  | Lấy thông tin các đơn vị đo           |

Ví dụ: Chạy select_sensor_data.sql để hiển thị dữ liệu cảm biến đã lưu:
```bash
sudo mysql -u root -p < ./queries/select_sensor_data.py
```

---

## ⚙️ 3. Khởi tạo CSDL và kiểm thử bằng Python

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
  Password:         pi
  Database:         smarthome


### ▶️ Cách chạy:
Chạy môi trường ảo venv trước khi chạy các script python:
```bash
source ../venv/bin/activate
```


Từ thư mục gốc dự án (nơi chứa thư mục `database/`), mở Terminal (hoặc CMD) và chạy lệnh sau:

```bash
python3 database/scripts/init_db.py
python3 database/scripts/test_sample_data.py
```
Script sẽ thực hiện các bước sau:
1. Kết nối tới MySQL server
2. Tạo cơ sở dữ liệu 'smarthome' nếu chưa tồn tại
3. Tạo các bảng từ file: schema/create_table.sql
4. Thêm dữ liệu mẫu từ file: schema/sample_data.sql

---

### ♻️ Xoá dữ liệu mẫu sau khi test (giữ nguyên cấu trúc)
```bash
python3 database/scripts/reset_data.py
```

## 🧪 4. Kiểm tra kết quả

Sau khi chạy `init_db.py`, bạn có thể mở MySQL Workbench hoặc terminal để xác minh:
mysql -u root -p

```sql
-- Kiểm tra các bảng đã tạo
SHOW TABLES;

-- Xem dữ liệu phòng
SELECT * FROM rooms;

-- Xem dữ liệu cảm biến
SELECT * FROM sensor_data;

-- Xem lịch sử điều khiển
SELECT * FROM control_log;
