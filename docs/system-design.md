# System Design: ESP32 Black Box Data Logger

## 1. Overview
A fault-tolerant flight data logger for capturing aerospace telemetry using ESP32, logging to microSD in real time.

## 2. System Block Diagram
Currently Working On.

## 3. Hardware Components
- **ESP32 WROOM** — main microcontroller
- **DS3231 RTC** — accurate timestamping
- **MPU6050** — IMU for acceleration and gyroscope
- **BMP280** — barometric pressure (altitude)
- **GPS Module** — latitude and longitude
- **microSD Module** — data storage
- **Pushbuttons & LEDs** — user interface
- **Battery Backup** — optional for power failure handling

## 4. Logging Workflow
1. User powers on the device
2. RTC provides time sync
3. Button press starts logging
4. Sensor data is collected at interval (e.g., 5Hz)
5. Timestamp + sensor values saved to CSV
6. Another button press stops logging

## 5. Data Format
| Time (UTC) | Altitude (m) | Accel (m/s²) | Gyro (°/s) | Latitude | Longitude |
|------------|--------------|--------------|------------|----------|-----------|
| HH:MM:SS   | 1234.56      | X,Y,Z        | X,Y,Z      | xx.xxxx  | xx.xxxx   |

## 6. Resilience Plan
- Buffered writes to handle brief power loss
- Periodic file flush to protect data
- Optional FRAM/SRAM backup buffer (future)

## 7. Planned Improvements
- Wi-Fi sync to remote ground station
- Sensor fusion filtering
- Real-time web dashboard via ESP32
