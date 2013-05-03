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
      case 'T': // tilt up
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
        break;
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
      if (speed[0] == speed[1] == 0)
        motor_standby(true); // paramos los motores FIXME: y si se pierde el paquete con ceros?
      else {
        motor_standby(false); 
        motor_speed2(motor_A,speed[0]);
        motor_speed2(motor_B,speed[1]);
      }
    }
    return;
  }
  // else ignore (malformed)
}



int parseCommand(char* command, int* returnValues)
{
  // parsing state machine
  byte i = 2, j = 0, sign = 0;
  int temp = 0;
  while(*(command + i) != '\0')
  {
    if (i > 6) return -1;
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
        temp = temp * 10 + *(command + i) - 48;
    }
    i++;
  }
  // set last return value
  returnValues[j] = sign?-temp:temp;
  return 0;
}

