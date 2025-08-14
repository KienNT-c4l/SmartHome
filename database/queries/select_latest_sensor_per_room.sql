USE smarthome;

-- Get the latest sensor data for each room
SELECT 
    sd.id,
    sd.room_id,
    r.name AS room_name,
    sd.temperature,
    sd.humidity,
    sd.gas_level,
    sd.motion_detected,
    sd.timestamp
FROM sensor_data sd
JOIN rooms r ON sd.room_id = r.id
JOIN (
    SELECT room_id, MAX(timestamp) AS latest_time
    FROM sensor_data
    GROUP BY room_id
) latest ON sd.room_id = latest.room_id AND sd.timestamp = latest.latest_time;
