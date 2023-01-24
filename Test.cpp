#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <ESP8266WebServer.h>
#include <DNSServer.h>

#define LED 2

const char *ssid = "sdev265";
const char *password = "sdev265$";

const char *mqttServer = "10.81.104.102";
const int mqttPort = 1883;
const char *mqttUser = "";
const char *mqttPassword = "";

  WiFiClient espClient;
  PubSubClient mqttClient(espClient);

void setup()
{
  // put your setup code here, to run once:
  // #include "HelloWorld.cpp"
  // void HellowWorld();

  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);
  Serial.begin(9600);
  pinMode(LED, OUTPUT);

  Serial.print("Scan start ... ");
  int n = WiFi.scanNetworks();

  for (int i = 0; i < n; i++)
  {
    Serial.println(WiFi.SSID(i));
    Serial.print(n);
    Serial.println(" network(s) found");
    Serial.printf("%d: %s, Ch:%d (%ddBm) %s\n", i + 1, WiFi.SSID(i).c_str(), WiFi.channel(i), WiFi.RSSI(i), WiFi.encryptionType(i) == ENC_TYPE_NONE ? "open" : "");
  }
  Serial.println();

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.println("Connecting to WiFi..");
    delay(500);
  }
  Serial.println("Connected to the WiFi network ");
  Serial.print("IP address:\t");
  Serial.println(WiFi.localIP()); 
  //*************************************************
 
   mqttClient.setServer(mqttServer, mqttPort);
  while (!mqttClient.connected())
  {
    Serial.print("Connecting to MQTT... ");
    if (mqttClient.connect("ESP32Client"))  //, mqttUser, mqttPassword))
    {
      Serial.println("connected");
      delay(500);
    }
    else
    {
      Serial.print("failed with state ");
      Serial.println(mqttClient.state());
      delay(1000);
    }
  }
 
}

void loop() 
{
  // put your main code here, to run repeatedly:

 mqttClient.publish("esp32/test/", "vvvvvvvvvvv");

  digitalWrite(LED, HIGH);
  Serial.println("LED is on");
  delay(1000);
  digitalWrite(LED, LOW);
  Serial.println("LED is off");
  delay(1000);
}
