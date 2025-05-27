  #include <Wire.h>
  #include <SPI.h>
  #include <SD.h>

  // Placeholders libaries
  // #include <RTClib.h>
  // #include <MPU6050.h>
  // #include <TinyGPS++.h>

  #define SD_CS 5 // Adjust based on wiring
  #define BUTTON_START 12
  #define BUTTON_STOP 14

  bool isLogging = false;
  File logFile;

  void setup() {
    Serial.begin(115200);
    pinMode(BUTTON_START, INPUT_PULLUP);
    pinMode(BUTTON_STOP, INPUT_PULLUP);

    // Init SD card
    if (!SD.begin(SD_CS)) {
      Serial.println("SD card init failed!");
      while (true);
    }

    // Create new log file
    String filename = "/flight-log.csv";
    logFile = SD.open(filename, FILE_WRITE);
    if (logFile) {
      logFile.println("Time. Altitude, AccelX, AccelY, AccelZ, GyroX, GyroY, GyroZ, Lat, Lon");
      logFile.close();
      Serial.println("Log file created.");
    } else {
      Serial.println("Failed to create log file.");
    }

    // TODO: Init RTC, IMU, GPS here
  }

  void loop() {
    if (digitalRead(BUTTON_START) == LOW && !isLogging) {
      isLogging = true;
      Serial.println("Logging Started");
    } 

    if (digitalRead(BUTTON_STOP) == LOW && isLogging) {
      isLogging = false;
      Serial.println("Logging Stopped");
    }

    if (isLogging) {
      // TODO: Read sensor data here
      // TODO: Get time from RTC
      // TODO: Write data to SD
    }

    delay(100); // Adjust sample rate
  }
