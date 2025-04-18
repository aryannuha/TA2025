// Fungsi sendToRidam untuk mengonversi data menjadi string dan menampilkannya
// void sendToRidam(int id, float value) {
//   String data = String(id) + "," + String(value); // Konversi ke string
//   displayBeforeSend(data);  // Menampilkan data sebelum dikirim
//   // Kirim data ke mikrokontroler lain, misalnya melalui serial
// }

// Fungsi bantu untuk mengubah int jadi string 2 digit (dengan 0 di depan jika perlu)
String formatTwoDigit(int value) {
  if (value < 10) return "0" + String(value);
  else return String(value);
}

// Fungsi untuk mengirim data
void sendToProc(String namaSensor, String alamat, struct_data_sensor data) {
  // Format masing-masing bagian
  String kodeModulStr = formatTwoDigit(data.kodeModul);
  String kodeVarWindspeedStr = formatTwoDigit(data.kodeVariabelWindspeed);

  // Gabungkan semua jadi satu string
  String dataString = kodeModulStr + kodeVarWindspeedStr + " " +
                      String(data.dataWindspeed, 2);

  String pesan = namaSensor + " " + alamat + " " + dataString;

  Serial.println(pesan);

  // Kirim via serial hardware dengan ttSerial
  // windSerial.println(pesan);
}

