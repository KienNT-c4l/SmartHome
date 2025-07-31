-- Get the average sensor data for today
SELECT rooms.name, AVG(sensor_data.temperature), AVG(sensor_data.humidity)
FROM rooms
JOIN sensor_data ON sensor_data.room_id = rooms.id
WHERE DATE(sensor_data.timestamp) = CURDATE()
GROUP BY rooms.name;