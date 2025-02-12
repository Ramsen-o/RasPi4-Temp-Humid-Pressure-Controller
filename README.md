# Greenhouse Controller - Raspberry Pi 4
C source files and header files must be extracted to the main program folder in order to run.

## Overview
The **Greenhouse Controller** is a Raspberry Pi 4 project that displays real-time sensor data for **temperature, humidity, and pressure**. The readings are visualized in two ways:
- As a **thermostat-style display** on an attached screen.
- As a **bar graph** on the Raspberry Pi.

### **Visual Representation**
- **Green bars** indicate the **current** temperature.
- **Red bars** indicate the **current** humidity
- **Blue bars** indicate the **current** pressure.
- **Purple squares** represent the **target setpoints** for temperature and humidity.

## Features
- Real-time **sensor data collection**.
- **Dynamic bar graph** representation.
- User-defined **target setpoints**.
- Runs efficiently on **Raspberry Pi 4**.

## Installation & Setup
1. **Update and install required libraries**
   ```sh
   sudo apt update && sudo apt upgrade -y
   sudo apt install python3-pip
   pip3 install matplotlib Adafruit_DHT smbus2
   ```
2. **Enable I2C communication** (for BMP280 sensor)
   ```sh
   sudo raspi-config
   # Navigate to Interfacing Options > Enable I2C
   ```
3. **Clone the repository**
   ```sh
   git clone https://github.com/YOUR_USERNAME/Greenhouse-Controller.git
   cd Greenhouse-Controller
   ```
4. **Run the program**
   ```sh
   python3 greenhouse_controller.py
   ```

## Future Improvements
- Add **historical data logging** for trend analysis.
- Implement **automated climate control** based on sensor readings.
- Add a **web interface** for remote monitoring.

---
**Author:** [Ramsen Oraha]  
**Version:** 1.14  
**Date:** [2025-02-11]

