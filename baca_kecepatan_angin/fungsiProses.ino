void proses(){
  // Konversi integer ke string
  // dtostrf(tempDeg, 6, 2, datasuhu_Str);  
  // dtostrf(hum, 6, 2, datakelembaban_Str);  
  
  // fungsi kalibrasi nilai pembacaan sensor
  kalibrasi();

  // Store in circular buffer
  windWindow[bufferIndex] = yWind;
  bufferIndex = (bufferIndex + 1) % WINDOW_SIZE;

  // Track count of valid readings
  if (count < WINDOW_SIZE) count++;

  // fungsi filtrasi hasil kalibrasi menggunakan MA
  avgWind = filtrasi(windWindow, count); 

  windspeedData.kodeModul = 4;
  windspeedData.dataWindspeed = avgWind;

  // fungsi kondisi kodeVariabel
  kondisi();
}

void kalibrasi(){
  // perhitungan linearitas
  aWind = (akhir_ukurWind - akhir_sensorWind)/(awal_ukurWind - awal_sensorWind);

  // perhitungan kalibrasi
  yWind = aWind * windSpeed + cWind;
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
  if(windspeedData.dataWindspeed > 26 && windspeedData.dataWindspeed < 35){
    windspeedData.kodeVariabelWindspeed = 13;
  }else if(windspeedData.dataWindspeed >= 0 && windspeedData.dataWindspeed < 15){
    windspeedData.kodeVariabelWindspeed = 14;
  }else if(windspeedData.dataWindspeed >= 15 && windspeedData.dataWindspeed <= 26){
    windspeedData.kodeVariabelWindspeed = 15;
  }else if(windspeedData.dataWindspeed >= 35 && windspeedData.dataWindspeed < 45){
    windspeedData.kodeVariabelWindspeed = 16;
  }else{
    windspeedData.kodeVariabelWindspeed = 17;
  }
}