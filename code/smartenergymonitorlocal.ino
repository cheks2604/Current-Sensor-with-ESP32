#include <WiFi.h>

// === CONFIGURATION ===
const char* ssid = "yourSSID";       // 🔁 Replace with your Wi-Fi name
const char* password = "yourPASS";   // 🔁 Replace with your Wi-Fi password

const int sensorPin = 34;
float sensitivity = 0.185;           // For ACS712-5A
float voltageOffset = 1.51;          // Your sensor's offset

WiFiServer server(80);

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);

  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nConnected to WiFi!");
  Serial.print("ESP32 IP Address: ");
  Serial.println(WiFi.localIP());

  server.begin();
}

void loop() {
  WiFiClient client = server.available();
  Serial.println(WiFi.status());
  if (client) {
    while (client.connected()) {
      if (client.available()) {
        client.readStringUntil('\r'); // Basic request discard
        client.flush();

        int rawValue = analogRead(sensorPin);
        float voltage = (rawValue / 4095.0) * 3.3;
        float current = (voltage - voltageOffset) / sensitivity;

        // === Send basic HTML page with readings ===
        client.println("HTTP/1.1 200 OK");
        client.println("Content-Type: text/html");
        client.println("Connection: close");
        client.println();
        client.println("<!DOCTYPE html><html><head>");
        client.println("<meta http-equiv='refresh' content='1'>"); // Auto-refresh every 1s
        client.println("<title>Current Sensor Readings</title></head><body>");
        client.println("<h2>ESP32 Current Sensor</h2>");
        client.print("<p>Raw Value: "); client.print(rawValue); client.println("</p>");
        client.print("<p>Voltage: "); client.print(voltage, 3); client.println(" V</p>");
        client.print("<p>Current: "); client.print(current, 3); client.println(" A</p>");
        client.println("</body></html>");
        break;
      }
    }
    delay(1);
    client.stop();
  }
}
