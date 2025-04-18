void displayMonitor(){
  Serial.print("TEGANGAN: ");
  Serial.print(volts, 3);            // Print with 3 decimal places
  Serial.print("V           ");

  Serial.print("CO2: ");
  // if (percentage == -1) {
  //   Serial.print("< 400");
  // } else {
  //   Serial.print(percentage);
  // }

  // Serial.print(co2Data.dataCO2);
  Serial.print(ppm);

  Serial.print(" ppm");
  Serial.println();
}