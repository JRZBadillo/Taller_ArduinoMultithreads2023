
#include "MultiFunctionShield.h"
#include "pt.h"

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
int8_t estado = 0;
uint8_t sentido = 0;

static struct pt pt1;
static struct pt pt2;
static struct pt pt3;

static int protothread1(struct pt *pt1 ) {
  static unsigned long timestamp = 0;
  PT_BEGIN(pt1);
  if (x > 0){
    if (x % 100 > 70){
      x -= 40;
    }
    MFS.Display (x);
    x --;
  }else{
     for (i = 0; i < 50; i++){
       digitalWrite(BUZZER_PIN, LOW);
       PT_WAIT_UNTIL(pt1, millis() - timestamp > 30);
       timestamp = millis();
       digitalWrite(BUZZER_PIN, HIGH);
       PT_WAIT_UNTIL(pt1, millis() - timestamp > 10);
       timestamp = millis();
     }
   }    
  PT_WAIT_UNTIL(pt1, millis() - timestamp > 1000);
  timestamp = millis();
  PT_END(pt1);
}

static int protothread2(struct pt *pt2 ) {
  static unsigned long timestamp = 0;
  PT_BEGIN(pt2);
  if (digitalRead(BUTTON_1_PIN) == 0 || 
      digitalRead(BUTTON_2_PIN) == 0 || 
      digitalRead(BUTTON_3_PIN) == 0){   
    if (digitalRead(BUTTON_1_PIN) == 0 ){
      contador = 20;
      //digitalWrite(LED_1_PIN, LOW);
      //digitalWrite(LED_2_PIN, HIGH);
      //digitalWrite(LED_3_PIN, HIGH);
    }else if (digitalRead(BUTTON_2_PIN) == 0 ){
      contador = 40;
      //digitalWrite(LED_1_PIN, HIGH);
      //digitalWrite(LED_2_PIN, LOW);
      //digitalWrite(LED_3_PIN, HIGH);
    }else if(digitalRead(BUTTON_3_PIN) == 0 ){
      contador = 60;
      //digitalWrite(LED_1_PIN, HIGH);
      //digitalWrite(LED_2_PIN, HIGH);
      //digitalWrite(LED_3_PIN, LOW);
    }else{
      contador = 0;
      //digitalWrite(LED_1_PIN, HIGH);
      //digitalWrite(LED_2_PIN, HIGH);
      //digitalWrite(LED_3_PIN, HIGH); 
    }
  }
  if (contador > 0){
    for (i = 0; i < contador; i++){
      digitalWrite(BUZZER_PIN, LOW);
      PT_WAIT_UNTIL(pt2, millis() - timestamp > 40);
      timestamp = millis();
      digitalWrite(BUZZER_PIN, HIGH);
      PT_WAIT_UNTIL(pt2, millis() - timestamp > 10);
      timestamp = millis();
    }
    contador = 0;
    //digitalWrite(LED_1_PIN, HIGH);
    //digitalWrite(LED_2_PIN, HIGH);
    //digitalWrite(LED_3_PIN, HIGH); 
  }  
  PT_WAIT_UNTIL(pt2, millis() - timestamp > 100 );
  timestamp = millis();
  PT_END(pt2);
}

static int protothread3(struct pt *pt3 ) {
  static unsigned long timestamp = 0;
  PT_BEGIN(pt3);
  switch (estado){
    case 0:
      digitalWrite(LED_1_PIN, LOW);
      digitalWrite(LED_2_PIN, HIGH);
      digitalWrite(LED_3_PIN, HIGH);
      digitalWrite(LED_4_PIN, HIGH);
      break;
    case 1:
      digitalWrite(LED_1_PIN, HIGH);
      digitalWrite(LED_2_PIN, LOW);
      digitalWrite(LED_3_PIN, HIGH);
      digitalWrite(LED_4_PIN, HIGH);
      break;
    case 2:
      digitalWrite(LED_1_PIN, HIGH);
      digitalWrite(LED_2_PIN, HIGH);
      digitalWrite(LED_3_PIN, LOW);
      digitalWrite(LED_4_PIN, HIGH);
      break;
    case 3:
      digitalWrite(LED_1_PIN, HIGH);
      digitalWrite(LED_2_PIN, HIGH);
      digitalWrite(LED_3_PIN, HIGH);
      digitalWrite(LED_4_PIN, LOW);
      break;
    default:
      break;
  }
  if((estado < 0) || (estado > 3)){
    sentido++;
  }
  if (sentido % 2 == 0){
    estado ++;
  }else {
    estado --;
  }
  PT_WAIT_UNTIL(pt3, millis() - timestamp > 130);
  timestamp = millis();
  PT_END(pt3);
}

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
  PT_INIT(&pt1);
  PT_INIT(&pt2);
  PT_INIT(&pt3);
}
 
void loop(){
  protothread1(&pt1 );
  protothread2(&pt2 );
  protothread3(&pt3 );
}
