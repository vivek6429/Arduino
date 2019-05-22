void RunCounter(){
  bool nextstate=LOW;//condition when counter is spinning 
  bool stopstate=HIGH;//after one rotation sensor pin will be high
  
  if(digitalRead(Csensor)==HIGH){// START ON CENSOR HIGH
    while(1){
      digitalWrite(Cmp1,HIGH);// TURNS MOTOR
      digitalWrite(Cmp1,LOW); // THESE PINS ARE TO DRIVER 
                    // SENSOR WILL RETURN HIGH UNTIL THE object moves awy----then low----then high(stopcondition);
      if(digitalRead(Csensor)!=nextstate)
        continue;// that is skip code below until sensor reading is low
      
      if (digitalRead(Csensor)==stopstate){
        count+=1;
        break;
      }
      
       
    }
  }
}

void loadtotop(){
  while(digitalRead(sensor1)==LOW && digitalRead(sensor2)==LOW){ //stop if sensor 1 or 2 becomes high
    digitalWrite(Lmp1,HIGH);digitalWrite(Lmp2,LOW);
  }
  
}
