#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <RTClib.h>
#include <Adafruit_BMP280.h>
#include <MPU6050.h>

RTC_DS3231 rtc;
File logFile;

const int chipSelect = 5; // Adjust to your SD module CS pin
const int startButtonPin = 12;
const int stopButtonPin = 14;
bool logging = false;

void setup() {
  Serial.begin(115200);

  pinMode(startButtonPin, INPUT_PULLUP);
  pinMode(stopButtonPin, INPUT_PULLUP);

  Wire.begin();
  
  if (!rtc.begin()) {
    Serial.println("RTC initalized failed");
    while(1);
  }

  if (!SD.begin(chipSelect)) {
    Serial.println("SD initalized failed");
    while(1);
  }

  imu.initialize();
  bmp.begin();

  Serial.println("System Ready. Awaiting start...");
}

void loop() {
  if (digitalRead(startButtonPin) == LOW && !Logging) {
    startLogging();
  }

  if (digitalRead(stopButtonPin) == LOW && !Logging) {
    stopLogging();
  }

  if (logging) {
    logData();
  }

  delay(100); // ~10 Hz sample rate
}

void startLogging() {
  String filename = "/flight-" + getTimeStamp() + ".csv";
  logFile = SD.open(filename, FILE_WRITE);
  if (logFile) {
    logFile.println("Time,Altitude,AccelX,AccelY,AccelZ,GyroX,GyroY,GyroZ");
    logging = true;
    Serial.println("Logging Started.");
  } else {
    Serial.println("Failed to open log file.");
  }
}

void stopLogging() {
  if (logFile) {
    logFile.close();
    Serial.println("Logging stopped.");
  }
  logging = false;
}

void logData() {
  if (!logFile) return;

  DataTime now = rtc.now();
  int16_t ax, ay, az, gx, gy, gz;
  imu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);

  float altitude = bmp.readAltitude();

  String line = String(now.timestamp(DateTime::TIMESTAMP_TIME)) + "," +
                String(altitude) + "," +
                String(ax) + "," + String(ay) + "," + String(az) + "," +
                String(gx) + "," + String(gy) + "," + String(gz);

  logFile.println(line);
  logFile.flush();
}

String getTimeStamp() {
  DateTime now = rtc.now();
  char buffer[20];
  sprintf(buffer, "%04d%02d%02d-%02d%02d",
          now.year(), now.month(), now.day(),
          now.hour(), now.minute());
  return String(buffer);
}
