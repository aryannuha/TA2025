// 15 April 2025
// Program akuisisi data Kecepatan Angin
// Program kirim ke MQTT
// Kecepatan angin dibaca melalui sensor anemometer pada pin RX 16 dan TX 17 yang disimpan pada variabel 
// windSpeed, dikirim ke MQTT dalam bentuk string melalui variabel
// windspeed_Str
// Hasil pembacaan raw data dan scalling dikalibrasi dalam fungsi kalibrasi
// Hasil kalibrasi difilter setiap 5 data sekali dengan SIMPLE MOVING AVERAGE
// dalam fungsi filtrasi
// proses dan pengirim ke RIDAM LOCALE A dan RIDAM CLOUD A belum terealisasi
// menunggu modul RIDAM LOCAL A dan RIDAM CLOUD A diselesaikan
// MAC ADDRESS KECEPATAN ANGIN F4:65:0B:54:C9:90

// Library
#include <ModbusMaster.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <PubSubClient.h>
#include <HTTPClient.h>

// deklarasi modbus node
#define RXD2 16
#define TXD2 17
#define WS_BAUD 4800

// jumlah data yang dijadikan MA
#define WINDOW_SIZE 5    // Number of readings to average

// variabel array untuk menyimpan 5 data
float windWindow[WINDOW_SIZE];
int bufferIndex = 0;
int count = 0;
float avgWind;

// deklarasi variabel kalibrasi
float yWind;
float awal_ukurWind, awal_sensorWind,
      akhir_ukurWind, akhir_sensorWind;
float aWind;
float cWind;

// Deklarasi variabel global rawdata yang dibagi 10
float windSpeed = 0;

// Buffer untuk menyimpan string hasil konversi kecepatan angin
char windspeed_Str[10];  

// Ganti dengan informasi HiveMQ Anda
const char* mqtt_server = "9a59e12602b646a292e7e66a5296e0ed.s1.eu.hivemq.cloud";  // Broker URL
const int mqtt_port = 8883;  // Gunakan 8883 untuk TLS
const char* mqtt_user = "testing";  // Username HiveMQ
const char* mqtt_password = "Testing123";  // Password HiveMQ

// Structure untuk kirim ke ALARM
struct struct_data_sensor{
  int kodeModul;
  int kodeVariabelWindspeed;
  float dataWindspeed;
};
struct_data_sensor windspeedData;

// Variabel untuk millis()
unsigned long previousMillis = 0;
const long interval = 1000; // Baca data setiap 10000 ms (10 detik)

WiFiClientSecure espClient;
PubSubClient client(espClient);
ModbusMaster node; //object node for class ModbusMaster
HardwareSerial wsSerial(2);

void setup() {
  // Inisialisasi Serial
  Serial.begin(115200); 

  // Serial begin untuk modbus
  wsSerial.begin(WS_BAUD, SERIAL_8N1, RXD2, TXD2);

  // Slave address: the factory default is 01H (set according to the need, 00H to FCH)
  node.begin(1, wsSerial);
 
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
    readAngin();    

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

