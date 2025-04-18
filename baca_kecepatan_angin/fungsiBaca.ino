void readAngin() {
  uint8_t result = node.readHoldingRegisters(0x0000, 1);

  if (result == node.ku8MBSuccess)
  {
    uint16_t rawWind = node.getResponseBuffer(0); // Mendapatkan nilai mentah
    windSpeed = rawWind / 10.0; // Sesuai datasheet: nilai dikali 0.1
    // Serial.print("Wind Speed: ");
    // Serial.print(windSpeed);
    // Serial.println(" m/s");
  }

  delay(1000);
}