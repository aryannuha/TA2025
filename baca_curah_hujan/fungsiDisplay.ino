void displaySerial()
{
  Serial.print("Jumlah tip=");
  Serial.print(jumlah_tip);
  Serial.println(" kali ");
  Serial.print("Curah hujan=");
  Serial.print(rainfall, 1);
  Serial.print(" mm");
  Serial.println();
}
