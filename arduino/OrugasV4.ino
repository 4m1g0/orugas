
#include <Servo.h> 
//*******
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


//*********


char val;

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

//Acelerometro
  int x;
  int y;
  int z;
int offsetX=0;
int offsetY=0;

int vel=0;

Servo servoPan;
Servo servoTilt;


//*************************************************************************************************

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
  if (Serial.available()) { // If data is available to read,
    val = Serial.read(); // read it and store it in val
  }
  if (val == 'A') { //Hacia adelante
          vel=leeSerie(2);
          motor_standby(false);
          motor_speed2(motor_A,vel);
          motor_speed2(motor_B,vel);
  } 
  else if (val == 'R') {//Hacia detras
          vel=leeSerie(2);
          motor_standby(false);
          motor_speed2(motor_A,-vel);
          motor_speed2(motor_B,-vel);
  }
  else if (val == 'D') {//Derecha
          vel=leeSerie(2);
          motor_standby(false);
          motor_speed2(motor_A,vel);
          motor_speed2(motor_B,-vel);
  } 
  else if (val == 'I') {//Izquierda
          vel=leeSerie(2);
          motor_standby(false);
          motor_speed2(motor_A,-vel);
          motor_speed2(motor_B,vel);
  } 
  else if (val == 'S') {//Standby
          motor_standby(true);
  } 
  else if (val == 'P') {//Pan izquierda
    if (anglePan < 180) {
      anglePan += 5;
    } 
  } 
  else if (val == 'x') {
    anglePan =leeSerie(3);
  }  
  else if (val == 'y') {
    angleTilt =leeSerie(3);
  } 
  else if (val == 'p') {//Pan Derecha
    if (anglePan > 0) {
      anglePan -= 5;
    } 
  } 
  else if (val == 'T') {//Tilt Arriba
    if (angleTilt < 180) {
      angleTilt += 5;
    } 
  } 
  else if (val == 't') {//Tilt Abajo
    if (angleTilt > 0) {
      angleTilt -= 5;
    } 
  }
  else if (val == 'H') {//Home
    Home();
    }      
  else if (val == 'L') {//Home
    digitalWrite(laser,HIGH);
    }      
  else if (val == 'l') {//Home
    digitalWrite(laser,LOW);
    }      
  else if (val == 'F') {//Home
    digitalWrite(luz,HIGH);
    }      
  else if (val == 'f') {//Home
    digitalWrite(luz,LOW);
    }    
    
    
    
    
    
    
    
    
    
    
  else if (val == 'Z') {//
    int AX=analogRead(A0)/4;
    if(AX<=99) {Serial.print(0);}
    Serial.print(AX);
    val=0;
  } 
  else if (val == 'm') {
    acelerometro(false);
    ultrasonidos();
    temp();
    luminosidad();
    Serial.print("1");
    Serial.print(",");
    Serial.print(y);
    Serial.print(",");
    Serial.print(x);
    Serial.print(",");
    Serial.print(distancia);
    Serial.print(",");
    Serial.print(lum);
    Serial.print(",");
    Serial.println(temperatura);
    

    val=0;
  }
  else if (val == 'c') {//Calibrado Acelerometro
    Home();
    calibraAcel();
    }   
  else {
    digitalWrite(13, HIGH);
  }
  servoPan.write(anglePan);
  servoTilt.write(angleTilt);
  
  delay(50); // Wait 100 milliseconds for next reading
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
  
