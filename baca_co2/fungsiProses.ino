/*****************************  MQGetPercentage **********************************
Input:   volts   - SEN-000007 output measured in volts
         pcurve  - pointer to the curve of the target gas
Output:  ppm of the target gas
Remarks: By using the slope and a point of the line. The x(logarithmic value of ppm)
         of the line could be derived if y(MG-811 output) is provided. As it is a
         logarithmic coordinate, power of 10 is used to convert the result to non-logarithmic
         value.
************************************************************************************/
void proses()
{
  // if ((volts / DC_GAIN) >= ZERO_POINT_VOLTAGE) {
  //   return -1;
  // } else {
  ppm = pow(10, ((volts / DC_GAIN) - CO2Curve[1]) / CO2Curve[2] + CO2Curve[0]);

  // fungsi kalibrasi nilai pembacaan sensor
  kalibrasi();

  // Store in circular buffer
  co2Window[bufferIndex] = yCo2;
  bufferIndex = (bufferIndex + 1) % WINDOW_SIZE;

  // Track count of valid readings
  if (count < WINDOW_SIZE) count++;

  // fungsi filtrasi hasil kalibrasi menggunakan MA
  avgCo2 = filtrasi(co2Window, count); 

  co2Data.kodeModul = 3;
  co2Data.dataCO2 = avgCo2;

  // }
}

void kalibrasi(){
  // perhitungan linearitas
  aCo2 = (akhir_ukurCo2 - akhir_sensorCo2)/(awal_ukurCo2 - awal_sensorCo2);

  // perhitungan kalibrasi
  yCo2 = aCo2 * ppm + cCo2;
}

float filtrasi(float* data, int size){
  float sum = 0.0;
  for (int i = 0; i < size; i++) {
    sum += data[i];
  }
  return sum / size;
}