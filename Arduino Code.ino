#include <WiFiNINA.h>
#include <ArduinoHttpClient.h>
#include "arduino_secrets.h"

// Network credentials
const char ssid[] = SECRET_SSID;
const char pass[] = SECRET_PASS;

// Firebase project details
const char* host = "traffic-control-3c3b4-default-rtdb.asia-southeast1.firebasedatabase.app";
const int httpsPort = 443;

WiFiSSLClient wifiClient;
HttpClient client = HttpClient(wifiClient, host, httpsPort);

const int redLEDPin = 2;
const int greenLEDPin = 3;
const int blueLEDPin = 4;

void setup() {
  Serial.begin(9600);
  while (!Serial);

  pinMode(redLEDPin, OUTPUT);
  pinMode(greenLEDPin, OUTPUT);
  pinMode(blueLEDPin, OUTPUT);

  if (WiFi.status() == WL_NO_MODULE) {
    Serial.println("Communication with WiFi module failed!");
    while (true);
  }

  Serial.print("Connecting to ");
  Serial.println(ssid);
  while (WiFi.begin(ssid, pass) != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("Connected!");
}

void getLEDStatus(String color) {
  String path = "/" + color + ".json";
  Serial.println("GET Request Path: " + path);

  client.beginRequest();
  client.get(path);
  int statusCode = client.responseStatusCode();
  String response = client.responseBody();
  client.endRequest();

  Serial.print("GET request sent to path: ");
  Serial.println(path);
  Serial.print(color);
  Serial.print(" LED read status code: ");
  Serial.println(statusCode);
  Serial.print(color);
  Serial.print(" LED response: ");
  Serial.println(response);

  if (statusCode == 200) {
    bool isOn = response.indexOf("true") != -1;
    Serial.println(color + " LED is " + (isOn ? "ON" : "OFF"));
    if (color == "red") {
      digitalWrite(redLEDPin, isOn ? HIGH : LOW);
    } else if (color == "green") {
      digitalWrite(greenLEDPin, isOn ? HIGH : LOW);
    } else if (color == "blue") {
      digitalWrite(blueLEDPin, isOn ? HIGH : LOW);
    }
  } else {
    Serial.println("Failed to read the data.");
  }
}

void loop() {
  getLEDStatus("blue");
  getLEDStatus("green");
  getLEDStatus("red");
  delay(2000);
}
