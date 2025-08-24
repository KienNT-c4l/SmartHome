# Project Overview
# SmartHome - IoT Smart Home System  

## 📌 Introduction  

**SmartHome** is a simulated IoT-based smart home system built with **ESP32** and simple devices (LEDs, motors) to represent appliances. The system provides the following features:  

- Real-time monitoring of **temperature and humidity** (room1: living room, bedroom, …) using the **DHT11 sensor**.  
- Kitchen monitoring with **DHT11** and **gas sensor** for fire/gas leak detection. When thresholds are exceeded, the system sends a **Telegram alert**.  
- Device control: **LEDs** represent lights, and **motors** represent fans.  
- Door access via **passcode entry** (door module), without PIR sensor integration at this stage.  

The project demonstrates the ability to design and implement an IoT prototype, develop **ESP32 firmware**, integrate **MQTT**, manage data with **MariaDB**, and provide visualization/control via **OpenHAB**.  

🔗 More details are available on the [Wiki Page: Project Detail](https://github.com/KienNT-c4l/SmartHome/wiki).  
![SmartHome System Model](https://github.com/KienNT-c4l/SmartHome/blob/main/assets/demo1.png)  

---

## 🚀 Features  

- **Room1 Module (living room, bedroom, …):**  
  - DHT11: temperature & humidity monitoring.  
  - LED (light) and motor (fan) control based on environmental conditions.  

- **Kitchen Module:**  
  - DHT11: temperature & humidity monitoring.  
  - Gas sensor: gas leak detection.  
  - Sends **Telegram alerts** when thresholds are exceeded.  

- **Door Module:**  
  - Passcode-based door unlocking, simulating security.  

- **Database:**  
  - **MariaDB** for storing sensor data and device states.  

- **Frontend:**  
  - **OpenHAB** for visualization and device control.  

---

## 🛠️ Technologies Used  

- **Hardware:** ESP32, Raspberry Pi 5, DHT11, Gas sensor, LED, Motor  
- **Firmware:** Arduino IDE (C/C++ for ESP32)  
- **Protocols:** MQTT  
- **Server & Database:** Raspberry Pi 5, Python, MariaDB  
- **Frontend:** OpenHAB  

---

## 📂 Project Structure  

SmartHome/
├── Door/                  # Door module: passcode entry
├── kitchen/               # Kitchen module: DHT11 + gas sensor + Telegram alert
├── room1/                 # Room module: DHT11 + LED/motor (fan, light)
├── database/              # Sensor data & device state management
├── openHAB/               # OpenHAB configuration for visualization & control
├── assets/                # Images, GIFs, demo videos
└── README.md

---

## 🔧 Deployment Guide  

1. **Set up Python Environment**  

2. **Configure MariaDB**  
   - Create a database and tables for storing sensor data and device states.  
   - Update the database connection details in the Python script.  

3. **ESP32 Firmware**  
   - Flash the ESP32 with the provided firmware using Arduino IDE.  
   - ESP32 publishes DHT11 and gas sensor data to the MQTT broker running on Raspberry Pi.  

4. **Run MQTT-to-DB Script**  

```bash
cd database
python3 mqtt_to_db.py

 - This script subscribes to MQTT topics from ESP32 and stores the data in **MariaDB** in real time.  

---

## 📊 Visualization & Control  

- Configure **OpenHAB** to connect with the MQTT broker and MariaDB.  
- Build a dashboard to display:  
  - Temperature  
  - Humidity  
  - Gas status  
  - LED (light)  
  - Motor (fan)  
- Users can directly control simulated devices (lights/fans) from the **OpenHAB interface**.  

---

## 🤝 Contribution  

Contributions are welcome! You can help improve the project by:  

- Adding new modules (sensors, actuators).  
- Optimizing the database structure and enhancing OpenHAB dashboards.  
- Expanding alerting capabilities via **Telegram** or other notification services.  
