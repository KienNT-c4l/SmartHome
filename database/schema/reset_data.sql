USE smarthome;
SET FOREIGN_KEY_CHECKS = 0;

-- Xóa dữ liệu trong đúng thứ tự khóa ngoại (từ phụ đến chính)
DELETE FROM control_log;
DELETE FROM sensor_data;
DELETE FROM devices;
DELETE FROM rooms;
DELETE FROM units;

-- Reset AUTO_INCREMENT (nếu muốn bắt đầu lại từ ID = 1)
ALTER TABLE control_log AUTO_INCREMENT = 1;
ALTER TABLE sensor_data AUTO_INCREMENT = 1;
ALTER TABLE devices AUTO_INCREMENT = 1;
ALTER TABLE rooms AUTO_INCREMENT = 1;
ALTER TABLE units AUTO_INCREMENT = 1;

SET FOREIGN_KEY_CHECKS = 1;
