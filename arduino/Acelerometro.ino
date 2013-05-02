//////////////////////////////////////////////////////////////////
//©2011 bildr
//Released under the MIT License - Please reuse change and share
//Simple code for the ADXL335, prints calculated orientation via serial
//////////////////////////////////////////////////////////////////
void acelerometro(boolean debug){
  //Analog read pins
  const int xPin = 0;
  const int yPin = 1;
  const int zPin = 2;


  //read the analog values from the accelerometer
  int xRead = analogRead(xPin);
  int yRead = analogRead(yPin);
  int zRead = analogRead(zPin);

  //convert read values to degrees -90 to 90 - Needed for atan2
  int xAng = map(xRead, 267, 417, -90, 90);
  int yAng = map(yRead, 271, 410, -90, 90);
  int zAng = map(zRead, 277, 415, -90, 90);

  //Caculate 360deg values like so: atan2(-yAng, -zAng)
  //atan2 outputs the value of -π to π (radians)
  //We are then converting the radians to degrees
  x = (RAD_TO_DEG * (atan2(-yAng, -zAng) + PI))+offsetX;
  y = (RAD_TO_DEG * (atan2(-xAng, -zAng) + PI))+offsetY;

  if(debug){//Output the caculations
  Serial.print("[Acelerometro] ");
  Serial.print("Pitch: ");
  Serial.print(y);
  Serial.print(" | Roll: ");
  Serial.println(x);
  }
}

void calibraAcel(){
  //Analog read pins
  const int xPin = 0;
  const int yPin = 1;
  const int zPin = 2;

  //read the analog values from the accelerometer
  int xRead = analogRead(xPin);
  int yRead = analogRead(yPin);
  int zRead = analogRead(zPin);

  //convert read values to degrees -90 to 90 - Needed for atan2
  int xAng = map(xRead, 267, 417, -90, 90);
  int yAng = map(yRead, 271, 410, -90, 90);
  int zAng = map(zRead, 277, 415, -90, 90);
  
  offsetX = 0-(RAD_TO_DEG * (atan2(-yAng, -zAng) + PI));
  offsetY = 0-(RAD_TO_DEG * (atan2(-xAng, -zAng) + PI));
}  


void ultrasonidos(){
int sum=0;
int anVolt;
for(int i = 0; i < 5 ; i++)
  {
    //Used to read in the analog voltage output that is being sent by the MaxSonar device.
    //Scale factor is (Vcc/512) per inch. A 5V supply yields ~9.8mV/in
    anVolt = analogRead(A3);
    sum += anVolt;

  }  
  distancia =  1.39*(sum/5);

  
}

void temp(){
temperatura=(analogRead(A4)*0.48828125)-273;  
}

void luminosidad(){
lum=map(analogRead(A5),0,1024,0,100);
}

