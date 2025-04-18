void readDHT() {
  tempDeg = dht.readTemperature();// Suhu dalam Celsius
  hum = dht.readHumidity();       // Kelembaban dalam %
}