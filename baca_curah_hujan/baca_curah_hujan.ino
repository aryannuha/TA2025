// 15 April 2025
// Program akuisisi data CURAH HUJAN
// Program kirim ke MQTT
// CURAH HUJAN dibaca melalui sensor anemometer pada pin GPIO4 yang disimpan pada variabel 
// rainfall, dikirim ke MQTT dalam bentuk string melalui variabel
// rainfall_Str
// Hasil pembacaan raw data dan scalling dikalibrasi dalam fungsi kalibrasi
// Hasil kalibrasi difilter setiap 5 data sekali dengan SIMPLE MOVING AVERAGE
// dalam fungsi filtrasi
// proses dan pengirim ke RIDAM LOCALE A dan RIDAM CLOUD A belum terealisasi
// menunggu modul RIDAM LOCAL A dan RIDAM CLOUD A diselesaikan
// MAC ADDRESS CURAH HUJAN F4:65:0B:59:DA:08

//Curah hujan adalah jumlah air yang jatuh di permukaan tanah selama periode tertentu yang diukur dengan satuan tinggi milimeter (mm) di atas permukaan horizontal.
//Curah hujan 1 mm adalah jumlah air hujan yang jatuh di permukaan per satuan luas (m2) dengan volume sebanyak 1 liter tanpa ada yang menguap, meresap atau mengalir.
// Lebih lengkap silahkan dipelajari lebih lanjut disini https://www.climate4life.info/2015/12/hujan-1-milimeter-yang-jatuh-di-jakarta.html

//Perhitungan rumus
//Tinggi curah hujan (cm) = volume yang dikumpulkan (mL) / area pengumpulan (cm2)
//Luas kolektor (Corong) 8,4cm x 3,7cm = 31,08 cm2
//Koleksi per ujung tip kami dapat dengan cara menuangkan 100ml air ke kolektor kemudian menghitung berapa kali air terbuang dari tip,
//Dalam perhitungan yang kami lakukan air terbuang sebanyak 70 kali. 100ml / 70= 1.42mL per tip.
//Jadi 1 tip bernilai 1.42 / 31.08 = 0,04cm atau 0.40 mm curah hujan.

// PENTING
// Nilai kalibrasi yang kami lakukan berlaku untuk semua sensor curah hujan yang kami jual tentu Anda dapat melakukan kalibrasi ulang sendiri jika dibutuhkan.

// Library
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <PubSubClient.h>
#include <HTTPClient.h>

// jumlah data yang dijadikan MA
#define WINDOW_SIZE 5    // Number of readings to average

// variabel array untuk menyimpan 5 data
float rainfallWindow[WINDOW_SIZE];
int bufferIndex = 0;
int count = 0;
float avgRainfall;

// deklarasi variabel kalibrasi
float yRainfall;
float awal_ukurRainfall, awal_sensorRainfall,
      akhir_ukurRainfall, akhir_sensorRainfall;
float aRainfall;
float cRainfall;

const int pin_interrupt = 4; 
long int jumlah_tip = 0;
long int temp_jumlah_tip = 0;
float rainfall = 0.00;
float milimeter_per_tip = 0.40;
volatile boolean flag = false;

// Buffer untuk menyimpan string hasil konversi kecepatan angin
char rainfall_Str[10];  

// Ganti dengan informasi HiveMQ Anda
const char* mqtt_server = "9a59e12602b646a292e7e66a5296e0ed.s1.eu.hivemq.cloud";  // Broker URL
const int mqtt_port = 8883;  // Gunakan 8883 untuk TLS
const char* mqtt_user = "testing";  // Username HiveMQ
const char* mqtt_password = "Testing123";  // Password HiveMQ

// Structure untuk kirim ke ALARM
struct struct_data_sensor{
  int kodeModul;
  int kodeVariabelRainfall;
  float dataRainfall;
};
struct_data_sensor rainfallData;

// Variabel untuk millis()
unsigned long previousMillis = 0;
const long interval = 1000; // Baca data setiap 10000 ms (10 detik)

WiFiClientSecure espClient;
PubSubClient client(espClient);

void setup() {
  // Inisialisasi Serial
  Serial.begin(115200); 

  // inisialisasi pin interupt
  pinMode(pin_interrupt, INPUT);
  attachInterrupt(digitalPinToInterrupt(pin_interrupt), hitung_curah_hujan, FALLING); // Akan menghitung tip jika pin berlogika dari HIGH ke LOW

  // Inisialisasi wifi
  setup_wifi();

  // Gunakan sertifikat root agar bisa konek ke broker TLS
  espClient.setInsecure();  

  // Set MQTT
  client.setServer(mqtt_server, mqtt_port);    
}

void loop() {
  // Deklarasi variabel baca millis sekarang
  unsigned long currentMillis = millis();

  // loop mqtt
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  // Update pembacaan sesuai interval
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis; // Update waktu terakhir pembacaan

    // Panggil fungsi baca Kecepatan Angin
    readHujan();    

    // Panggil fungsi proses
    // proses();

    // Panggil fungsi sendToRidam
    // sendToRidam(1, tempDeg);
    // sendToRidam(2, hum);

    // kirim semua data
    // sendData();

    // Panggil fungsi menampilkan pembacaan di serial monitor
    displaySerial();
  }
}

