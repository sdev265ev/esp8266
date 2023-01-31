#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <DNSServer.h>

#include <WiFiManager.h>
#include <PubSubClient.h>

const char *ssid = "zoomfrog";
const char *password = "8129839566";

const char *mqttServer = "192.168.2.158";
const int mqttPort = 1883;
const char *mqttUser = "user";
const char *mqttPassword = "user";

int counter = 0;
String ss = "";

String MACaddress = "";

#define LED 2

//=================================================================
WiFiClient espClient;
PubSubClient mqttClient(espClient);
void setup()
{
  // put your setup code here, to run once:

  pinMode(LED, OUTPUT);
  Serial.begin(57600);
  // 300, 600, 1200, 1800, 2400, 4800, 7200, 9600, 14,400, 19,200, 38,400, 57,600, and 115,200 baud
  //=========== WIFI ==================================================
  WiFi.disconnect();
  WiFi.mode(WIFI_STA);

  delay(100);

  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);

  Serial.print("Scan start ... ");
  int n = WiFi.scanNetworks();

  for (int i = 0; i < n; i++)
  {
    Serial.print(n);
    Serial.println(" network(s) found");
    Serial.println(WiFi.SSID(i));

    Serial.printf("%d: %s, Ch:%d (%ddBm) %s\n", i + 1, WiFi.SSID(i).c_str(), WiFi.channel(i), WiFi.RSSI(i), WiFi.encryptionType(i) == ENC_TYPE_NONE ? "open" : "");
  }
  Serial.println();

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.println("Connecting to WiFi..");
    delay(200);
  }
  Serial.println("Connected to the WiFi network ");
  Serial.print("IP address:\t");
  Serial.println(WiFi.localIP());
  Serial.println(WiFi.macAddress());
  MACaddress = WiFi.macAddress();

  //***** MQTT SETUP ********************************************
  mqttClient.setServer(mqttServer, mqttPort);
  while (!mqttClient.connected())
  {
    Serial.println("Connecting to MQTT...");
    if (mqttClient.connect("ESP32Client", mqttUser, mqttPassword))
    {
      Serial.println("connected");
      delay(500);
    }
    else
    {
      Serial.print("failed with state ");
      Serial.print(mqttClient.state());
      delay(2000);
    }
  }
}
//===============================================================

// variables to keep track of the timing of recent interrupts
unsigned long button_time = 0;
unsigned long last_button_time = 0;

//.............................................
// UINT8 is an 8-bit unsigned integer 0 - 255
//  _t is for type. It is used to insure
//   the code runs on different platforms
struct Button
{
  const uint8_t PIN;
  uint32_t numberKeyPresses;
  bool pressed;
};

Button button1 = {5, 0, false};
Button button2 = {6, 0, false};

// **************************************************************
// The IRAM_ATTR attribute places the compiled code Internal RAM (IRAM)
//   instead of slower Flash
// ISR = Interrupt Service Routine

void IRAM_ATTR ButtonCallBack1()
{
  button_time = millis();
  if (button_time - last_button_time > 50)
  {
    button1.numberKeyPresses++;
    last_button_time = button_time;
    button1.pressed = true;
  }

  // Modes: LOW, HIGH, CHANGE, FALLING, RISING
  pinMode(button1.PIN, INPUT_PULLUP);
  attachInterrupt(button1.PIN, ButtonCallBack1, FALLING);
}

//*************************** LOOP ***********************************
void loop()
{
  // put your main code here, to run repeatedly:
  ss = String(counter);
  Serial.println(ss);
  mqttClient.publish("58:BF:25:D8:88:0F/test/", ss.c_str());
  counter++;

  digitalWrite(LED, HIGH);

  Serial.println("LED is on");

  digitalWrite(LED, LOW);
  Serial.println("LED is off");
  delay(1000);
}
