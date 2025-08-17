USE smarthome;

-- Device control history
SELECT 
    control_log.timestamp AS control_time,
    devices.name AS device_name,
    rooms.name AS room_name,
    control_log.status,
    control_log.sender
FROM devices
JOIN rooms ON rooms.id = devices.room_id
JOIN control_log ON devices.id = control_log.device_id
ORDER BY control_log.timestamp DESC;
