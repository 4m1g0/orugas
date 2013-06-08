#define PT_SPEED      2     // velocidad de movimiento del pantilt
#define PT_MAX_TILT   180   // Angulos maximos y minimos del pantilt
#define PT_MAX_PAN    180
#define PT_MIN_TILT   0
#define PT_MIN_PAN    0

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

int panPos=PT_MAX_PAN+PT_MIN_PAN/2;
int tiltPos=PT_MAX_TILT+PT_MIN_TILT/2;

long distancia=0;
long temperatura=0;
long lum=0;

float longitud=43.3621;
float latitud=-8.4125;
float orientacion=0;

//Acelerometro
int acelX;
int acelY;

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

// timers
unsigned long readJoystickTimer=0, sendPositionTimer=0, sendEnviromentTimer;

const int readJoystickTime=100, sendPositionTime=100, sendEnviromentTime=400;

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
  if (millis() - readJoystickTimer > readJoystickTime) {
    readJoystick();
    readJoystickTimer = millis();
  }
  
  if (orugas.available()) { // Leemos el serial tan pronto este disponible, para evitar saturar la entrada
    Serial.readBytesUntil('\n', serialData, 31);
    long data[31];
    if (serialData[0] == 'e') {
      if (!parseCommand(serialData+1, data)) {
          lum = data[0];
          temperatura = data[1];
          latitud = data[2];
          longitud = data[3];
      } 
    } else if (serialData[0] == 'p') {
      if (!parseCommand(serialData+1, data)) {
        acelY = data[0];
        acelX = data[1];
        distancia = data[2];
        orientacion = data[3];
      }
    }
  }
  
  while (Serial.available()) { // Miramos si hay comandos desde terminal y se los enviamos directamente a orugas
    orugas.print(Serial.read());
  }
  
  if (millis() - sendPositionTimer > sendPositionTime) {
    sendPositionData();
    sendPositionTimer = millis();
  }
  
  if (millis() - sendEnviromentTimer > sendEnviromentTime) {
    sendEnviromentData();
    sendEnviromentTimer = millis();
  }
}

void sendPositionData() {
  Serial.print("{\"p\"=[");
  Serial.print(panPos);
  Serial.print(",");
  Serial.print(tiltPos);
  Serial.print(",");
  Serial.print(acelX);
  Serial.print(",");
  Serial.print(acelY);
  Serial.print(",");
  Serial.print(distancia);
  Serial.print(",");
  Serial.print(orientacion);
  Serial.print("]}\n");
}

void sendEnviromentData() {
  erial.print("{\"e\"=[");
  Serial.print(lum);
  Serial.print(",");
  Serial.print(temperatura);
  Serial.print(",");
  Serial.print(latitud);
  Serial.print(",");
  Serial.print(longitud);
  Serial.print("]}\n");
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
  
  joyDec=joyButtonDown+(joyButtonUp*2);
  // botones
  //TODO:
  

  // posicion del pantilt
  if (joyRight || joyUp || joyDown || joyLeft){ // si ha cambiado
    panPos += joyRight*PT_SPEED - joyLeft*PT_SPEED;
    tiltPos += joyUp*PT_SPEED - joyDown*PT_SPEED;
    panPos = constrain(panPos, PT_MIN_PAN, PT_MAX_PAN);
    tiltPos = constrain(tiltPos, PT_MIN_TILT, PT_MAX_TILT);
    
    orugas.print("p");
    orugas.print(panPos);
    orugas.print(",");
    orugas.print(tiltPos);
    orugas.print("\n");
  }

  // movimiento solo se envia con trigger activo
  if(joyTrigger){
    orugas.print("m");
    orugas.print(-rightMotorScaled);
    orugas.print(",");
    orugas.print(-leftMotorScaled);
    orugas.print("\n");
  }
  else // enviamos 0, para que orugas no se mueva aun que hayan perdido paquetes
    orugas.println("m0,0");
  }
}

int parseCommand(char* command, int* returnValues)
{
  // parsing state machine
  byte i = 0, j = 0, sign = 0;
  int temp = 0;
  while(*(command + i) != '\0' && *(command + i) != '\n' && *(command + i) != '\r')
  {
    switch(*(command + i))
    {
      case ',':
        if (j == 1) return -1;
        returnValues[j++] = sign?-temp:temp;
        sign = 0;
        temp = 0;
        break;
      case '-':
        sign = 1;
        break;
      default:
        if (*(command+i) < 48)
          break;
        temp = temp * 10 + *(command + i) - 48;
    }
  }
  // set last return value
  returnValues[j] = sign?-temp:temp;
  return 0;
}
