// ─────────────────────────────────────────
// ESP32 Wi-Fi Connect + IP Print
// Step 1 of your wireless OTA project
// ─────────────────────────────────────────

#include <WiFi.h>

// ── Change these to your network credentials ──
const char* ssid     = "Airtel_m000_6317";
const char* password = "Air@59781";

void setup() {
  Serial.begin(115200);
  delay(500);

  Serial.println("\n[WiFi] Connecting to: " + String(ssid));
  WiFi.mode(WIFI_STA);          // Station mode (client, not AP)
  WiFi.begin(ssid, password);

  // Wait until connected — print a dot every 500 ms
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\n[WiFi] Connected!");
  Serial.print("[WiFi] IP Address : ");
  Serial.println(WiFi.localIP());
  Serial.print("[WiFi] MAC Address: ");
  Serial.println(WiFi.macAddress());
  Serial.print("[WiFi] Signal (RSSI): ");
  Serial.print(WiFi.RSSI());
  Serial.println(" dBm");
}

void loop() {
  // Reconnect automatically if connection drops
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("[WiFi] Lost connection, reconnecting...");
    WiFi.reconnect();
    delay(5000);
  }
}