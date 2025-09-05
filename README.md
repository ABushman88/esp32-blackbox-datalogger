# esp32-blackbox-datalogger
This project is a prototype of an aerospace black box designed to log flight-critical telemetry such as altitude, acceleration, orientation, and GPS location to a mircoSD card in real time using an ESP32.

## Features (Planned)
- Real-time sensor data acquisition (IMU, barometer)
- Timestamped logging using DS3231 RTC
- Push-button controlled logging start/stop
- Power failure resilience
- CSV data logging for easy analysis

## Hardware
- ESP32 WROOM Module
- DS3231 Real-Time Clock
- MircoSD Card Module
- MPU6050 / BMP280 / GPS Module
- Pushbuttons, LEDs
- Optional: Battery backup circuit

## Folder Structure
```
project-root/
├── firmware/
│   └── main.ino
├── docs/
│   └── system-design.md
├── logs/
│   └── sample-flight.csv
├── images/
│   └── wiring-diagram.png
├── .gitignore
├── README.md
└── LICENSE
```

## Data Output Format (Planned)
| Time (UTC) | Altitude (m) | Accel (m/s²) | Gyro (°/s) |
|------------|--------------|--------------|------------|
| HH:MM:SS   | 1234.56      | X,Y,Z        | X,Y,Z      |

## Status
Hardware is being assembled.
Firmware and system design documentation in progress.
