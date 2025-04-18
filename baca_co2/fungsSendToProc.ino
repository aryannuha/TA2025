// Fungsi bantu untuk mengubah int jadi string 2 digit (dengan 0 di depan jika perlu)
// String formatTwoDigit(int value) {
//   if (value < 10) return "0" + String(value);
//   else return String(value);
// }

// Fungsi untuk mengirim data
void sendData() {
 // Buat dokumen JSON
  StaticJsonDocument<200> doc;
  doc["kodeModul"] = "04";
  doc["kodeVariabel"] = 11;
  doc["kodeData"] = ppm;
  doc["kodeAlarm"] = 0;
  doc["berita"] = "Normal"; // ArduinoJson tidak mendukung char langsung

  // Serialize ke buffer
  char buffer[256];
  size_t n = serializeJson(doc, buffer);

  // Kirim lewat UDP
  udp.writeTo((const uint8_t*)buffer, n, destinationIP, 1234);

  Serial.print("Dikirim ke ");
  Serial.println(destinationIP);
  Serial.print("Payload: ");
  Serial.println(buffer);

  delay(1000);
}