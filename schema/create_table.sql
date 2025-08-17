-- Create database
CREATE DATABASE IF NOT EXISTS smarthome;
USE smarthome;

-- Create tables
-- ROOMS
CREATE TABLE IF NOT EXISTS rooms(
    id INT AUTO_INCREMENT PRIMARY KEY,
    name VARCHAR(30)NOT NULL
);

-- DEVICES
CREATE TABLE IF NOT EXISTS devices(
    id INT AUTO_INCREMENT PRIMARY KEY,
    room_id INT,
    name VARCHAR(30) NOT NULL,
    status ENUM('on', 'off'),
    FOREIGN KEY (room_id) REFERENCES rooms(id)
);

-- SENSOR DATA
CREATE TABLE IF NOT EXISTS sensor_data(
    id INT AUTO_INCREMENT PRIMARY KEY,
    room_id INT,
    temperature FLOAT,
    humidity FLOAT,
    gas_level FLOAT,
    motion_detected BOOLEAN DEFAULT FALSE,
    timestamp DATETIME DEFAULT CURRENT_TIMESTAMP,
    FOREIGN KEY (room_id) REFERENCES rooms(id)
);

-- DOOR LOG
CREATE TABLE IF NOT EXISTS door_log(
    id INT AUTO_INCREMENT PRIMARY KEY,
    room_id INT,                                    -- The room that have door
    door_status ENUM('open', 'closed') NOT NULL,    -- The door status
    timestamp DATETIME DEFAULT CURRENT_TIMESTAMP,   -- timestamp
    FOREIGN KEY (room_id) REFERENCES rooms(id)
);

-- CONTROL LOG
CREATE TABLE IF NOT EXISTS control_log(
    id INT AUTO_INCREMENT PRIMARY KEY,
    device_id INT,
    name VARCHAR(20),
    status ENUM('on', 'off'),
    sender VARCHAR(30),
    timestamp DATETIME DEFAULT CURRENT_TIMESTAMP,
    FOREIGN KEY (device_id) REFERENCES devices(id)
);

-- DATA UNIT
CREATE TABLE IF NOT EXISTS units (
    id INT AUTO_INCREMENT PRIMARY KEY,
    name VARCHAR(50) NOT NULL,                   -- temperature, humidity, ...
    symbol VARCHAR(10) NOT NULL,                 -- °C, °F, %, ...
    description TEXT                             -- description
);
