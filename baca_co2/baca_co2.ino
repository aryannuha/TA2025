// 13 April 2025
// Program akuisisi data DFRobot CO2
// CO2 dibaca melalui sensor DFRobot CO2 di pin 32 yang disimpan pada variabel 
// ppm, dikirim ke MODUL ALARM dalam bentuk string melalui variabel
// pesan pada fungsi sendToProc
// MAC ADDRESS : F4:65:0B:59:07:50
// proses dan pengirim ke RIDAM LOCALE A dan RIDAM CLOUD A belum terealisasi
// menunggu modul RIDAM LOCAL A dan RIDAM CLOUD A diselesaikan

// Library
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <PubSubClient.h>
#include <HTTPClient.h>
#include <AsyncUDP.h>
#include <ArduinoJson.h>

#define         MG_PIN                       (32)     // ESP32 ADC pin (GPIO34)
// #define         BOOL_PIN                     (4)      // ESP32 digital pin (GPIO4)
#define         DC_GAIN                      (8.5)    // define the DC gain of amplifier
#define         ADC_RESOLUTION               (4096)   // ESP32 12-bit ADC resolution
#define         ADC_VOLTAGE                  (3.3)    // ESP32 operates at 3.3V instead of 5V

#define         READ_SAMPLE_INTERVAL         (50)    // define the time interval(in milisecond) between each samples
#define         READ_SAMPLE_TIMES            (5)     // define how many samples you are going to take in normal operation

#define         ZERO_POINT_VOLTAGE           (0.344) // define the output of the sensor in volts when the concentration of CO2 is 400PPM, was read 2,67V
#define         REACTION_VOLTAGE             (0.030) // define the voltage drop of the sensor when move the sensor from air into 1000ppm CO2

float           CO2Curve[3]  =  {2.602,ZERO_POINT_VOLTAGE,(REACTION_VOLTAGE/(2.602-3))};
                                                     // two points are taken from the curve.
                                                     // with these two points, a line is formed which is
                                                     // "approximately equivalent" to the original curve.
                                                     // data format:{ x, y, slope}; point1: (lg400, 0.324), point2: (lg4000, 0.280)
                                                     // slope = ( reaction voltage ) / (log400 –log1000)

// // deklarasi RX2 TX2
// #define RXD2 16
// #define TXD2 17
// #define CO2_BAUD 9600

// Ganti dengan SSID dan Password WiFi Anda
const char* ssid = "Workshop 1";
const char* password = "eForacimenyan";

// IP statis ESP32 ini
IPAddress local_IP(192, 168, 0, 232);
IPAddress subnet(255, 255, 255, 0);

// IP tujuan (ESP32 lain)
IPAddress destinationIP(192, 168, 0, 120);

// jumlah data yang dijadikan MA
#define WINDOW_SIZE 5    // Number of readings to average

// variabel array untuk menyimpan 5 data
float co2Window[WINDOW_SIZE];
int bufferIndex = 0;
int count = 0;
float avgCo2;

// deklarasi variabel kalibrasi
float yCo2;
float awal_ukurCo2, awal_sensorCo2,
    akhir_ukurCo2, akhir_sensorCo2;
float aCo2;
float cCo2;

// Structure untuk kirim ke ALARM
struct struct_data_sensor{
  int kodeModul;
  int kodeVariabelCO2;
  int dataCO2;
};
struct_data_sensor co2Data;

// deklarasi global variabel
float ppm;
float volts;

// deklarasi variabel interval pembacaan
unsigned long previousMillis = 0;
const int interval = 1000;

// deklarasi objek
WiFiClientSecure espClient;
PubSubClient client(espClient);
AsyncUDP udp;

void setup()
{
    Serial.begin(115200);                              // UART setup, baudrate = 9600bps
    // pinMode(BOOL_PIN, INPUT_PULLUP);                 // ESP32 requires explicit pullup configuration
    
    // Inisialisasi wifi
    setup_wifi();

    // ESP32-specific ADC configuration
    analogSetWidth(12);                              // Set resolution to 12 bits
    analogSetAttenuation(ADC_11db);                  // Set attenuation for the full 3.3V range

    Serial.println("MG-811 Demonstration for ESP32");
    Serial.println("-------------------------------");
}

void loop()
{
    unsigned long currentMillis = millis();

    if(currentMillis - previousMillis >= interval){
        previousMillis = currentMillis;

        // fungsi baca co2 masih dalam bentuk tegangan
        readCO2();

        // fungsi proses, kalibrasi, MA
        // proses();

        // fungsi kirim ke alarm
        sendData();

        // fungsi menampilkan di serial monitor
        // displayMonitor();
    }
}