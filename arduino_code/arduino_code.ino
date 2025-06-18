#include "Arduino_SensorKit.h"

#define SYNC_PATTERN 0xABCD1234
#define Environment Environment_I2C

const uint8_t BMP280_ID = 1;
const uint8_t ACCELEROMETER_ID = 2;
const uint8_t DHT20_ID = 3;

#define LED 6

// Global variables
bool ledState = false;

void setup() {
  Serial.begin(9600);
  Pressure.begin();
  Accelerometer.begin();
  Environment.begin();
  pinMode(LED, OUTPUT);
  digitalWrite(LED, LOW);
}

struct __attribute__((packed)) BMP_Payload {
  uint8_t id;
  float temp;
  float pressure;
  float altitude;
};

struct __attribute__((packed)) Acc_Payload {
  uint8_t id;
  float x;
  float y;
  float z;
};

struct __attribute__((packed)) DHT_Payload {
  uint8_t id;
  float temp;
  float humidity;
};

void sendDHTTelemetry() {
  // Read sensors
  float temp = Environment.readTemperature();
  float humidity = Environment.readHumidity();

  DHT_Payload data = { DHT20_ID, temp, humidity };

  int DHT_PAYLOAD_LEN = sizeof(uint32_t) + 1 + sizeof(DHT_Payload);
  uint8_t buffer[DHT_PAYLOAD_LEN];

  // Sets sync data
  uint32_t sync = SYNC_PATTERN;
  memcpy(buffer, &sync, sizeof(sync));

  // Sets the length of data
  uint8_t length = sizeof(DHT_Payload); // 13
  buffer[sizeof(sync)] = length;

  // Sets the data
  memcpy(buffer + sizeof(sync) + 1, &data, sizeof(DHT_Payload));

  // Send the packet
  Serial.write(buffer, DHT_PAYLOAD_LEN);
} 

void sendAccTelemetry() {
  // Read sensors
  float x = Accelerometer.readX();
  float y = Accelerometer.readY();
  float z = Accelerometer.readZ();

  Acc_Payload data = { ACCELEROMETER_ID, x, y, z };

  int ACC_PAYLOAD_LEN = sizeof(uint32_t) + 1 + sizeof(Acc_Payload);
  uint8_t buffer[ACC_PAYLOAD_LEN];

  // Sets sync data
  uint32_t sync = SYNC_PATTERN;
  memcpy(buffer, &sync, sizeof(sync));

  // Sets the length of data
  uint8_t length = sizeof(Acc_Payload); // 13
  buffer[sizeof(sync)] = length;

  // Sets the data
  memcpy(buffer + sizeof(sync) + 1, &data, sizeof(Acc_Payload));

  // Send the packet
  Serial.write(buffer, ACC_PAYLOAD_LEN);
}

void sendBMPTelemetry() {
  // Read sensors
  float temp     = Pressure.readTemperature();
  float pressure = Pressure.readPressure();
  float altitude = Pressure.readAltitude();

  BMP_Payload data = { BMP280_ID, temp, pressure, altitude };

  // Holds the actual packet data
  int BMP_PAYLOAD_LEN = sizeof(uint32_t) + 1 + sizeof(BMP_Payload);
  uint8_t buffer[BMP_PAYLOAD_LEN];

  // Sets sync data
  uint32_t sync = SYNC_PATTERN;
  memcpy(buffer, &sync, sizeof(sync));

  // Sets the length of data
  uint8_t length = sizeof(BMP_Payload); // 13
  buffer[sizeof(sync)] = length;

  // Sets the data
  memcpy(buffer + sizeof(sync) + 1, &data, sizeof(BMP_Payload));

  // Send the packet
  Serial.write(buffer, BMP_PAYLOAD_LEN);
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
  sendBMPTelemetry();
  sendAccTelemetry();
  sendDHTTelemetry();
  // Wait one second
  delay(1000);
}
