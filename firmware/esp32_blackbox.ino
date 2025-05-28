#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <RTClib.h>
#include <TinyGPS++.h>
#include <HardwareSerial.h>

RTC_DS3231 rtc;
TinyGPSlus gps;
HardwareSerial gpsSerial(1); // Use UART1 (pins vary per board)

File logFile;

const int chipSelect = 5; // Adjust to your SD module CS pin
const int startButtonPin = 12;
const int stopButtonPin = 14;
bool logging = false;

void setup() {
  Serial.being(115200);
  gpsSerial.begin(9600, SERIAL_8N1, 16, 17); // RX, TX (adjust pins)

  pinMode(startButtonPin, INPUT_PULLUP)
  pinMode(stopButtonPin, INPUT_PULLUP)

  Wire.begin();
  if (!rtc.begin()) {
    Serial.println("RTC initalized failed");
    while(1);
  }

  if (!SD.begin(chipselect)) {
    Serial.println("SD initalized failed");
    while(1);
  }

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

  while (gpsSerial.available()) {
    gps.encode(gpsSerial.read());
  }
}

void startLogging() {
  String filename = "/flight-" + getTimeStamp() + ".csv";
  logFile = SD.open(filename, FILE_WRITE);
  if (logFile) {
    logFile.println("Time,Lat,Lon"); // Placeholders headers
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
  if (gps.location.isUpdated()) {
    String line = String(now.timestamp(DateTime::TIMESTAMP_TIME)) + "," +
                  String(gps.location.lat(), 6) + "," +
                  String(gps.location.lng(), 6);
    logFile.println(line);
    logFile.flush();
  }
}

String getTimeStamp() {
  DateTime now = rtc.now();
  char buffer[20];
  sprintf(buffer, "%04d%02d%02d-%02d%02d",
          now.year(), now.month(), now.day(),
          now.hour(), now.minute());
  return String(buffer);
}
