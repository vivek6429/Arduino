void StartMainMotor(){
  digitalWrite(MRR,LOW);
  delay(250);
  digitalWrite(MSR,LOW);
  delay(MOTORRESPNSETIME);
  digitalWrite(MSR,HIGH);
  motor_state=HIGH;
  
}
void StopMainMotor(){
   digitalWrite(MSR,HIGH);
   delay(100);
   digitalWrite(MRR,HIGH);
   delay(250);
}
