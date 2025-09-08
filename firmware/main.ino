#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <RTClib.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>

// --- Pin Definitions ---
#define SD_CS 5         
#define BUTTON_START 12 
#define BUTTON_STOP 14  

// --- Globals ---
bool isLogging = false;
File logFile;

RTC_DS3231 rtc;
Adafruit_MPU6050 mpu;

void setup() {
  Serial.begin(115200);

  pinMode(BUTTON_START, INPUT_PULLUP);
  pinMode(BUTTON_STOP, INPUT_PULLUP);

  // --- SD Init ---
  if (!SD.begin(SD_CS)) {
    Serial.println("SD card init failed!");
    while (true);
  }
  Serial.println("SD card initialized.");

  // --- RTC Init ---
  if (!rtc.begin()) {
    Serial.println("Couldn't find RTC");
    while (true);
  }
  if (rtc.lostPower()) {
    Serial.println("RTC lost power, setting time to compile time.");
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }

  // --- IMU Init ---
  if (!mpu.begin()) {
    Serial.println("Failed to find MPU6050 chip");
    while (true);
  }
  mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
  mpu.setGyroRange(MPU6050_RANGE_500_DEG);
  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);

  // --- Create log file ---
  String filename = "/flight-log.csv";
  logFile = SD.open(filename, FILE_WRITE);
  if (logFile) {
    logFile.println("Time,AccelX,AccelY,AccelZ,GyroX,GyroY,GyroZ");
    logFile.close();
    Serial.println("Log file created.");
  } else {
    Serial.println("Failed to create log file.");
  }
}

void loop() {
  // --- Handle buttons ---
  if (digitalRead(BUTTON_START) == LOW && !isLogging) {
    isLogging = true;
    Serial.println("Logging Started");
    delay(200); // debounce
  }

  if (digitalRead(BUTTON_STOP) == LOW && isLogging) {
    isLogging = false;
    Serial.println("Logging Stopped");
    delay(200); // debounce
  }

  // --- Log Data --- 
  if (isLogging) {
    DateTime now = rtc.now();

    sensors_event_t a, g, temp;
    mpu.getEvent(&a, &g, &temp);

    logFile = SD.open("/flight-log.csv", FILE_APPEND);
    if (logFile) {
      logFile.printf("%02d:%02d:%02d,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f\n",
        now.hour(), now.minute(), now.second(),
        a.acceleration.x, a.acceleration.y, a.acceleration.z,
        g.gyro.x, g.gyro.y, g.gyro.z
      );
      logFile.close();
    }

    delay(100); // ~10 Hz logging rate
  }
} 
