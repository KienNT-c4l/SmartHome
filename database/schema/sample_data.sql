-- Use database
USE smarthome;

-- Add rooms
INSERT INTO rooms (name) VALUES 
('Kitchen'), 
('Living Room'), 
('Bedroom');

-- Add devices
INSERT INTO devices (room_id, name) VALUES 
(1, 'Kitchen Light'),
(2, 'Living Room Light'),
(3, 'Bedroom Fan');

-- Add sensor data
INSERT INTO sensor_data (room_id, temperature, humidity, gas_level, motion_detected) VALUES 
(1, 30.2, 60.5, 320.7, TRUE),
(2, 28.1, 55.0, NULL, FALSE),
(3, 27.0, 50.2, NULL, FALSE);

-- Record device control logs
INSERT INTO control_log (device_id, name, status, sender) VALUES 
(2, 'Kitchen Light', 'on', 'Auto'),
(3, 'Living Room Light', 'off', 'User'),
(4, 'Bedroom Fan', 'on', 'User');
