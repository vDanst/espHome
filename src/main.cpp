#include <Arduino.h>
#include <OneWire.h>
#include <DallasTemperature.h>

#define ONE_WIRE_BUS D2
#define ADC_A0 A0

float volt = 0.0;
float vin = 0.0;  
float r1 = 98820;
float r2 = 9937;

int timeWake = 0;

int value = 0;

// Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
OneWire oneWire(ONE_WIRE_BUS);

// Pass our oneWire reference to Dallas Temperature. 
DallasTemperature sensors(&oneWire);

// arrays to hold device addresses


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(ADC_A0,INPUT);
  sensors.begin();
  
  
}

void countsleep(){
  timeWake++;
  Serial.println(timeWake);
  ESP.deepSleep(30e6);
  
}

void loop() {

  sensors.requestTemperatures(); // Send the command to get temperatures
  float atempC = sensors.getTempCByIndex(0);
  float btempC = sensors.getTempCByIndex(1);
  Serial.print("DS18B20: ");
  Serial.print(atempC);
  Serial.println("C");

  value = analogRead(ADC_A0);
  volt = (value * 3.0)/1023.0;
  vin = volt / (r2 / (r1+r2));

  Serial.print("V - ");
  Serial.println(vin);
  delay(1000);
  
  //countsleep();
  
}