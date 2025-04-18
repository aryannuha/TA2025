void hitung_curah_hujan()
{
  flag = true;
}

void readHujan(){
  if (flag == true) // don't really need the == true but makes intent clear for new users
  {
    rainfall += milimeter_per_tip; // Akan bertambah nilainya saat tip penuh
    jumlah_tip++;
    delay(500);
    flag = false; // reset flag
  }
  rainfall = jumlah_tip * milimeter_per_tip;
  if ((jumlah_tip != temp_jumlah_tip)) // Print serial setiap 1 menit atau ketika jumlah_tip berubah
  {
    displaySerial();
  }
  temp_jumlah_tip = jumlah_tip;
}