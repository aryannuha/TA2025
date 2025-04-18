// Fungsi bantu untuk mengubah int jadi string 2 digit (dengan 0 di depan jika perlu)
String formatTwoDigit(int value) {
  if (value < 10) return "0" + String(value);
  else return String(value);
}

// Fungsi untuk mengirim data
void sendToProc(String namaSensor, String alamat, struct_data_sensor data) {
  // Format masing-masing bagian
  String kodeModulStr = formatTwoDigit(data.kodeModul);
  String kodeVarRainfallStr = formatTwoDigit(data.kodeVariabelRainfall);

  // Gabungkan semua jadi satu string
  String dataString = kodeModulStr + kodeVarRainfallStr + " " +
                      String(data.dataRainfall, 2);

  String pesan = namaSensor + " " + alamat + " " + dataString;

  Serial.println(pesan);

  // Kirim via serial hardware dengan ttSerial
  // windSerial.println(pesan);
}