// Fungsi Koneksi ke WiFi
void setup_wifi() {
  // Setting SSID dan Password
  const char* ssid = "Workshop 1"; // ganti dengan ssid sendiri
  const char* password = "eForacimenyan"; // ganti dengan password sendiri

  delay(10);
  Serial.println("Menghubungkan ke WiFi...");
  
  // Menghubungkan ke WIFI
  WiFi.begin(ssid, password);

  // Tampilkan pesan ketika tidak konek
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  // Tampilkan pesan ketika terhubung
  Serial.println("\nWiFi Terhubung!");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
}