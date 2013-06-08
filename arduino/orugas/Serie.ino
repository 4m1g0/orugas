#define PT_MAX_TILT   180    // Angulos maximos y minimos del pantilt
#define PT_MAX_PAN    180
#define PT_MIN_TILT   0
#define PT_MIN_PAN    0

void parseSerial() {
  char serialData[32];
  Serial.readBytesUntil('\n', serialData, 31);
  
  
  if (serialData[0] == 'b') { // boton
    switch(serialData[1]) {
      case 'L': // laser on
        digitalWrite(laser,HIGH);
        break;
      case 'l': // laser off
        digitalWrite(laser,LOW);
        break;
      case 'F': // light off
        digitalWrite(luz,HIGH);
        break;
      case 'f': // light off
        digitalWrite(luz,LOW);
        break;
      /*case 'T': // tilt up
        if (angleTilt < 180) {
          angleTilt += 5;
          servoTilt.write(angleTilt);
        }
        break;
      case 't': // tilt down
        if (angleTilt > 0) {
          angleTilt -= 5;
          servoTilt.write(angleTilt);
        }
        break;
      case 'P': // pan left
        if (anglePan < 180) {
          anglePan += 5;
          servoPan.write(anglePan);
        }
        break;
      case 'p': // pan right
        if (anglePan > 0) {
          anglePan -= 5;
          servoPan.write(anglePan);
        }
        break;*/
      case 'c': // reset ?
        Home();
        calibraAcel();
        break;
      default:
        return; // ignore, malformed
    }
    return;
  }
  
  if (serialData[0] == 'm') { // movement
    int speed[2];
    
    if (!parseCommand(serialData+1, speed)) {
      speed[0] = constrain(speed[0], -100, 100);
      speed[1] = constrain(speed[1], -100, 100);
      if(speed[0]==0 && speed[1]==0){
        motor_standby(true);
      }
      else{
        motor_standby(false);
        motor_speed2(motor_A,speed[0]);
        motor_speed2(motor_B,speed[1]);
      }
    }
    return;
  }
  
  if (serialData[0] == 'p') { // pantilt
    int pantilt[2];
    if (!parseCommand(serialData+1, pantilt)) {
      pantilt[0] = constrain(pantilt[0], PT_MIN_PAN, PT_MAX_PAN);
      pantilt[1] = constrain(pantilt[1], PT_MIN_TILT, PT_MAX_TILT);
      servoPan.write(pantilt[0]);
      servoTilt.write(pantilt[1]);
    }
    return;
  }
  // else ignore (malformed)
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
