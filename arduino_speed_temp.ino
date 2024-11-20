#include <Wire.h>  // I2C library
#include <stdint.h> // For explicit data type declaration

// MS4525DO sensor parameters
const uint8_t MS4525DAddress = 0x28;  // I2C address for the sensor
uint16_t T_dat;  // 11-bit temperature data
byte _status;    // I2C status byte

// Function to fetch temperature data
byte fetch_temperature(uint16_t &T_dat) {
  byte Temp_H, Temp_L;
  
  // Start communication with the sensor
  Wire.beginTransmission(MS4525DAddress);
  delay(100);
  Wire.requestFrom(MS4525DAddress, (uint8_t)4, (uint8_t)false); // Request 4 bytes (pressure + temperature)
  delay(100);
  
  // Read the data from the sensor
  Wire.read(); // Ignore the first byte (pressure high)
  Wire.read(); // Ignore the second byte (pressure low)
  Temp_H = Wire.read(); // Third byte contains higher bits of temperature
  Temp_L = Wire.read(); // Fourth byte contains lower bits of temperature
  
  Wire.endTransmission();

  // Extract 11-bit temperature data
  Temp_L = (Temp_L >> 5); // Remove unnecessary bits
  T_dat = (((uint16_t)Temp_H) << 3) | Temp_L;

  return _status;
}

void setup() {
  Serial.begin(9600);
  Wire.begin();

  while (!Serial) {
    delay(1);
  }
  
  Serial.println("MS4525DO Temperature Measurement");
}

void loop() {
  _status = fetch_temperature(T_dat);

  if (_status == 0) {  // If status is OK
    // Convert temperature data to degrees Celsius
    float temperature_C = ((float)T_dat / 2047.0) * 200.0 - 50.0;
    
    // Output to Serial Monitor
    Serial.print("Temperature (°C): ");
    Serial.println(temperature_C, 2);
  } else {
    Serial.println("Error reading temperature data");
  }

  delay(1000);  // Wait 1 second before next reading
}
