-- List devices that are currently on
SELECT devices.name, rooms.name, devices.status
FROM devices
JOIN rooms ON devices.room_id = rooms.id
WHERE devices.status = "on";