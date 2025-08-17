USE smarthome;

-- Get information about units
SELECT 
    id AS unit_id,
    name AS unit_name,
    symbol AS unit_symbol,
    description AS unit_description
FROM units;
