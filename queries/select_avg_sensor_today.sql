USE smarthome;

-- Get the average sensor data for today
SELECT 
    rooms.name AS room_name,
    ROUND(AVG(sensor_data.temperature), 1) AS avg_temperature,
    ROUND(AVG(sensor_data.humidity), 1) AS avg_humidity
FROM rooms
JOIN sensor_data ON sensor_data.room_id = rooms.id
WHERE DATE(sensor_data.timestamp) = CURDATE()
GROUP BY rooms.name;
