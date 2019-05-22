void notify()
{Serial.println("STOCK OUT");};

bool checkstock(){
  if((digitalRead(sensor2)==HIGH) && (digitalRead(sensor1)==LOW)){
    stock=LOW;
    return LOW;} 
  else {stock=HIGH;return HIGH;} 
    
}

void rettobottom(){
  while(digitalRead(sensor3)==LOW){ //stop if sensor 3 becomes high
    digitalWrite(Lmp1,LOW);digitalWrite(Lmp2,HIGH);  // runs in opposite direction
  }
  
}
bool ChkglSensor(){
  if(digitalRead(glsensor)==HIGH){
    glass=HIGH; return HIGH;
  }else {glass=LOW ;return LOW;}
}
