USE smarthome;

-- Insert rooms
INSERT INTO rooms (name) VALUES 
('room1'),
('kitchen'),
('door');



-- Insert devices
INSERT INTO devices (room_id, name, status) VALUES
(1, 'Light Room1', 'off'),
(1, 'Fan Room1', 'on'),
(2, 'Light Kitchen', 'on'),
(3, 'Door Sensor', 'off');


-- Insert sensor_data
INSERT INTO sensor_data (room_id, temperature, humidity, gas_level, motion_detected, timestamp) VALUES
(1, 29.3, 56.4, NULL, NULL, '2025-08-06 15:16:11'),
(1, 28.5, 54.5, NULL, NULL, '2025-08-06 15:23:11'),
(1, 29.3, 55.1, NULL, NULL, '2025-08-06 15:30:11'),
(1, 29.0, 56.8, NULL, NULL, '2025-08-06 15:37:11'),
(1, 28.2, 54.5, NULL, NULL, '2025-08-06 15:44:11'),
(1, 27.7, 54.0, NULL, NULL, '2025-08-06 15:51:11'),
(1, 27.8, 54.3, NULL, NULL, '2025-08-06 15:58:11'),
(1, 29.2, 57.0, NULL, NULL, '2025-08-06 16:05:11'),
(1, 27.6, 57.6, NULL, NULL, '2025-08-06 16:12:11'),
(1, 29.0, 55.8, NULL, NULL, '2025-08-06 16:19:11'),
(2, 29.3, 61.6, 246, NULL, '2025-08-06 15:06:11'),
(2, 29.6, 62.5, 225, NULL, '2025-08-06 15:14:11'),
(2, 29.3, 61.6, 228, NULL, '2025-08-06 15:22:11'),
(2, 29.1, 61.9, 246, NULL, '2025-08-06 15:30:11'),
(2, 30.8, 61.0, 209, NULL, '2025-08-06 15:38:11'),
(2, 30.8, 61.5, 245, NULL, '2025-08-06 15:46:11'),
(2, 29.7, 62.9, 245, NULL, '2025-08-06 15:54:11'),
(2, 29.0, 61.5, 191, NULL, '2025-08-06 16:02:11'),
(2, 30.8, 59.9, 247, NULL, '2025-08-06 16:10:11'),
(2, 29.6, 59.1, 210, NULL, '2025-08-06 16:18:11'),
(3, NULL, NULL, NULL, 0, '2025-08-06 16:16:11'),
(3, NULL, NULL, NULL, 0, '2025-08-06 16:18:11'),
(3, NULL, NULL, NULL, 0, '2025-08-06 16:20:11'),
(3, NULL, NULL, NULL, 1, '2025-08-06 16:22:11'),
(3, NULL, NULL, NULL, 1, '2025-08-06 16:24:11');


-- Insert control_log
INSERT INTO control_log (device_id, name, status, sender, timestamp) VALUES
(1, 'Light Room1', 'off', 'user', NOW() - INTERVAL 65 MINUTE),
(2, 'Fan Room1', 'on', 'auto', NOW() - INTERVAL 55 MINUTE),
(3, 'Light Kitchen', 'on', 'user', NOW() - INTERVAL 45 MINUTE),
(4, 'Door Sensor', 'off', 'auto', NOW() - INTERVAL 5 MINUTE);
