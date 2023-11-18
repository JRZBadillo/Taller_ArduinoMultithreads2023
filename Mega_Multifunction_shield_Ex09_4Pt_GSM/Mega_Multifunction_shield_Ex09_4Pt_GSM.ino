
#include "pt.h"
#include "OLED_I2C.h"

#include <Wire.h>
#include <DS3232RTC.h>
#include <Time.h>

#include <SPI.h>
#include <SD.h>

#include <OneWire.h>
#include <DallasTemperature.h>

#define button_01 A3
#define button_02 A2
#define button_03 A1
#define button_04 A0
#define LED_01 A4
#define LED_02 A5
#define LED_03 A6
#define LED_04 A7
#define LED_05 A8
#define LED_06 A9
#define LED_07 A10
#define ONE_WIRE_BUS 4

OLED myOLED(SDA, SCL, 8);

time_t t;
tmElements_t tm;

File logFile;
char str_namefile [80];

extern uint8_t SmallFont[];
int8_t estado = 1;
uint8_t sentido = 0;
char str_core[100];
float tempC = 0.0;

static struct pt pt1;
static struct pt pt2;
static struct pt pt3;
static struct pt pt4;
uint8_t sendmsg_flg = 0;


OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensorDS18B20(&oneWire);

void getfilename(){
  setSyncProvider(RTC.get);
  sprintf(str_namefile, "%02d_%02d_%02d.TXT", 
  year()-2000,month(),day() );
}

static int protothread1(struct pt *pt1 ) {
  static unsigned long timestamp = 0;
  PT_BEGIN(pt1);
//**********************************************************
  setSyncProvider(RTC.get);
  sensorDS18B20.requestTemperatures();
  tempC = sensorDS18B20.getTempCByIndex(0);
  sprintf(str_core, "Temp: %02d.%02d", 
  int(tempC), int(tempC*100)%100 );
  myOLED.print(str_core, LEFT, 35);
  
  sprintf(str_core, "%02d/%02d/%02d %02d:%02d:%02d hrs", 
  day(),month(),year()-2000,hour(),minute(),second() );
  myOLED.print(str_core, CENTER, 55);
  myOLED.update();
//**********************************************************
  PT_WAIT_UNTIL(pt1, millis() - timestamp > 1000 );
  timestamp = millis();
  
  PT_END(pt1);
}

static int protothread2(struct pt *pt2 ) {
  static unsigned long timestamp = 0;
  PT_BEGIN(pt2);
//**********************************************************
  switch (estado){
    case 0:
      digitalWrite(LED_01, HIGH);
      digitalWrite(LED_02, LOW);
      digitalWrite(LED_03, LOW);
      digitalWrite(LED_04, LOW);
      digitalWrite(LED_05, LOW);
      digitalWrite(LED_06, LOW);
      digitalWrite(LED_07, LOW);
      break;
    case 1:
      digitalWrite(LED_01, LOW);
      digitalWrite(LED_02, HIGH);
      digitalWrite(LED_03, LOW);
      digitalWrite(LED_04, LOW);
      digitalWrite(LED_05, LOW);
      digitalWrite(LED_06, LOW);
      digitalWrite(LED_07, LOW);
      break;
    case 2:
      digitalWrite(LED_01, LOW);
      digitalWrite(LED_02, LOW);
      digitalWrite(LED_03, HIGH);
      digitalWrite(LED_04, LOW);
      digitalWrite(LED_05, LOW);
      digitalWrite(LED_06, LOW);
      digitalWrite(LED_07, LOW);
      break;
    case 3:
      digitalWrite(LED_01, LOW);
      digitalWrite(LED_02, LOW);
      digitalWrite(LED_03, LOW);
      digitalWrite(LED_04, HIGH);
      digitalWrite(LED_05, LOW);
      digitalWrite(LED_06, LOW);
      digitalWrite(LED_07, LOW);
      break;
    case 4:
      digitalWrite(LED_01, LOW);
      digitalWrite(LED_02, LOW);
      digitalWrite(LED_03, LOW);
      digitalWrite(LED_04, LOW);
      digitalWrite(LED_05, HIGH);
      digitalWrite(LED_06, LOW);
      digitalWrite(LED_07, LOW);
      break;
    case 5:
      digitalWrite(LED_01, LOW);
      digitalWrite(LED_02, LOW);
      digitalWrite(LED_03, LOW);
      digitalWrite(LED_04, LOW);
      digitalWrite(LED_05, LOW);
      digitalWrite(LED_06, HIGH);
      digitalWrite(LED_07, LOW);
      break;
    case 6:
      digitalWrite(LED_01, LOW);
      digitalWrite(LED_02, LOW);
      digitalWrite(LED_03, LOW);
      digitalWrite(LED_04, LOW);
      digitalWrite(LED_05, LOW);
      digitalWrite(LED_06, LOW);
      digitalWrite(LED_07, HIGH);
      break;
    default:
      digitalWrite(LED_01, LOW);
      digitalWrite(LED_02, LOW);
      digitalWrite(LED_03, LOW);
      digitalWrite(LED_04, LOW);
      digitalWrite(LED_05, LOW);
      digitalWrite(LED_06, LOW);
      digitalWrite(LED_07, LOW);
      break;
  }
  if((estado < 1) || (estado > 5)){
    sentido++;
  }
  if (sentido % 2 == 0){
    estado ++;
  }else {
    estado --;
  }
//**********************************************************
  if (digitalRead(button_04) == 1){
    getfilename();
    logFile = SD.open(str_namefile);
    if (logFile) {
      Serial.println(str_namefile);
      while (logFile.available()) {
        Serial.write(logFile.read());
      }
      logFile.close();
    } else {
      Serial.println("Error al leer archivo de microSD.");
    } 
  }
//***********************************************************
  PT_WAIT_UNTIL(pt2, millis() - timestamp > 100 );
  timestamp = millis();
  PT_END(pt2);
}

static int protothread3(struct pt *pt3 ) {
  static unsigned long timestamp = 0;
  PT_BEGIN(pt3);
//**********************************************************
  sensorDS18B20.requestTemperatures();
  tempC = sensorDS18B20.getTempCByIndex(0);
  if(tempC != DEVICE_DISCONNECTED_C){
    //Serial.print("Temperature is: ");
    //Serial.println(tempC);
  } 
  else{
    Serial.println("Error: Could not read temperature data");
  }
  setSyncProvider(RTC.get);
  getfilename();
  logFile = SD.open(str_namefile, FILE_WRITE);
  if (logFile) {
    sprintf(str_core, "%02d/%02d/%02d %02d:%02d:%02d hrs | %3d.%02d C", 
    day(),month(),year()-2000,hour(),minute(),second(), int(tempC), int(tempC*100)%100 );
    //Serial.println(str_core);
    logFile.println(str_core);
    logFile.close();
  } else {
    Serial.println("Error al escribir en microSD.");
  }
//**********************************************************
  PT_WAIT_UNTIL(pt3, millis() - timestamp > 15000 );
  timestamp = millis();
  PT_END(pt3);
}

static int protothread4(struct pt *pt4 ) {
  static unsigned long timestamp = 0;
  PT_BEGIN(pt4);
//**********************************************************
 if ( (digitalRead (button_01) == 1) && (sendmsg_flg == 0)){
   sendmsg_flg = 1;
   
   myOLED.print("SMS", RIGHT, 35);
   myOLED.update();
   
   Serial.println("Enviando mensaje SMS...");

   Serial2.println("AT");
   updateSerial();
   Serial2.println("AT+CMGF=1");
   updateSerial();
   Serial2.println("AT+CMGS=\"+52xxxxxxxxxx\"");  // -> reemplazar xxxxxxxxxxcon el no. telefonico a 10 digitos 
   updateSerial();
   Serial2.print("Hola Mundo!!! (enviado desde modulo de telemetria V2)..."); //text content
   
   PT_WAIT_UNTIL(pt4, millis() - timestamp > 1000 );
   timestamp = millis();
   updateSerial();
   Serial2.write(26);  
   PT_WAIT_UNTIL(pt4, millis() - timestamp > 2000 );
   timestamp = millis(); 
   
   myOLED.print("   ", RIGHT, 35);
   myOLED.update();
   
   sendmsg_flg = 0;
 }
//**********************************************************
  PT_WAIT_UNTIL(pt4, millis() - timestamp > 500 );
  timestamp = millis();
  PT_END(pt4);
}


void setup() {
  Serial.begin(9600, SERIAL_8N1);
  Serial2.begin(9600, SERIAL_8N1);
  
  sensorDS18B20.begin();
    
  pinMode(button_01, INPUT );
  pinMode(button_02, INPUT );
  pinMode(button_03, INPUT );
  pinMode(button_04, INPUT );   
  pinMode(LED_01, OUTPUT );
  pinMode(LED_02, OUTPUT );
  pinMode(LED_03, OUTPUT );
  pinMode(LED_04, OUTPUT );
  pinMode(LED_05, OUTPUT );
  pinMode(LED_06, OUTPUT );
  pinMode(LED_07, OUTPUT );
  digitalWrite(LED_01, LOW );
  digitalWrite(LED_02, LOW );
  digitalWrite(LED_03, LOW );
  digitalWrite(LED_04, LOW );
  digitalWrite(LED_05, LOW );
  digitalWrite(LED_06, LOW );
  digitalWrite(LED_07, LOW );
  
  myOLED.begin();
  myOLED.setFont(SmallFont);
  myOLED.print("TELEMETRIA", CENTER, 1);
  myOLED.print("SISTEMA G2", CENTER, 10);
  myOLED.update();

  // Inicializacion de hilos
  PT_INIT(&pt1);
  PT_INIT(&pt2); 
  PT_INIT(&pt3); 
  PT_INIT(&pt4);
  
  setSyncProvider(RTC.get); 
  if  (year() < 2023){
    tm.Year = 0x35;  //2023
    tm.Month = 11;
    tm.Day = 15;
    tm.Hour = 16;
    tm.Minute = 48;
    tm.Second = 0;
    t = makeTime(tm);
    RTC.set(t);
    setTime(t);
  }
  Serial.print("Inicializando microSD... ");
  if (!SD.begin(53)) { Serial.println("Fallo en Inicializacion"); }
  else {               Serial.println("Ok."); }

  Serial.println("Initializando SIM800C.");
  delay(1000);
  
  Serial2.println("AT"); //Once the handshake test is successful, it will back to OK
  updateSerial();
  Serial2.println("AT+CSQ"); //Signal quality test, value range is 0-31 , 31 is the best
  updateSerial();
  Serial2.println("AT+CCID"); //Read SIM information to confirm whether the SIM is plugged
  updateSerial();
  Serial2.println("AT+CREG?"); //Check whether it has registered in the network
  updateSerial();
}

void loop() {
  protothread1(&pt1 );
  protothread2(&pt2 );
  protothread3(&pt3 );
  protothread4(&pt4 );
}


void updateSerial(){
  delay(500);
  while (Serial.available()) {
    Serial2.write(Serial.read());//Forward what Serial received to Software Serial Port
  }
  while(Serial2.available()) {
    Serial.write(Serial2.read());//Forward what Software Serial received to Serial Port
  }
}



