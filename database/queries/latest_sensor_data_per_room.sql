-- Get the latest sensor data for each room
SELECT * FROM sensor_data
WHERE timestamp = (
    SELECT MAX(timestamp)
    FROM sensor_data
);