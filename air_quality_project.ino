#include <WiFi.h>
#include <PubSubClient.h>
#include <DHT.h>
#include <MQUnifiedsensor.h>
#include <HardwareSerial.h>

// WiFi credentials – replace with your network info.
const char* ssid = "Spidey_24_nomap";
const char* password = "toshiba2025";

// MQTT Broker details (using public Mosquitto server)
const char* mqtt_server = "192.168.0.114";
const uint16_t mqtt_port = 1883;
const char* mqtt_topic = "kammaaz/airquality/data";

WiFiClient espClient;
PubSubClient client(espClient);

// Serial Port connections for PMS5003 Sensor
#define RXD2 16 // To sensor TXD
#define TXD2 17 // To sensor RXD

struct pms5003data {
  uint16_t framelen;
  uint16_t pm10_standard, pm25_standard, pm100_standard;
  uint16_t pm10_env, pm25_env, pm100_env;
  uint16_t particles_03um, particles_05um, particles_10um, particles_25um, particles_50um, particles_100um;
  uint16_t unused;
  uint16_t checksum;
};

struct pms5003data data;

// ----- Sensor Setup -----
// DHT22: Temperature & Humidity Sensor
#define DHTPIN 4       // Data pin connected to GPIO4
#define DHTTYPE DHT22     
DHT dht(DHTPIN, DHTTYPE);

// MQ-135: VOC Sensor
#define BOARD "ESP-32"
#define Voltage_Resolution 3.3
#define ADC_Pin 34      // Use ADC1 channel (GPIO34) for stable analog readings
#define Sensor_Type "MQ-135"
#define ADC_Bit_Resolution 12
#define RatioMQ135CleanAir 3.6  // Calibration factor in clean air
double VOC_ppm = 0;  
MQUnifiedsensor MQ135(BOARD, Voltage_Resolution, ADC_Bit_Resolution, ADC_Pin, Sensor_Type);

// Timing for sensor reads
unsigned long previousMillis = 0;
const long interval = 3000; // Read sensors every 3 seconds

// Function to connect to WiFi
void setupWiFi() {
  delay(10);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

// Function to reconnect to MQTT broker
void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    String clientId = "ESP32Client-";
    clientId += String(random(0xffff), HEX);
    if (client.connect(clientId.c_str())) {
      Serial.println(" connected");
    } else {
      Serial.print(" failed, rc=");
      Serial.print(client.state());
      Serial.println(" trying again in 5 seconds");
      delay(5000);
    }
  }
}

// Updated PMS5003 data reading function
boolean readPMSdata(Stream *s) {
  if (!s->available()) {
    return false;
  }
 
  // Look for the start byte (0x42)
  if (s->peek() != 0x42) {
    s->read();
    return false;
  }
 
  // Ensure at least 32 bytes are available
  if (s->available() < 32) {
    return false;
  }
 
  uint8_t buffer[32];
  uint16_t sum = 0;
  s->readBytes(buffer, 32);
 
  // Calculate checksum from the first 30 bytes
  for (uint8_t i = 0; i < 30; i++) {
    sum += buffer[i];
  }
 
  // Convert the data to 16-bit values (correcting for endianness)
  uint16_t buffer_u16[15];
  for (uint8_t i = 0; i < 15; i++) {
    buffer_u16[i] = buffer[2 + i * 2 + 1];
    buffer_u16[i] += (buffer[2 + i * 2] << 8);
  }
 
  // Copy data into our struct
  memcpy((void *)&data, (void *)buffer_u16, 30);
 
  if (sum != data.checksum) {
    Serial.println("Checksum failure");
    return false;
  }
  // Successful read
  return true;
}

void setup() {
  Serial.begin(115200);
  dht.begin();
  
  // Initialize WiFi and MQTT connection.
  setupWiFi();
  client.setServer(mqtt_server, mqtt_port);
  
  // Set up UART for the PMS5003 sensor
  Serial1.begin(9600, SERIAL_8N1, RXD2, TXD2);
  Serial.println("PMS5003 sensor reading started...");

  // Configure and calibrate MQ-135 sensor for VOC readings.
  MQ135.setRegressionMethod(1);   // Exponential regression model
  MQ135.setA(110.47);              
  MQ135.setB(-2.862);              
  MQ135.init();
  
  Serial.print("Calibrating MQ-135, please wait");
  float calcR0 = 0;
  for (int i = 1; i <= 10; i++) {
    MQ135.update();
    calcR0 += MQ135.calibrate(RatioMQ135CleanAir);
    Serial.print(".");
    delay(1000);
  }
  MQ135.setR0(calcR0 / 10);
  Serial.println(" Calibration done!");
  
  if (isinf(calcR0)) {
    Serial.println("Warning: R0 is infinite. Check wiring/power.");
    while (1);
  }
  if (calcR0 == 0) {
    Serial.println("Warning: R0 is zero. Check wiring/power.");
    while (1);
  }
  MQ135.serialDebug(false); // Disable MQ-135 debug printing
}

void loop() {
  unsigned long currentMillis = millis();
  
  // Ensure MQTT is connected
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
  
  // Check sensors at the defined interval (every 3 seconds)
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    
    // Read DHT22 sensor (temperature & humidity)
    float humidity = dht.readHumidity();
    float temperature = dht.readTemperature();
    bool validDHT = true;
    if (isnan(humidity) || isnan(temperature)) {
      Serial.println("DHT22 read error!");
      validDHT = false;
    } else {
      Serial.print("Temperature: ");
      Serial.print(temperature);
      Serial.print("°C, Humidity: ");
      Serial.print(humidity);
      Serial.println("%");
    }
    
    // Read PMS5003 sensor (PM2.5)
    bool pmsReadSuccess = false;
    while (Serial1.available() >= 32) {
      if (readPMSdata(&Serial1)) {
        pmsReadSuccess = true;
      }
    }
    
    int pm25Value = 0;
    if (pmsReadSuccess) {
      pm25Value = data.pm25_env;
      Serial.print("PM 2.5: ");
      Serial.print(pm25Value);
      Serial.println(" µg/m³");
    } else {
      Serial.println("NO PMS Reading");
    }
    
    // Read MQ-135 sensor (VOC)
    MQ135.update();
    VOC_ppm = MQ135.readSensor();
    bool validVOC = true;
    if (isnan(VOC_ppm) || isinf(VOC_ppm) || VOC_ppm <= 0) {
      Serial.println("MQ135 read error!");
      validVOC = false;
    } else {
      Serial.print("Measured VOC (PPM): ");
      Serial.println(VOC_ppm);
    }
    
    // Only send data if all sensor readings are available/valid
    if (validDHT && pmsReadSuccess && validVOC) {
      char jsonPayload[150];
      snprintf(jsonPayload, sizeof(jsonPayload),
               "{\"temperature\":%.2f,\"humidity\":%.2f,\"pm25\":%d,\"voc\":%.2f}",
               temperature, humidity, pm25Value, VOC_ppm);
      
      Serial.print("Publishing JSON: ");
      Serial.println(jsonPayload);
      client.publish(mqtt_topic, jsonPayload);
    } else {
      Serial.println("Not all sensor readings available. Skipping MQTT publish.");
    }
    
    Serial.println("--------------------------------");
  }
}
