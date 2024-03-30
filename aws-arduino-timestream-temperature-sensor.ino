#include <Arduino.h>

#include <ArduinoBearSSL.h>
#include <ArduinoMqttClient.h>
#include <WiFiNINA.h>
#include <ArduinoECCX08.h>
#include <Arduino_JSON.h>
#include "DHT.h"

#include "arduino_secrets.h"

// Enter your sensitive data in arduino_secrets.h
const char ssid[] = SECRET_SSID;
const char pass[] = SECRET_PASS;

const char broker[] = SECRET_BROKER;

const char deviceId[] = DEVICE_ID;

const char* deviceCertificate  = SECRET_CERTIFICATE;
const char* privateCertificate  = AWS_CERT_PRIVATE;

WiFiClient wifiClient;
BearSSLClient sslClient(wifiClient);
MqttClient mqttClient(sslClient);
DHT dht(6, DHT22);

const char* topic = "arduino-temperature-sensor";

unsigned long lastTime = 0;
unsigned long publishFrequency = 5; // publish every x seconds



void setup() {
  Serial.println("setup");
  if (Serial.available()) {
    Serial.begin(9600);
    while (!Serial);
  }

  if (!ECCX08.begin()) {
    Serial.println("No ECCX08 present!");
    while (1);
  }

  dht.begin();

  ArduinoBearSSL.onGetTime(getTime);

  sslClient.setKey(privateCertificate, deviceCertificate);

  mqttClient.setId(deviceId);

}

void loop() {
  JSONVar jsonObject;

  if (WiFi.status() != WL_CONNECTED) {
    connectWiFi();
  }
  
  if (!mqttClient.connected()) {
    // connect over MQTT if not already.
    connectMQTT();
  }


  unsigned long currentTime = getTime();
  if (currentTime >= (lastTime + publishFrequency)) {
    lastTime = currentTime;

    // read sensor data
    float temperature = roundf(dht.readTemperature());
    float humidity = roundf(dht.readHumidity());

    Serial.println(String("Temperature: ") + temperature + String(" Humidity: ") + humidity);


    mqttClient.beginMessage(topic);

    jsonObject["timestamp"] = currentTime;
    jsonObject["temperature"] = temperature;
    jsonObject["humidity"] = humidity;
    String jsonString = JSON.stringify(jsonObject);

    mqttClient.print(jsonString);
    mqttClient.endMessage();
  }
}

unsigned long getTime() {
  // get the current time from the WiFi module  
  return WiFi.getTime();
}

void connectWiFi() {
  Serial.print(String("Attempting to connect to SSID: ") + ssid);
  
  while (WiFi.begin(ssid, pass) != WL_CONNECTED) {
    // failed try again
    Serial.print(".");
    delay(3000);
  }
  Serial.println("");

  Serial.println(String("Now connected to: ") + ssid);
}

void connectMQTT() {
  Serial.println(String("Attempting to connect to MQTT endpoint: ") + broker);
  
  while (!mqttClient.connect(broker, 8883)) {
    // failed try again
    delay(3000);
    Serial.println(String("MQTT Connection Error: ") + mqttClient.connectError());
  }
  Serial.println(String("Now connected to: ") + broker);
}