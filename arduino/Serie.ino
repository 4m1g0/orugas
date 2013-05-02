int leeSerie(int posiciones){
  char in[posiciones];
  int out=0;
  memset(in, 0, sizeof(in)+1);
  for (int i=0;i<posiciones;i++){
    delay(5);
    in[i]=Serial.read();
  }
  out=atoi(in);
  return out;
}
