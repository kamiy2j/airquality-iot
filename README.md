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

![WhatsApp Image 2025-03-14 at 17 12 13_2204669a](https://github.com/user-attachments/assets/e3f72386-2b8c-4a77-a25d-cf5109ae8b9a)
![WhatsApp Image 2025-03-14 at 17 12 12_cc341ec2](https://github.com/user-attachments/assets/8e0bfa1d-c917-4537-ab39-6575f57ada09)
![WhatsApp Image 2025-03-14 at 17 12 12_572288d4](https://github.com/user-attachments/assets/76608dfb-e15b-45d8-a874-3320b2dea4e9)

### 🎥 Demo Video

[Watch the demo on YouTube](https://www.youtube.com/watch?v=jAZZ5G1qESE)

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

### 🔍 Snapshots
![Screenshot 2025-03-14 172522](https://github.com/user-attachments/assets/98d08e81-113a-4ece-9066-856fd670efb6)
![Screenshot 2025-03-14 162812](https://github.com/user-attachments/assets/0944ef8c-41dc-4a40-9ad4-f5953a49ad65)
![Screenshot 2025-03-14 162759](https://github.com/user-attachments/assets/e422e9a7-d731-4e4e-aad5-14eb3df4bc69)
![Screenshot 2025-03-14 162434](https://github.com/user-attachments/assets/3b0afc60-85ec-40d9-b24a-dce49df52711)

![Screenshot 2025-03-14 162839](https://github.com/user-attachments/assets/c3533b64-3c86-4435-bf7e-35e3b015ce76)
![Screenshot 2025-03-14 163128](https://github.com/user-attachments/assets/232e6dc6-8d19-4076-acf6-9e14d22aeb50)







