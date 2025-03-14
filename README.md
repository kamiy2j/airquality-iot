# Smart Air Quality Monitoring and Ventilation Prediction System
### IOT (Class: 98410) - Spring 2025

Kamran Shahid - 29457

Muhammad Maaz Arsalan Batla - 22794

### 📖 Description

This project monitors indoor air quality and predicts ventilation needs using IoT and machine learning. It uses an ESP32 microcontroller with MQ-135, DHT22, and PMS5003 sensors to measure air quality parameters:

- 🌡️ Temperature

- 💧 Humidity

- 🌬️ Particulate Matter (PM2.5)

- 💨 Volatile Organic Compounds (VOCs)

Data is transmitted to a Raspberry Pi running an MQTT server (Mosquitto). A machine learning model built with TensorFlow Lite predicts ventilation needs, and data is stored and visualized using InfluxDB and Grafana.

### 🎥 Demo Video

[Watch the demo on YouTube](https://youtu.be/JR_lew51vHkd)

### 🚀 Features

- Real-Time Monitoring of air quality parameters.

- Machine Learning Prediction of ventilation needs categorized as:
  -   No Ventilation Needed
	- 	Low
	- 	Moderate
	- 	High
	- 	Urgent

- Comprehensive Visualization using Grafana.

- Efficient Data Storage with InfluxDB.

### 🛠️ Technology Stack
- **Microcontroller:** ESP32

- **Sensors:** MQ-135 (VOC), DHT22 (Temperature & Humidity), PMS5003 (PM2.5)

- **Communication Protocol:** MQTT (Mosquitto)

- **Machine Learning Framework:** TensorFlow Lite

- **Database:** InfluxDB

- **Visualization:** Grafana

### 💡 Why this Project?

Proper ventilation ensures a healthy indoor environment, boosting productivity, health, and comfort, particularly in:

- 🏫 Classrooms and Educational Institutions

- 🏢 Offices and Meeting Rooms

- 🏥 Healthcare Facilities

- 🏠 Residential Spaces

This system enables proactive management of air quality, reducing health risks and enhancing energy efficiency.

### 🔍 How it Works
- **Data Collection:** Sensors connected to ESP32 gather air quality data every 3 seconds.

- **Data Transmission:** ESP32 sends data as JSON via MQTT to a Raspberry Pi.

- **Machine Learning:** TensorFlow Lite model predicts ventilation needs on Raspberry Pi.

- **Storage & Visualization:** Data and predictions stored in InfluxDB and visualized with Grafana.
