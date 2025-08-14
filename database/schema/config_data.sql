USE smarthome;

-- Add measurement units
INSERT INTO units (name, symbol, description) VALUES 
('Temperature', '°C', 'Celsius degree'),
('Humidity', '%', 'Relative Humidity'),
('Gas_level', 'ppm', 'Parts per million - gas concentration');
