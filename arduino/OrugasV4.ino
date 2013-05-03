#include <Servo.h> 

/*********/
#define out_STBY 9
#define out_B_PWM 12
#define out_A_PWM 6
#define out_A_IN2 7
#define out_A_IN1 8
#define out_B_IN1 10
#define out_B_IN2 11
#define motor_A 0
#define motor_B 1
#define motor_AB 2

/*********/

const int loopTime = 100;
const int positionTime = 100;
const int enviromentTime = 2000;

unsigned long loopTimer=0, positionTimer=0, enviromentTimer=0;

int motorA1 = 3;
int motorA2 = 5;

int motorB1 = 6;
int motorB2 = 11;
int laser=38;
int luz=50;

int anglePan=90;
int angleTilt=90;

long distancia=0;
long temperatura=0;
long lum=0;

float longitud;
float latitud;
float orientacion;

//Acelerometro
int x;
int y;
int z;
  
int offsetX=0;
int offsetY=0;

Servo servoPan;
Servo servoTilt;

/************************************/

void setup() {
  Serial.begin(9600); // Start serial communication

  pinMode(out_STBY,OUTPUT);
  pinMode(out_A_PWM,OUTPUT);
  pinMode(out_A_IN1,OUTPUT);
  pinMode(out_A_IN2,OUTPUT);
  pinMode(out_B_PWM,OUTPUT);
  pinMode(out_B_IN1,OUTPUT);
  pinMode(out_B_IN2,OUTPUT);
  pinMode(laser,OUTPUT);
  pinMode(luz,OUTPUT);
  pinMode(A0, INPUT);
  pinMode(A1, INPUT);
  pinMode(A2, INPUT);
  pinMode(A3, INPUT);
  pinMode(A4, INPUT);
  pinMode(A5, INPUT);
  servoPan.attach(4);
  servoTilt.attach(5);

  Home();
}

void loop() {
  loopTimer = millis();

  // if available read serial
  if (Serial.available()) {
    parseSerial();
  }

  // if necesary read enviroment sensors and send data
  if (millis() - enviromentTimer > enviromentTime) {
    // light, temperature, and gps
    temp();
    luminosidad();
    Serial.print("{\"e\":[");
    Serial.print(lum);
    Serial.print(",");
    Serial.print(temperatura);
    Serial.print(",");
    Serial.print(latitud);
    Serial.print(",");
    Serial.print(longitud);
    Serial.println("]}");
    
    enviromentTimer = millis();
  }

  // if necesary read position sensors and send data
  if (millis() - positionTimer > positionTime) {
    // acelerometer, distance, and compass
    // TODO: get GPS or compass orientation
    acelerometro(false);
    ultrasonidos();
    
    Serial.print("{\"p\":[");
    Serial.print(y);
    Serial.print(",");
    Serial.print(x);
    Serial.print(",");
    Serial.print(distancia);
    Serial.print(",");
    Serial.print(orientacion); // TODO: set this variable
    Serial.println("]}");
    
    positionTimer = millis();
  }

  // Si el serial no esta sobrecargado esperamos
  while (millis() - loopTimer < loopTime && !Serial.available())
    delay(20);
}

void Home(){
  motor_standby(true);
  anglePan=90;
  angleTilt=90;
  servoTilt.write(angleTilt); 
  delay(1000);
  servoPan.write(anglePan);
  digitalWrite(13, HIGH);
  delay(200);
  digitalWrite(13, LOW);
}  
  
