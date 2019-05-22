#define MOTORRESPNSETIME 1000
#define TIMEFORPROCESSING 10000
int glsensor=3;
int MSR=4; //motor start  relay
int MRR=5;
int Csensor=6;
int Cmp1=7; // motor h bridge pins counter motor pin 1
int Cmp2=8; // motor h bridge pins
int count=0;
int sensor1=9;
int sensor2=10;
int sensor3=11;
int Lmp1=12;// motor h bridge pins loading motor pin 1
int Lmp2=13;// motor h bridge  motor pin 2
/*////////////////sensor pin positions
 *               
 *           sensor1
 * ---------
 * sensor2 |        |Csensor
 *         |        |
 *         |        |
 *         |        |
 *         |        |        
 *         |        |
 * sensor3 |        |
 * ---------        |
 *                  |  
 *                  |
 *                  |                  
 *                  |
 *                  |glsensor
 */
bool glass; // glass positioned or not
bool motor_state;
bool stock;

///function prototype
void StartMainMotor(); //completed
void StopMainMotor(); //completed
void RunCounter();// completed
void notify();    //completed
void rettobottom();//completed
void loadtotop();//completed
bool ChkglSensor();
bool checkstock();// completed
