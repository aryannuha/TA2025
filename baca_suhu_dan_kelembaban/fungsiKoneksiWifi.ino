void setup_wifi() {
  WiFi.mode(WIFI_STA);
  
  // Connect to WiFi
  Serial.println("Connecting to WiFi...");
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  IPAddress gateway = WiFi.gatewayIP();
  Serial.println("\nWiFi Connected!");
  Serial.print("Gateway: "); Serial.println(gateway);
  WiFi.config(local_IP, gateway, subnet);
  Serial.print("IP Statis: "); Serial.println(WiFi.localIP());
}