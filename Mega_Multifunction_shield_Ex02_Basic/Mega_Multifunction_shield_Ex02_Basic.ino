
#include "MultiFunctionShield.h"

#define BUZZER_PIN 3
#define LED_1_PIN 13
#define LED_2_PIN 12
#define LED_3_PIN 11
#define LED_4_PIN 10
#define BUTTON_1_PIN A1
#define BUTTON_2_PIN A2
#define BUTTON_3_PIN A3

MultiFunctionShield MFS;

uint8_t i;
uint16_t x = 1000;
uint8_t contador = 0;

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
    if (x > 0){
      if (x % 100 > 70){
        x -= 40;
      }
      MFS.Display (x);
      if (digitalRead(BUTTON_1_PIN) == 0 || 
          digitalRead(BUTTON_2_PIN) == 0 || 
          digitalRead(BUTTON_3_PIN) == 0){
        if (digitalRead(BUTTON_1_PIN) == 0 ){
          contador = 20;
          digitalWrite(LED_1_PIN, LOW);
          digitalWrite(LED_2_PIN, HIGH);
          digitalWrite(LED_3_PIN, HIGH);
        }else if (digitalRead(BUTTON_2_PIN) == 0 ){
          contador = 40;
          digitalWrite(LED_1_PIN, HIGH);
          digitalWrite(LED_2_PIN, LOW);
          digitalWrite(LED_3_PIN, HIGH);
        }else if(digitalRead(BUTTON_3_PIN) == 0 ){
          contador = 60;
          digitalWrite(LED_1_PIN, HIGH);
          digitalWrite(LED_2_PIN, HIGH);
          digitalWrite(LED_3_PIN, LOW);
        }else{
          contador = 0;
          digitalWrite(LED_1_PIN, HIGH);
          digitalWrite(LED_2_PIN, HIGH);
          digitalWrite(LED_3_PIN, HIGH); 
        }
     }
     if (contador > 0){
        for (i = 0; i < contador; i++){
          digitalWrite(BUZZER_PIN, LOW);
          delay (40);
          digitalWrite(BUZZER_PIN, HIGH);
          delay (10);
        }
        contador = 0;
        digitalWrite(LED_1_PIN, HIGH);
        digitalWrite(LED_2_PIN, HIGH);
        digitalWrite(LED_3_PIN, HIGH); 
     }
     x --;
   }else{
     for (i = 0; i < 50; i++){
       digitalWrite(BUZZER_PIN, LOW);
       delay (30);
       digitalWrite(BUZZER_PIN, HIGH);
       delay (10);
     }
   }
   delay (1000);       
}
