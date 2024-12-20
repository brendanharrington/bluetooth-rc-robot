#include <SoftwareSerial.h>

volatile bool var4 = LOW;
volatile bool var2 = LOW;
const int pinOn = 6;             //D6 is on/off switch , active low
volatile int L_counter = 0;      //L encouder counter
volatile int R_counter = 0;      //R encouder counter
volatile int target = 15 * 660;  //target distance
const int pinLeftPWM = 10;       //connect D10 to left Vref bia Left RC LPF
const int pinLeftForward = 11;   //connect D11 to FWD
const int pinLeftBackward = 12;  //connect D12 to Back
int ClockWise = 1;               //to use in function to configure easier
int CounterClockWise = 2;        // to use in function to configure easier
int Forward = 1;                 //to use in function to configure easier
int Backward = 2;                //to use in function to configure easier
//right pins
const int pinRightPWM = 9;       //connect D9 to left Vref bia Left RC LPF
const int pinRightForward = 7;   //connect D7 to FWD
const int pinRightBackward = 8;  //connect D8 to Back
int LF = 0;                      //left fron status 1=high 0 = low
int RF = 0;                      //right fron status 1=high 0 = low
int LB = 0;                      //left back status 1=high 0 = low
int RB = 0;                      //right back status 1=high 0 = low
int speedL = 150;                //set speed to 0
int speedR = 250;                // set speed to 0


SoftwareSerial HM10(3, 2);  // RX = 2, TX = 3

char appData;
String inData = "";
const int trigPin = 5;
const int echoPin = 6;
// defines variables
long duration;
int distance;

void setup() {
  Serial.begin(9600);  // Set the baud rate for the Serial Monitor
  Serial.println("HM10 serial started at 9600");
  HM10.begin(9600);  // set HM10 serial at 9600 baud rate
  pinMode(pinLeftForward, OUTPUT);
  pinMode(pinLeftBackward, OUTPUT);
  pinMode(pinLeftPWM, OUTPUT);
  pinMode(pinRightForward, OUTPUT);
  pinMode(pinRightBackward, OUTPUT);
  pinMode(pinRightPWM, OUTPUT);
  pinMode(13, OUTPUT);       //to debug counter
  pinMode(trigPin, OUTPUT);  // Sets the trigPin as an Output
  pinMode(echoPin, INPUT);   // Sets the echoPin as an Input

  digitalWrite(pinLeftForward, LF);    //stop forward
  digitalWrite(pinLeftBackward, LB);   //stop backward
  analogWrite(pinLeftPWM, speedL);     //set Vref, duty cycle 200/255
  digitalWrite(pinRightForward, RF);   //stop forward
  digitalWrite(pinRightBackward, RB);  //stop backward

  analogWrite(pinRightPWM, speedR);  //set Vref, duty cycle 150/255
}

void loop()

{

  ReadController();



  if (inData == "F") {
    RF = 1;
    LF = 1;
    RB = 0;
    LB = 0;

    UpdateDirecion(RF, LF, RB, LB);  //UpdateDirecion(int RF,int LF, int RB,int LB)
  }

  if (inData == "B") {
    RF = 0;
    LF = 0;
    RB = 1;
    LB = 1;
    speedR = 150;
    speedL = 150;
    UpdateDirecion(RF, LF, RB, LB);  //UpdateDirecion(int RF,int LF, int RB,int LB)
  }
  if (inData == "R") {
    RF = 0;
    LF = 1;
    RB = 1;
    LB = 0;
    speedR = 100;
    speedL = 100;
    UpdateDirecion(RF, LF, RB, LB);  //UpdateDirecion(int RF,int LF, int RB,int LB)
  }
  if (inData == "L") {
    RF = 1;
    LF = 0;
    RB = 0;
    LB = 1;

    UpdateDirecion(RF, LF, RB, LB);  //UpdateDirecion(int RF,int LF, int RB,int LB)
  }
  if (inData == "S") {
    RF = 0;
    LF = 0;
    RB = 0;
    LB = 0;

    UpdateDirecion(RF, LF, RB, LB);  //UpdateDirecion(int RF,int LF, int RB,int LB)
  }
  //  Serial.println(inData);
  int d = safe();
  if (d != 0 & d <= 15 & RF == 1 & LF == 1) {

    inData = "S";
    Serial.println(inData);
    Serial.println(d);
  }

  //HM10.write(distance);
}



void UpdateDirecion(int RF, int LF, int RB, int LB) {
  digitalWrite(pinLeftForward, LF);   //stop forward
  digitalWrite(pinLeftBackward, LB);  //stop backward
  digitalWrite(pinRightForward, RF);  //stop forward
  digitalWrite(pinRightBackward, RB);
}


void ReadController() {

  HM10.listen();  // listen the HM10 port

  while (HM10.available() > 0) {  // if HM10 sends something then read
    appData = HM10.read();
    inData = String(appData);  // save the data in string format
    Serial.write(appData);
    if (Serial.available()) {  // Read user input if available.
      delay(10);
      HM10.write(Serial.read());
    }
    Serial.println(inData);
    // if(inData!="R" & inData!="F" & inData!="B" & inData!="L")
    // inData="S";
  }
}
int safe() {
  // Clears the trigPin
  distance = 0;
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);
  // Calculating the distance
  distance = duration * 0.034 / 2;
  // Prints the distance on the Serial Monitor
  //Serial.print("Distance: ");
  //Serial.println(distance);
  return distance;
}
void speedcontroll() {
  analogWrite(pinRightPWM, speedR);  //set Vref, duty cycle 150/255
  analogWrite(pinLeftPWM, speedL);   //set Vref, duty cycle 150/255
}