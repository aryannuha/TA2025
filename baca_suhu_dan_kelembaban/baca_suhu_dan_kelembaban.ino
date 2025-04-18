// 18 April 2025
// Program akuisisi data DHT22 Suhu dan Kelembaban Udara
// Program kirim ke RIDAM CLOUD melalui jaringan lokal
// Suhu dan kelembaban dibaca melalui sensor DHT22 di pin 4 yang disimpan pada variabel 
// Mengirim data ke RIDAM CLOUD melalui format JSON
// proses dan pengirim ke RIDAM LOCALE A belum terealisasi
// menunggu modul RIDAM LOCAL A diselesaikan
// MAC ADDRESS T&H INDOOR F4:65:0B:54:9C:74
// MAC ADDRESS T&H OUTDOOR 8C:4F:00:29:56:88

// Library
#include <DHT.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <HTTPClient.h>
#include <AsyncUDP.h>
#include <ArduinoJson.h>

// Deklarasi pin DHT22
#define DHT_PIN 4
#define DHT_TYPE DHT22

// Ganti dengan SSID dan Password WiFi Anda
const char* ssid = "Workshop 1";
const char* password = "eForacimenyan";

// IP statis ESP32 ini
IPAddress local_IP(192, 168, 0, 231);
IPAddress subnet(255, 255, 255, 0);

// IP tujuan (ESP32 lain)
IPAddress destinationIP(192, 168, 0, 120);

// deklarasi variabel kalibrasi
float ySuhu, yKelembaban;
float awal_ukurSuhu, awal_sensorSuhu,
      akhir_ukurSuhu, akhir_sensorSuhu;
float awal_ukurKelembaban, awal_sensorKelembaban,
      akhir_ukurKelembaban, akhir_sensorKelembaban;
float aSuhu, aKelembaban;
float cSuhu, cKelembaban;

// jumlah data yang dijadikan MA
#define WINDOW_SIZE 5    // Number of readings to average

// variabel array untuk menyimpan 5 data
float tempWindow[WINDOW_SIZE];
float humWindow[WINDOW_SIZE];
int bufferIndex = 0;
int count = 0;
float avgTemp, avgHum;

// Deklarasi variabel global
float tempDeg = 0;
float hum = 0;

// Buffer untuk menyimpan string hasil konversi suhu
char datasuhu_Str[10];  
// Buffer untuk menyimpan string hasil konversi kelembaban
char datakelembaban_Str[10];  

// Ganti dengan informasi HiveMQ Anda
const char* mqtt_server = "9a59e12602b646a292e7e66a5296e0ed.s1.eu.hivemq.cloud";  // Broker URL
const int mqtt_port = 8883;  // Gunakan 8883 untuk TLS
const char* mqtt_user = "testing";  // Username HiveMQ
const char* mqtt_password = "Testing123";  // Password HiveMQ

// Structure untuk kirim ke ALARM
struct struct_data_sensor{
  int kodeModul;
  int kodeVariabelSuhu;
  int kodeVariabelKelembaban;
  float dataSuhu;
  float dataKelembaban;
};
struct_data_sensor dhtData;

// Variabel untuk millis()
unsigned long previousMillis = 0;
const long interval = 1000; // Baca data setiap 10000 ms (10 detik)

DHT dht(DHT_PIN,DHT_TYPE);
WiFiClientSecure espClient;
PubSubClient client(espClient);
HardwareSerial ttSerial(2);
AsyncUDP udp;

void setup() {
  // Inisialisasi Serial
  Serial.begin(115200); 
  // ttSerial.begin(TH_BAUD, SERIAL_8N1, RXD2, TXD2);
 
  // Inisialisasi sensor DHT22
  dht.begin();     
  
  // Inisialisasi wifi
  setup_wifi();

  // // Gunakan sertifikat root agar bisa konek ke broker TLS
  // espClient.setInsecure();  

  // // Set MQTT
  // client.setServer(mqtt_server, mqtt_port);    
}

void loop() {
  // Deklarasi variabel baca millis sekarang
  unsigned long currentMillis = millis();

  // loop mqtt
  // if (!client.connected()) {
  //   reconnect();
  // }
  // client.loop();

  // Update pembacaan sesuai interval
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis; // Update waktu terakhir pembacaan

    // Panggil fungsi baca DHT22
    readDHT();    

    // Panggil fungsi proses
    // proses();

    // Panggil fungsi sendToRidam
    // sendToRidam(1, tempDeg);
    // sendToRidam(2, hum);

    // kirim semua data
    sendData();

    // Panggil fungsi menampilkan pembacaan di serial monitor
    // displaySerial();
  }
}

