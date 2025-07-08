# Cattle Health Monitoring System

A real-time cattle health monitoring and disease prediction system using IoT sensors, ThingSpeak cloud, and machine learning models.

## 📋 Project Overview

This project integrates affordable sensors with an ESP32 microcontroller to monitor cattle health in real-time. Data collected includes temperature, pulse, and motion, which is transmitted to ThingSpeak for visualization and analysis. A MATLAB-based machine learning model (Random Forest) processes the data to predict the cattle's health status as Healthy, Ovulation, or Unhealthy. Alerts are triggered via email for unhealthy conditions.

## 🛠 Components Used
- ESP32 DevKit V1
- MLX90614 (IR Temperature Sensor)
- MAX30102 (Pulse Sensor)
- MPU6050 (Motion Sensor)
- ThingSpeak Cloud
- MATLAB Analysis App (for ML)

 ## 🔄 Data Flow
### 1. Data Acquisition
- Sensors (MAX30102, MLX90614, MPU6050) attached to the cow capture physiological and motion-related data.
- These include:
  - Pulse rate (in BPM)
  - Surface body temperature (in °C)
  - 3-axis motion data (acceleration)

### 2. Edge Processing with ESP32
- The ESP32 microcontroller polls the sensor data over I2C.
- Performs basic preprocessing (e.g., averaging, noise filtering).
- Manual inputs such as age and sex are entered via UART or hard-coded.

### 3. Data Upload to Cloud
- Every 15 to 30 seconds, ESP32 connects to a Wi-Fi network.
- Sensor values are uploaded to ThingSpeak using HTTP REST API.
- Each parameter maps to a designated field in the ThingSpeak channel.

### 4. Cloud-Side Processing with MATLAB
- MATLAB Analysis on ThingSpeak is triggered after each new entry.
- It reads the latest values from all fields.
- Applies normalization and feeds the data to a Random Forest classifier.

### 5. Health Classification
- The model predicts the animal's health as:
  - Healthy
  - Ovulation (if female with high temperature and motion)
  - Unhealthy (patterns resembling FMD symptoms)

### 6. Result Storage and Visualization
- The predicted result is written to Field 6.
- Real-time graphs on ThingSpeak show trends in temperature, pulse, and motion.

### 7. Email Alerts
- If the output is "Unhealthy," an alert email is sent via `sendEmailAlert()`.
- Farmers can take immediate action to isolate or treat the animal.

  ## Block Diagram
  

![Untitled Diagram drawio (4) drawio](https://github.com/user-attachments/assets/379a43c8-ef38-4d44-beaa-31e690c7b2b4)

## Circuit Diagram
![Screenshot 2025-06-30 203251](https://github.com/user-attachments/assets/4cb5b943-e6b9-4358-87a6-4d96873cfd83)



## Results
### 1.Matlab Visualisation
![Screenshot 2025-07-01 120921](https://github.com/user-attachments/assets/0565a374-f3b4-458a-bcd2-7c032e95676a)


### 2.Alerting mail




![WhatsApp Image 2025-07-01 at 13 56 39_d372466f](https://github.com/user-attachments/assets/faeaae31-9a6d-48ff-9f73-33d458b96fd2)
