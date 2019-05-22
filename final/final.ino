#include <SoftwareSerial.h> // i have set device baud rate to 9600 currently
SoftwareSerial wifiSerial(2,3); //Define hardware connections
#include <RTClib.h>
#include <Wire.h>
#include <Servo.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#define ONE_WIRE_BUS 4
RTC_DS1307 RTC;
Servo myservo;
// https://thingspeak.com     gr8tm@host-info.com    LuttapyKuntham1
#define MINTNKLVL   1
#define MAXTNKLVL   90
#define FEEDHOUR    8   // 24 hour
#define FEEDMINUTE  0
/////////////////////////////////////ph////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define SensorPin A3 //pH meter Analog output to Arduino Analog Input 0
#define Offset 0.00 //deviation compensate
#define LED 13
#define samplingInterval 20
#define printInterval 800
#define ArrayLenth 40 //times of collection
int pHArray[ArrayLenth]; //Store the average value of the sensor feedback
int pHArrayIndex=0;
double avergearray(int* arr, int number);///function to calculate
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//rtc sda a4 scl a5
int ff =8;
int gbf=9;
int relay=10;
int servo =11;
// Ultra_sonic
int trigPin = 12;
int echoPin = 13;
long duration;
int distance;
// readings
int tank_lvl=0;
bool motor_State ;
DateTime now;
void change_motor_condition(int x);
void clockdisplay();
void startfishfeed();
int pos = 0; // variable to store the servo position
///for get change values as per req/// wifi
String cipstart="AT+CIPSTART=\"TCP\",\"184.106.153.149\",80";
String const_getStr = "GET /update?api_key=";
String getStr;
String apikey="WFPRNDZ9PACU9AXN";// API ID
int upload_count=0;
void uploaddataesp(float volt,float ph);
int ultrasonic_read();
// temperature 
float temperature();
float tempC=33.0;     
OneWire oneWire(ONE_WIRE_BUS);	// Setup a oneWire instance to communicate with any OneWire device
DallasTemperature sensors(&oneWire);  

void setup()
{  

    Serial.begin(9600);
    wifiSerial.begin(9600);
    Wire.begin();
    RTC.begin();
     sensors.begin();	// Start up the dellastemperture library
    pinMode(ff, INPUT_PULLUP);// sets float to input , default high
    pinMode(gbf, INPUT_PULLUP);// sets float to input , default high
    pinMode(relay,OUTPUT);
    pinMode(servo,OUTPUT);
    pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
    pinMode(echoPin, INPUT); // Sets the echoPin as an Input
    myservo.attach(servo);
    bool motor_State = false;
//rtc   

    // Check to see if the RTC is keeping time.  If it is, load the time from your computer.
    if (! RTC.isrunning()) {
    Serial.println("RTC is NOT running!");
    // This will reflect the time that your sketch was compiled
     RTC.adjust(DateTime(__DATE__, __TIME__));
    }


}

void loop()
{
    tank_lvl=ultrasonic_read();
    if (tank_lvl <= MINTNKLVL)
   { Serial.println("tank water low"); tank_lvl=5;}
    else if (tank_lvl >= MAXTNKLVL)
           { Serial.println("tank water high"); tank_lvl=95;}

    if (digitalRead(ff)==HIGH && digitalRead(gbf)==HIGH){ // both  floats are down  --start motor
        change_motor_condition(1); // start motor
        Serial.println("mo tor on");
        motor_State=true;
    }

    if (digitalRead(ff)==LOW || digitalRead(gbf)==LOW){ // one of the floats are up  --stop motor
        change_motor_condition(0);
        Serial.println("motor  off");
        motor_State=false;}
     
    now = RTC.now(); 
    clockdisplay();
    if (now.hour()==FEEDHOUR && now.minute()==FEEDMINUTE && now.second()==0)
    startfishfeed();

    tempC=temperature();    
///////////////////////////////////ph/////////////////////////////
    static unsigned long samplingTime = millis();
    static unsigned long printTime = millis();
    static float pHValue,voltage;
  
    if(millis()-samplingTime > samplingInterval){
    pHArray[pHArrayIndex++]=analogRead(SensorPin);
    if(pHArrayIndex==ArrayLenth)pHArrayIndex=0;
    voltage = avergearray(pHArray, ArrayLenth)*5.0/1024;
    pHValue = 3.5*voltage+Offset;
        
    samplingTime=millis();
    }
    if(millis() - printTime > printInterval){ //Every 800 milliseconds, print a numerical, convert the state of the LED indicator
    Serial.print("Voltage:");
    Serial.print(voltage,2);
    Serial.print(" pH value: ");
    pHValue=pHValue+3.0;    ////////////uda_yip
    Serial.println(pHValue,2);/////// --------------------PHVALUE
    printTime=millis();
    }

///////////////////////////////////////////////////////////////////////////// 
//  loop ends here by uploading data every 15 seconds 
 if (millis()% 15000 > upload_count) 
       {  uploaddataesp(voltage,pHValue);upload_count++;
       Serial.println("upload count=");Serial.print(upload_count++);Serial.println();Serial.println();} 
//////////////////////////////////////////
}











///////////////////////////////////////////////iot/// this function has a 4 sec delay dont forget!!!!
void uploaddataesp(float volt,float ph){
  getStr=const_getStr;
  wifiSerial.println(cipstart);
  delay(2000);
  getStr +=apikey;
  /// adding fields
  getStr +="&field1=";
  //Serial.println("tanklvl");
  //Serial.println(tank_lvl);
  getStr += String(tank_lvl);
  getStr +="&field2=";
  getStr += String(motor_State);
  getStr +="&field3=";
  getStr += String(volt);
  getStr +="&field4=";  ///getStr +="&field4=";
  getStr += String(ph);///getStr += String(ph);
  getStr +="&field5=";
  getStr += String(tempC);
  getStr +="\r\n";
  Serial.println(getStr);
  wifiSerial.println("AT+CIPSEND="+String((getStr.length()+2)));
  delay(250);
  wifiSerial.println(getStr);
  

}

///////////////////////////////////

int ultrasonic_read()
{
digitalWrite(trigPin, LOW);
delayMicroseconds(2);
digitalWrite(trigPin, HIGH);   // Sets the trigPin on HIGH state for 10 micro seconds
delayMicroseconds(10);
digitalWrite(trigPin, LOW);
duration = pulseIn(echoPin, HIGH);// Reads the echoPin, returns the sound wave travel time in microseconds
distance= duration*0.034/2;// Calculating the distance in cm
Serial.print("Tank_lvl_Distance:");// Prints the distance on the Serial Monitor
Serial.print(distance);Serial.print("cm "); 
return(distance);
}

void change_motor_condition(int x) // 0 to stop else will be onlayon
{if (x==0)
    {digitalWrite(relay,HIGH);Serial.println("relay off");}//relay works on active LOW
    else    { digitalWrite(relay,LOW);Serial.println("relay onn");} //motor on 
}

void clockdisplay(){
    
    Serial.print(now.month());
    Serial.print('/');
    Serial.print(now.day());
    Serial.print('/');
    Serial.print(now.year());
    Serial.print(' ');
    Serial.print(now.hour(), DEC);
    Serial.print(':');
    Serial.print(now.minute(), DEC);
    Serial.print(':');
    Serial.print(now.second(), DEC);
    Serial.println();    
    delay(1000);
}

void startfishfeed()
{
  // need to be configured 
   for(pos = 0; pos <= 180; pos += 1) // goes from 0 degrees to 180 degrees
  { // in steps of 1 degree
    myservo.write(pos); // tell servo to go to position in variable 'pos'
    delay(15); // waits 15ms for the servo to reach the position
  }
  for(pos = 180; pos>=0; pos-=1) // goes from 180 degrees to 0 degrees
  {
    myservo.write(pos); // tell servo to go to position in variable 'pos'
    delay(15); // waits 15ms for the servo to reach the position
  }

}



//////////////////////////////////////////////////////////////////////////////
double avergearray(int* arr, int number){
int i;
int max,min;
double avg;
long amount=0;
if(number<=0){
Serial.println("Error number for the array to avraging!/n");
return 0;
   }

if(number<5){ //less than 5, calculated directly statistics
for(i=0;i<number;i++){
amount+=arr[i];
                     }
avg = amount/number;
return avg;
         }
else{
if(arr[0]<arr[1]){
min = arr[0];max=arr[1];
}
else{
min=arr[1];max=arr[0];
}
for(i=2;i<number;i++){
if(arr[i]<min){
amount+=min; //arr<min
min=arr[i];
}
else {
if(arr[i]>max){
amount+=max; //arr>max
max=arr[i];
}
else{
amount+=arr[i]; //min<=arr<=max
}
}
}
avg = (double)amount/(number-2);
}
return avg;

}

float temperature()
{ float val;
 // Send the command to get temperatures
  sensors.requestTemperatures(); 
  val=sensors.getTempCByIndex(0);
  //print the temperature in Celsius
  Serial.println("Temperature: ");
  Serial.print(val);
  Serial.print((char)176);//shows degrees character oc
  Serial.println("C");
  return val;
}



/*




AT
response : OK

AT+GMR

14:46:30.876 -> AT version:1.3.0.0(Jul 14 2016 18:54:01)
14:46:30.876 -> SDK version:2.0.0(5a875ba)
14:46:30.923 -> Farylink Technology Co., Ltd. v1.0.0.2
14:46:30.969 -> May 11 2017 22:23:58
14:46:30.969 -> OK

print mac

AT+CIPSTAMAC?

reset module 
AT+RST

AT+CWMODE - WIFI mode（station, AP, station + AP）
Variant   Command   Response  Function
Test  AT+CWMODE=?   +CWMODE:(1-3) OK  List valid modes
Query   AT+CWMODE?  +CWMODE:mode OK   Query AP’s info which is connect by ESP8266.
Execute   AT+CWMODE=mode  OK  Set AP’s info which will be connect by ESP8266.

Parameters:

    mode： An integer designating the mode of operation either 1, 2, or 3.
    1 = Station mode (client)
    2 = AP mode (host)
    3 = AP + Station mode (Yes, ESP8266 has a dual mode!)

########AT+CWLAP - Lists available APs
Variant   Command   Response  Function
Set   AT+CWLAP=ssid,mac,ch  +CWLAP:ecn,ssid,rssi,mac OK   Search available APs with specific conditions.
Execute   AT+CWLAP  AT+CWLAP:ecn,ssid,rssi,mac OK   Lists available Access Points.
Parameters:

    ecn:
        0 = OPEN
        1 = WEP
        2 = WPA_PSK
        3 = WPA2_PSK
        4 = WPA_WPA2_PSK
    ssid: String, SSID of AP
    rssi: signal strength
    mac: String, MAC address
  

########AT+CWJAP - Connect to AP

Variant   Command   Response  Function
Query   AT+CWJAP?   + CWJAP:ssid OK   Prints the SSID of Access Point ESP8266 is connected to.
Execute   AT+CWJAP=ssid,pwd   OK  Commands ESP8266 to connect a SSID with supplied password.
Parameters:

    ssid：String, AP’s SSID
    pwd：String, not longer than 64 characters

Example：

AT+CWJAP="my-test-wifi","1234test"  
Example AT+CWJAP?：

+CWJAP:"my-test-wifi"  

#########AT+CWQAP - Disconnect from AP
Variant   Command   Response  Function
Execute   AT+CWQAP  OK  Disconnect ESP8266 from the AP is currently connected to.

  
#########AT+CIPSTAMAC - Set MAC address of ESP8266 station
Variant   Command   Response  Function
Query   AT+CIPSTAMAC?   +CIPSTAMAC:mac OK   Print current MAC ESP8266’s address.
Execute   AT+CIPSTAMAC=mac  OK  Set ESP8266’s MAC address.
Parameters:

    mac： String, MAC address of the ESP8266 station.
  
#########AT+CIPSTA - Set IP address of ESP8266 station
Variant   Command   Response  Function
Query   AT+CIPSTA?  +CIPSTA:ip OK   Get IP address of ESP8266 station.
Execute   AT+CIPSTA=ip  OK  Set ip addr of ESP8266 station.
Parameters:









*/
