void proses(){
  // Konversi integer ke string
  // dtostrf(tempDeg, 6, 2, datasuhu_Str);  
  // dtostrf(hum, 6, 2, datakelembaban_Str); 

  // fungsi kalibrasi nilai pembacaan sensor
  kalibrasi();

  // Store in circular buffer
  tempWindow[bufferIndex] = ySuhu;
  humWindow[bufferIndex] = yKelembaban;
  bufferIndex = (bufferIndex + 1) % WINDOW_SIZE;

  // Track count of valid readings
  if (count < WINDOW_SIZE) count++;

  // fungsi filtrasi hasil kalibrasi menggunakan MA
  avgTemp = filtrasi(tempWindow, count); 
  avgHum = filtrasi(humWindow, count); 

  dhtData.kodeModul = 2;
  dhtData.dataSuhu = avgTemp;
  dhtData.dataKelembaban = avgHum;
  
  // kondisi kodeVariabel 
  kondisi();
}

void kalibrasi(){
  // perhitungan linearitas
  aSuhu = (akhir_ukurSuhu - akhir_sensorSuhu)/(awal_ukurSuhu - awal_sensorSuhu);
  aKelembaban = (akhir_ukurKelembaban - akhir_sensorKelembaban)/(awal_ukurKelembaban - awal_sensorKelembaban);

  // perhitungan kalibrasi
  ySuhu = aSuhu * tempDeg + cSuhu;
  yKelembaban = aKelembaban * hum + cKelembaban;
}

float filtrasi(float* data, int size){
  float sum = 0.0;
  for (int i = 0; i < size; i++) {
    sum += data[i];
  }
  return sum / size;
}

void kondisi(){
  if(dhtData.dataSuhu > 26 && dhtData.dataSuhu < 35){
    dhtData.kodeVariabelSuhu = 13;
  }else if(dhtData.dataSuhu >= 0 && dhtData.dataSuhu < 15){
    dhtData.kodeVariabelSuhu = 14;
  }else if(dhtData.dataSuhu >= 15 && dhtData.dataSuhu <= 26){
    dhtData.kodeVariabelSuhu = 15;
  }else if(dhtData.dataSuhu >= 35 && dhtData.dataSuhu < 45){
    dhtData.kodeVariabelSuhu = 16;
  }else{
    dhtData.kodeVariabelSuhu = 17;
  }

   // kondisi kodeVariabel kelembaban
  if(dhtData.dataKelembaban > 26 && dhtData.dataKelembaban < 35){
    dhtData.kodeVariabelKelembaban = 18;
  }else if(dhtData.dataKelembaban >= 0 && dhtData.dataKelembaban < 15){
    dhtData.kodeVariabelKelembaban = 19;
  }else if(dhtData.dataKelembaban >= 15 && dhtData.dataKelembaban <= 26){
    dhtData.kodeVariabelKelembaban = 20;
  }else if(dhtData.dataKelembaban >= 35 && dhtData.dataKelembaban < 45){
    dhtData.kodeVariabelKelembaban = 21;
  }else{
    dhtData.kodeVariabelKelembaban = 22;
  }
}