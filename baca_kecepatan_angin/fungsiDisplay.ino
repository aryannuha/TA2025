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
  Serial.print("Wind Speed: ");
  Serial.print(windSpeed);
  Serial.println(" m/s");
}

void displayBeforeSend(String data) {
  Serial.print("Data yang dikirim: ");
  Serial.println(data);
}