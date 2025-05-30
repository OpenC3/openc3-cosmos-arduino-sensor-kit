#include "Arduino_SensorKit.h"

#define SYNC_PATTERN 0xABCD1234
const uint8_t BMP280_ID = 1;

#define LED 6

// Global variables
bool ledState = false;

void setup() {
  Serial.begin(9600);
  Pressure.begin();
  pinMode(LED, OUTPUT);
  digitalWrite(LED, LOW);
}

struct __attribute__((packed)) Payload {
  uint8_t id;
  float temp;
  float pressure;
  float altitude;
};

void sendTelemetry() {
  // Read sensors
  float temp     = Pressure.readTemperature();
  float pressure = Pressure.readPressure();
  float altitude = Pressure.readAltitude();

  Payload data = { BMP280_ID, temp, pressure, altitude };

  // Holds the actual packet data
  int PAYLOAD_LEN = sizeof(uint32_t) + 1 + sizeof(Payload);
  uint8_t buffer[PAYLOAD_LEN];

  // Sets sync data
  uint32_t sync = SYNC_PATTERN;
  memcpy(buffer, &sync, sizeof(sync));

  // Sets the length of data
  uint8_t length = sizeof(Payload); // 13
  buffer[sizeof(sync)] = length;

  // Sets the data
  memcpy(buffer + sizeof(sync) + 1, &data, sizeof(Payload));

  // Send the packet
  Serial.write(buffer, PAYLOAD_LEN);
}

void receiveCommands() {
  while (Serial.available() > 0) {
    int command = Serial.read();
    if (command == 1) {
      digitalWrite(LED, HIGH);
    } else if (command == 0) {
      digitalWrite(LED, LOW);
    }
  }
}


void loop() {
  receiveCommands();
  sendTelemetry();
  // Wait one second
  delay(1000);
}
