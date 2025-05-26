# System Design: ESP32 Black Box Data Logger

## 1. Overview
A fault-tolerant flight data logger for capturing aerospace telemetry using ESP32, logging to microSD in real time.

## 2. System Block Diagram
![System Block Diagram](../images/ESP32%20Blackbox%20Datalogger.drawio.png)

## 3. Firmware Flow Overview
1. Initialize all peripherals (RTC, IMU, GPS, SD card).
2. Wait for "Start Logging" button.
3. Begin sensor data logging.
4. Timestamp with DS3231.
5. Write to CSV on SD card.
6. Stop when "Stop logging" is pressed.

## 4. Component Interfaces
- **ESP32 <-> DS3231**: I2C
- **ESP32 <-> MPU6050 / BMP280**: I2C(Shared)
- **ESP32 <-> GPS**: UART
- **ESP32 <-> SD card**: SPI
- **Pushbutton/LEDs**: GPIO

## 5. Logging Strategy
- CSV format: `flight-YYYYMMDD-HHMM.csv`
- Sample rate: 10hz
- Columns: Time, Altitude, Accel (XYZ), Gyro (XYZ), Lat, Lon

## 6. Fault Handling
- SD not found -> retry, blink LED
- GPS no fix -> log "N/A"
- Power loss -> flush buffer, auto-recover


## 7. Hardware Components
- **ESP32 WROOM** — main microcontroller
- **DS3231 RTC** — accurate timestamping
- **MPU6050** — IMU for acceleration and gyroscope
- **BMP280** — barometric pressure (altitude)
- **GPS Module** — latitude and longitude
- **microSD Module** — data storage
- **Pushbuttons & LEDs** — user interface
- **Battery Backup** — optional for power failure handling

## 8. Logging Workflow
1. User powers on the device
2. RTC provides time sync
3. Button press starts logging
4. Sensor data is collected at interval (e.g., 5Hz)
5. Timestamp + sensor values saved to CSV
6. Another button press stops logging

## 9. Data Format
| Time (UTC) | Altitude (m) | Accel (m/s²) | Gyro (°/s) | Latitude | Longitude |
|------------|--------------|--------------|------------|----------|-----------|
| HH:MM:SS   | 1234.56      | X,Y,Z        | X,Y,Z      | xx.xxxx  | xx.xxxx   |

## 10. Resilience Plan
- Buffered writes to handle brief power loss
- Periodic file flush to protect data
- Optional FRAM/SRAM backup buffer (future)

## 11. Planned Improvements
- Wi-Fi sync to remote ground station
- Sensor fusion filtering
- Real-time web dashboard via ESP32
