-- List devices for a specific room
SELECT rooms.name, devices.name, devices.id
FROM devices
JOIN rooms ON rooms.id = devices.room_id
ORDER BY rooms.name, devices.name, devices.id;