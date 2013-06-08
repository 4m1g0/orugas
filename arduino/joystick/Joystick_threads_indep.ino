int x = 0;
int y = 0;
int calx=0;
int caly=0;
long joyDec=0;
boolean joyUp=false;
boolean joyDown=false;
boolean joyRight=false;
boolean joyLeft=false;
boolean joyBarUp=false;
boolean joyBarDown=false;
boolean joyButtonUp=false;
boolean joyButtonDown=false;
boolean joyGrip=false;
boolean joyTrigger=false;
boolean lastTimeJoyOn=false;
int brown=12;
int red=11;
int orange=10;
int yellow=9;
int green=8;
int blue=7;
int white=6;
int throttle, direction = 0; //throttle (Y axis) and direction (X axis)
int leftMotor,leftMotorScaled = 0; //left Motor helper variables
float leftMotorScale = 0;
int rightMotor,rightMotorScaled = 0; //right Motor helper variables
float rightMotorScale = 0;
float maxMotorScale = 0; //holds the mixed output scaling factor
int deadZone = 10; //jostick dead zone 

void setup() {
  pinMode(brown,  OUTPUT);
  pinMode(red,    OUTPUT);
  pinMode(orange, OUTPUT);
  pinMode(yellow, OUTPUT);
  pinMode(green,  INPUT);
  pinMode(blue,   INPUT);
  pinMode(white,  INPUT);
  // initialize serial communications at 9600 bps:
  Serial.begin(115200); 
  calx = analogRead(0);
  caly = analogRead(1);
}

void loop() {
  readJoystick();
  delay(100);                     
}

void readJoystick(){
  ///////////////////////ANALOG/////////////////////////////////
  //code inspiration: Kamil Zadora 
  //http://www.youtube.com/watch?v=sGpgWDIVsoE
  // read the analog axis and reescale from 0..1023 to -255..255
  y = analogRead(1); 
  throttle = map(analogRead(1),847,942,255,-255);
  delayMicroseconds(100);
  x = analogRead(0);
  direction = -map(analogRead(0),480,732,255,-255);
  
  //mix throttle and direction
  leftMotor = throttle+direction;
  rightMotor = throttle-direction;
    
  //calculate the scale of the results
  leftMotorScale =  leftMotor/100.0;
  leftMotorScale = abs(leftMotorScale);
  rightMotorScale =  rightMotor/100.0;
  rightMotorScale = abs(rightMotorScale);

  //choose the max scale value if it is above 1
  maxMotorScale = max(leftMotorScale,rightMotorScale);
  maxMotorScale = max(1,maxMotorScale);

  //and apply it to the mixed values
  leftMotorScaled = constrain(leftMotor/maxMotorScale,-100,100);
  rightMotorScaled = constrain(rightMotor/maxMotorScale,-100,100);
  
  if(abs(leftMotorScaled)<deadZone)leftMotorScaled=0;
  if(abs(rightMotorScaled)<deadZone)leftMotorScaled=0; 
  ///////////////////////BUTTON/////////////////////////////////
  digitalWrite(brown,HIGH);
  joyBarUp = digitalRead(white);
  joyDown = digitalRead(green);
  joyTrigger = digitalRead(blue);
  digitalWrite(brown,LOW);
  
  digitalWrite(red,HIGH);
  joyBarDown = digitalRead(white);
  joyLeft = digitalRead(green);
  joyGrip = digitalRead(blue);
  digitalWrite(red,LOW);
  
  digitalWrite(orange,HIGH);
  joyUp = digitalRead(green);
  joyButtonUp = digitalRead(blue);
  digitalWrite(orange,LOW);
  
  digitalWrite(yellow,HIGH);
  joyRight = digitalRead(green);
  joyButtonDown = digitalRead(blue);
  digitalWrite(yellow,LOW);
  
  joyDec=joyButtonDown+(joyButtonUp*2)
  +(joyLeft*4)+(joyRight*8)+(joyDown*16)+(joyUp*32);
  /*
  //large string
  if(joyTrigger){
    // print the results to the serial monitor:
    Serial.print("{\"r\":"); 
    Serial.print(-rightMotorScaled);      
    Serial.print(",\"l\":");      
    Serial.print(-leftMotorScaled);   
    Serial.print(",\"b\":");   
    Serial.print(joyDec);
    Serial.println("}");
    lastTimeJoyOn=true;
  }
  else if(lastTimeJoyOn){
    lastTimeJoyOn=false;
    Serial.println("S");
  }
 */
  //short string
  if(joyTrigger){
    // print the results to the serial monitor:
    Serial.print("["); 
    Serial.print(-rightMotorScaled);      
    Serial.print(",");      
    Serial.print(-leftMotorScaled);   
    Serial.print(",");   
    Serial.print(joyDec);
    Serial.println("]");
    lastTimeJoyOn=true;
  }
  else if(lastTimeJoyOn){
    lastTimeJoyOn=false;
    Serial.println("[0,0,0]");
  }
  
}
