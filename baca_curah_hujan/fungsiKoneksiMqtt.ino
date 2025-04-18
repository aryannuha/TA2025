// Fungsi Koneksi ke broker HiveMQ
void reconnect() {
    while (!client.connected()) {
        Serial.print("Menghubungkan ke MQTT...");
        
        if (client.connect("ESP32_Publisher", mqtt_user, mqtt_password)) {
            Serial.println("Terhubung!");
        } else {
            Serial.print("Gagal, rc=");
            Serial.print(client.state());
            Serial.println(" Coba lagi dalam 5 detik...");
            delay(5000);
        }
    }
}

void sendData(){
  // Kirim ke MQTT
  client.publish("esp32/rainfall", rainfall_Str, true);  // Kirim dalam bentuk string

  // // Kirim ke RIDAM LOCALE A
  // sendToProc(alamat, message);

  // // Kirim ke RIDAM CLOUD A

    //  struc_data_sensor berbentuk array yang terdiri dari kodeModul, kodeVariabel, tempDeg
  // // Kirim ke Alarm
//   sendToProc(addressProc, struc_data_sensor);
  sendToProc("Curah Hujan", "4", rainfallData);

  // // Kirim ke Datalogger 
  // sendToProc(alamat, message);
}