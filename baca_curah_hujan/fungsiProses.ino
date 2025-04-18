void proses(){
  // Konversi integer ke string
  // dtostrf(tempDeg, 6, 2, datasuhu_Str);  
  // dtostrf(hum, 6, 2, datakelembaban_Str);  
  
  // fungsi kalibrasi nilai pembacaan sensor
  kalibrasi();

  // Store in circular buffer
  rainfallWindow[bufferIndex] = yRainfall;
  bufferIndex = (bufferIndex + 1) % WINDOW_SIZE;

  // Track count of valid readings
  if (count < WINDOW_SIZE) count++;

  // fungsi filtrasi hasil kalibrasi menggunakan MA
  avgRainfall = filtrasi(rainfallWindow, count); 

  rainfallData.kodeModul = 4;
  rainfallData.dataRainfall = avgRainfall;

  // fungsi kondisi kodeVariabel
  kondisi();
}

void kalibrasi(){
  // perhitungan linearitas
  aRainfall = (akhir_ukurRainfall - akhir_sensorRainfall)/(awal_ukurRainfall - awal_sensorRainfall);

  // perhitungan kalibrasi
  yRainfall = aRainfall * rainfall + cRainfall;
}

float filtrasi(float* data, int size){
  float sum = 0.0;
  for (int i = 0; i < size; i++) {
    sum += data[i];
  }
  return sum / size;
}

void kondisi(){
 // kondisi kodeVariabel suhu
  if(rainfallData.dataRainfall > 26 && rainfallData.dataRainfall < 35){
    rainfallData.kodeVariabelRainfall = 13;
  }else if(rainfallData.dataRainfall >= 0 && rainfallData.dataRainfall < 15){
    rainfallData.kodeVariabelRainfall = 14;
  }else if(rainfallData.dataRainfall >= 15 && rainfallData.dataRainfall <= 26){
    rainfallData.kodeVariabelRainfall = 15;
  }else if(rainfallData.dataRainfall >= 35 && rainfallData.dataRainfall < 45){
    rainfallData.kodeVariabelRainfall = 16;
  }else{
    rainfallData.kodeVariabelRainfall = 17;
  }
}