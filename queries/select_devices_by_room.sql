USE smarthome;

-- List devices for a specific room
SELECT 
    rooms.name AS room_name,
    devices.name AS device_name,
    devices.id   AS device_id
FROM devices
JOIN rooms ON rooms.id = devices.room_id
ORDER BY room_name, device_name, device_id;
