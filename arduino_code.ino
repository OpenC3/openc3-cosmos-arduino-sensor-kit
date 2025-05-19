#include "Arduino_SensorKit.h"

const uint8_t BMP280_ID = 1;

void setup() {
  Serial.begin(9600);
  Pressure.begin();
}

void loop() {
  // Read sensors
  float temp     = Pressure.readTemperature();
  float pressure = Pressure.readPressure();
  float altitude = Pressure.readAltitude();

  const uint16_t PAYLOAD_LEN = 13; // 4 bytes * 3 (sensor readings) + 1 byte for packet ID
  uint8_t pkt[PAYLOAD_LEN];
  int idx = 0;

  // Add packet ID to the beginning of the packet
  pkt[idx++] = BMP280_ID & 0xFF;

  // Convert floats to bytes using pointer method
  uint8_t *tempBytes     = (uint8_t *)&temp;
  uint8_t *pressureBytes = (uint8_t *)&pressure;
  uint8_t *altitudeBytes = (uint8_t *)&altitude;
  
  // Copy temperature bytes
  for (int j = 0; j < 4; j++) pkt[idx++] = tempBytes[j];
  
  // Copy pressure bytes
  for (int j = 0; j < 4; j++) pkt[idx++] = pressureBytes[j];
  
  // Copy altitude bytes
  for (int j = 0; j < 4; j++) pkt[idx++] = altitudeBytes[j];

  // Send the packet
  Serial.write(pkt, PAYLOAD_LEN);

  // Wait one second
  delay(1000);
}