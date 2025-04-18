// void displaySerial() {
//   // Tampilkan ke Serial Monitor
//   Serial.print("Temperature: ");
//   Serial.print(tempDeg);
//   Serial.print(" °C | Humidity: ");
//   Serial.print(hum);
//   Serial.println(" %");
// }

void displaySerial() {
  // Tampilkan ke Serial Monitor
  Serial.print("String Temperature: ");
  Serial.print(dhtData.dataSuhu);
  Serial.print(" °C | String Humidity: ");
  Serial.print(dhtData.dataKelembaban);
  Serial.println(" %");
}

void displayBeforeSend(String data) {
  Serial.print("Data yang dikirim: ");
  Serial.println(data);
}