void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(Csensor,INPUT);
  pinMode(glsensor,INPUT);
  pinMode(MSR,OUTPUT);
  digitalWrite(MSR,HIGH);
  pinMode(MRR,OUTPUT);
  digitalWrite(MSR,HIGH);
  pinMode(Cmp1,OUTPUT);
  digitalWrite(Cmp1,HIGH);  
  pinMode(Cmp2,OUTPUT);
  digitalWrite(Cmp2,HIGH);
  pinMode(Lmp1,OUTPUT); 
  digitalWrite(Lmp1,LOW);
  pinMode(Lmp2,OUTPUT);
  digitalWrite(Lmp2,LOW); 
}
