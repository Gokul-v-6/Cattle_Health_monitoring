#include <WiFi.h>
#include <ThingSpeak.h>
#include <Wire.h>

#include <Adafruit_MLX90614.h>
#include <MPU6050_light.h>
#include <MAX30105.h>
#include "heartRate.h"

// WiFi
const char* ssid = "vivo Y56 5G";
const char* password = "hello12345";

// ThingSpeak
WiFiClient client;
unsigned long channelID = 2978485;
const char* writeAPIKey = "0ASEAAAJS5B4N8K3";

// Sensors
Adafruit_MLX90614 mlx = Adafruit_MLX90614();
MPU6050 mpu(Wire);
MAX30105 particleSensor;

// Manual Inputs
int age = 5;   // Set manually or from UI
int sex = 1;   // 0 = Male, 1 = Female

void setup() {
  Serial.begin(115200);
  Wire.begin();
  
  // Connect WiFi
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500); Serial.print(".");
  }
  Serial.println("\nConnected!");

  ThingSpeak.begin(client);

  // Initialize Sensors
  if (!mlx.begin()) {
    Serial.println("MLX90614 not detected!");
  }

  mpu.begin();
  mpu.calcOffsets(true, true);

  if (!particleSensor.begin(Wire)) {
    Serial.println("MAX30102 not found!");
    while (1);
  }
  particleSensor.setup();
  particleSensor.setPulseAmplitudeRed(0x2A);
  particleSensor.setPulseAmplitudeIR(0x2A);
  particleSensor.setPulseAmplitudeGreen(0); // Turn off green
}

float getMotionLevel() {
  mpu.update();
  return abs(mpu.getAccX()) + abs(mpu.getAccY()) + abs(mpu.getAccZ());
}

int getPulseRate() {
  const int measureDuration = 10000; // 10 seconds
  const int samplingDelay = 20;
  int bpmCount = 0;
  long lastBeat = 0;
  int bpmSum = 0;

  Serial.println("Measuring BPM...");

  unsigned long start = millis();
  while (millis() - start < measureDuration) {
    long irValue = particleSensor.getIR();

    if (irValue > 10000) {
      if (checkForBeat(irValue)) {
        long now = millis();
        if (lastBeat != 0) {
          int delta = now - lastBeat;
          int bpm = 60000 / delta;

          if (bpm > 40 && bpm < 180) {
            bpmSum += bpm;
            bpmCount++;
            Serial.print("Beat detected: ");
            Serial.println(bpm);
          }
        }
        lastBeat = now;
      }
    }

    delay(samplingDelay);
  }

  if (bpmCount > 0) {
    int avgBPM = bpmSum / bpmCount;
    Serial.print("Average BPM: ");
    Serial.println(avgBPM);
    return avgBPM;
  } else {
    Serial.println("No pulse detected.");
    return 0;
  }
}

void loop() {
  float temp = mlx.readObjectTempC();
  float motion = getMotionLevel();
  int pulse = getPulseRate();

  Serial.println("--- SENDING TO THINGSPEAK ---");
  Serial.print("Temp: "); Serial.println(temp);
  Serial.print("Motion: "); Serial.println(motion);
  Serial.print("Pulse: "); Serial.println(pulse);
  Serial.print("Age: "); Serial.println(age);
  Serial.print("Sex: "); Serial.println(sex);

  ThingSpeak.setField(1, pulse);
  ThingSpeak.setField(2, temp);
  ThingSpeak.setField(3, motion);
  ThingSpeak.setField(4, age);
  ThingSpeak.setField(5, sex);

  int res = ThingSpeak.writeFields(channelID, writeAPIKey);
  if (res == 200) {
    Serial.println("Upload successful!");
  } else {
    Serial.print("Upload failed. Error code: ");
    Serial.println(res);
  }

  delay(20000); //20 sec
}
