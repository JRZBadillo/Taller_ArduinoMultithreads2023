#include <OneWire.h>
#include <DallasTemperature.h>
#include "MultiFunctionShield.h"

#define BUZZER_PIN 3
#define LED_1_PIN 13
#define LED_2_PIN 12
#define LED_3_PIN 11
#define LED_4_PIN 10
#define BUTTON_1_PIN A1
#define BUTTON_2_PIN A2
#define BUTTON_3_PIN A3
#define ONE_WIRE_BUS A4

MultiFunctionShield MFS;
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensorDS18B20(&oneWire);

uint8_t i;
uint16_t x = 1000;
uint8_t contador = 0;
float tempC = 0.0;
int itemp = 0;
void setup (){
  MFS.begin();
  pinMode(BUTTON_1_PIN, INPUT);
  pinMode(BUTTON_2_PIN, INPUT);
  pinMode(BUTTON_3_PIN, INPUT);
  pinMode(LED_1_PIN, OUTPUT);
  pinMode(LED_2_PIN, OUTPUT);
  pinMode(LED_3_PIN, OUTPUT);
  pinMode(LED_4_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(LED_1_PIN, HIGH);    // off
  digitalWrite(LED_2_PIN, HIGH);    // off
  digitalWrite(LED_3_PIN, HIGH);    // off
  digitalWrite(LED_4_PIN, HIGH);    // off
  digitalWrite(BUZZER_PIN, HIGH);   // off
}
 
void loop(){
  sensorDS18B20.requestTemperatures();
  tempC = sensorDS18B20.getTempCByIndex(0);
  itemp = int(tempC*100);
  MFS.Display (itemp);
  delay (1000);       
}
