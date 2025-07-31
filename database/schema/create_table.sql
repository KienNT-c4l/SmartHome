-- Create database
CREATE DATABASE IF NOT EXISTS smarthome,
USE smarthome;

-- Create tables
-- ROOMS
CREATE TABLE rooms(
    id INT AUTO_INCREMENT PRIMARY KEY,
    name VARCHAR(20)
);

-- DEVICES
CREATE TABLE devices(
    id INT AUTO_INCREMENT PRIMARY KEY,
    room_id INT,
    name VARCHAR(20),
    status ENUM('on', 'off'),
    FOREIGN KEY (room_id) REFERENCES rooms(id)
    FOREIGN KEY (status) REFERENCES control_log(status)
);

-- SENSOR DATA
CREATE TABLE sensor_data(
    id INT AUTO_INCREMENT PRIMARY KEY,
    room_id INT,
    temperature FLOAT,                  -- 
    humidity FLOAT,
    gas_level FLOAT,
    motion_detected BOOLEAN,
    timestamp DATETIME DEFAULT CURRENT_TIMESTAMP,
    FOREIGN KEY (room_id) REFERENCES rooms(id)
);

-- CONTROL LOG
CREATE TABLE control_log(
    id INT AUTO_INCREMENT PRIMARY KEY,
    device_id INT,
    name VARCHAR(20),
    status ENUM('on', 'off'),
    sender VARCHAR(30),
    timestamp DATETIME DEFAULT CURRENT_TIMESTAMP,
    FOREIGN KEY (device_id) REFERENCES device(id)
);

-- DATA UNIT
CREATE TABLE units (
    id INT AUTO_INCREMENT PRIMARY KEY,
    name VARCHAR(50),                   -- temperature, humidity, ...
    symbol VARCHAR(10),                 -- °C, °F, %, ...
    description TEXT                    -- description
);
