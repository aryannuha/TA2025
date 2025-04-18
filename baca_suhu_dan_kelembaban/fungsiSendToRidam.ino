// Fungsi sendToRidam untuk mengonversi data menjadi string dan menampilkannya
// void sendToRidam(int id, float value) {
//   String data = String(id) + "," + String(value); // Konversi ke string
//   displayBeforeSend(data);  // Menampilkan data sebelum dikirim
//   // Kirim data ke mikrokontroler lain, misalnya melalui serial
// }

// Fungsi untuk mengirim data
void sendToProc() {
 // suhu
  // Buat dokumen JSON
  StaticJsonDocument<200> doc;
  doc["kodeModul"] = "02";
  doc["kodeVariabel"] = 11;
  doc["kodeData"] = tempDeg;
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

  // kelembaban
  // Buat dokumen JSON
  StaticJsonDocument<200> doc1;
  doc1["kodeModul"] = "03";
  doc1["kodeVariabel"] = 12;
  doc1["kodeData"] = hum;
  doc1["kodeAlarm"] = 1;
  doc1["berita"] = "Danger"; // ArduinoJson tidak mendukung char langsung

  // Serialize ke buffer
  char buffer1[256];
  size_t n1 = serializeJson(doc1, buffer1);

  // Kirim lewat UDP
  udp.writeTo((const uint8_t*)buffer1, n1, destinationIP, 1234);

  Serial.print("Dikirim ke ");
  Serial.println(destinationIP);
  Serial.print("Payload1: ");
  Serial.println(buffer1);
  delay(1000);
}

