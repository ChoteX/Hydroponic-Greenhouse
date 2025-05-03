
# Automated Greenhouse + AI disease detection


# 🥬 Lettuce Health Classifier

This project uses a modified EfficientNet-B0 model to classify grayscale images of lettuce leaves as either **Healthy** or **Diseased**.



## 🧠 Model Overview

- **Base Model**: EfficientNet-B0
- **Input**: jpeg image
- **Output Classes**: 
  - `Healthy`
  - `Diseased`
- **Checkpoint**: `lettuce_classifier.pth`



## 🛠️ Requirements

Install dependencies using pip:

```bash
pip install torch torchvision pillow
```


---

# 🌿 ESP32 Smart Greenhouse System

This Arduino-based system uses an **ESP32** microcontroller to automate and monitor an indoor agriculture or hydroponic environment. It collects sensor data and publishes it to **Adafruit IO** using MQTT while also allowing for remote control of actuators such as heaters, lights, pumps, and nutrient dispensers.



## 🧰 Hardware Used

- ESP32 development board  
- DHT11 sensor (temperature & humidity)  
- Ultrasonic sensor (for water level detection)  
- Servo motor (for nutrient dispensing)  
- SSD1306 OLED Display (128x64)  
- Relay-controlled devices: heater, pump, lights


## 📡 Features

- 📶 Connects to WiFi and publishes sensor data to **Adafruit IO**
- 📈 Publishes:
  - Temperature
  - Humidity
  - Water level
- 📲 Subscribes to:
  - Manual/Auto mode toggle
  - Light/heater/pump switches
  - Setpoint temperature
  - Nutrient dispensing commands
- 🧠 Supports both manual and automatic control
- 🖥 Displays key metrics on OLED screen



## 🔌 Pin Configuration (ESP32)

| Component     | Pin         |
|---------------|-------------|
| Heater Relay  | GPIO 12     |
| Pump Relay    | GPIO 27     |
| Light Relay   | GPIO 13     |
| DHT11 Sensor  | GPIO 14     |
| Servo Motor   | GPIO 5      |
| Ultrasonic    | Trig: 18, Echo: 19 |




## ☁️ MQTT Configuration (Adafruit IO)

Update these values in the code:

```cpp
#define WLAN_SSID "YourWiFiSSID"
#define WLAN_PASS "YourWiFiPassword"

#define AIO_USERNAME "YourAIOUsername"
#define AIO_KEY "YourAIOKey"

```
---

## 👥 Team Members

      

* **Dachi Tchotashvili**    
* **Levan Lolashvili**      
* **Dea Nozadze**   
* **Nika Gegenava**      

---
