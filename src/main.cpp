#include <Arduino.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>
#include <ArduinoJson.h>

#define ONE_WIRE_BUS D2
#define ADC_A0 A0

#define BOT_TOKEN "50281"
#define CHAT_ID "30"

const char* ssid = "ASUS_GUEST";
const char* password = "";

X509List cert(TELEGRAM_CERTIFICATE_ROOT);
WiFiClientSecure secured_client;
UniversalTelegramBot bot(BOT_TOKEN, secured_client);

float r1 = 98820;
float r2 = 9937;

int value = 0;

uint8 wifiIncriment = 0;


OneWire oneWire(ONE_WIRE_BUS); 
DallasTemperature sensors(&oneWire);


 String textMessage(){
  sensors.requestTemperatures(); // Send the command to get temperatures
  String ytempC = String(sensors.getTempCByIndex(0));
  String btempC = String(sensors.getTempCByIndex(1));

  value = analogRead(ADC_A0);
  float volt = (value * 3.2)/1023.0;
  float vin = volt / (r2 / (r1+r2));
  String batt = String(vin);
  delay(500);

  return "Yard " + ytempC + "C \n" + "Balcony " + btempC + "C \n" + "VCC " + batt + "v";
 }

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(ADC_A0,INPUT);
  sensors.begin();

  WiFi.begin(ssid,password); 
  secured_client.setTrustAnchors(&cert); // Add root certificate for api.telegram.org
  while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
      wifiIncriment++;
      if (wifiIncriment == 10){
      ESP.deepSleep(400e7);
      }
    }
  
  configTime(0, 0, "pool.ntp.org");

  bot.sendMessage(CHAT_ID, textMessage(), "");
  ESP.deepSleep(400e7);
}


void loop() {
 
 }
