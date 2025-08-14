USE smarthome;

-- List devices that are currently on
SELECT 
    devices.name AS device_name,
    rooms.name AS room_name,
    devices.status
FROM devices
JOIN rooms ON devices.room_id = rooms.id
WHERE devices.status = 'on';
