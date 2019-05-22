
void loop(){

  if( digitalRead(sensor1)==LOW){
    if(checkstock()==HIGH){
      loadtotop();
    }
  }
  
  if(checkstock()==HIGH){
    if(ChkglSensor()==HIGH){
      StartMainMotor();
      RunCounter();
      delay(TIMEFORPROCESSING);
      StopMainMotor();
      }
  }else {notify();rettobottom();}

  if(stock==HIGH && digitalRead(sensor1)==LOW)
    loadtotop();
  
}
