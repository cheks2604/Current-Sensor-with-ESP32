const int sensorPin = 34; // GPIO34 (Analog input only)
float sensitivity = 1; // For ACS712-5A, 185mV per Amp
float voltageOffset = 1.511; // Adjusted to my sensor

void setup() {
  Serial.begin(115200);

}

void loop() {
  int rawValue = analogRead(sensorPin);
  float voltage = (rawValue / 4095.0) * 3.3; // For 12-bit ADC on ESP32
  float current = (voltage - voltageOffset) / sensitivity;

  Serial.print("Raw: "); Serial.print(rawValue);
  Serial.print(" | Voltage: "); Serial.print(voltage, 3);
  Serial.print(" V | Current: "); Serial.print(current, 3);
  Serial.println(" A");

  delay(1000);
}
