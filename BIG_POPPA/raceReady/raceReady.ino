#include <Servo.h>
Servo gripper;
int openPos = 48;
int closePos = 125;

const int leftF = 5; // motor left - going forward
const int leftB = 9; // motor left - going backwards
const int rightF = 11; //motor right - going forward
const int rightB = 6; // motor right - going backwards

const int rotaryLeft = 3; // rotary sensor, left side
const int rotaryRight = 2; // rotary sensor, right side
int ticksLeft;
int ticksRight;

int previousState;
int state;

int adjustSpeed;
int rightSpeed;
int leftSpeed;
int rightValue;
int leftValue;

const int echoPin = 8; // attach pin D2 Arduino to pin Echo of HC-SR04
const int trigPin = 13; // attach pin D3 Arduino to pin Trig of HC-SR04
long duration; // Variable to store time taken to the pulse to reach receiver
long distance; // Variable to store distance calculated using formula

boolean raceStarted = false;
boolean raceFinished = false;
int squareCounter;

int IR1;
int IR2;
int IR3;
int IR4;
int IR5;
int IR6;
int IR7;
int IR8;


void setup(){
  Serial.begin(9600);
  setupIRSensors();
  setupDistanceSensor();
  setupRotarySensors();
  setupMotors();
  gripper.attach(10);

  if(raceStarted == false){
    readIRSensors();
    openGripper();
    forwardDistance(40, 255);
    idle();
    if(squareDetected){
      closeGripper();
    }
    turnLeft(20, 255);
    idle();
    raceStarted = true;
    raceFinished = false;
  }
}

void loop(){
  if(raceStarted){
    readDistance();
    if(distance <= 13){
      avoidObject();
    }else{
      followLine();
    }
    if(raceFinished){
      openGripper();
      idle();
      backwards();
      idle();
    }
  }
}

//===== SETUP FUNCTIONS =====
  void setupIRSensors(){
    pinMode(A0, INPUT);
    pinMode(A1, INPUT);
    pinMode(A2, INPUT);
    pinMode(A3, INPUT);
    pinMode(A4, INPUT);
    pinMode(A5, INPUT);
    pinMode(A6, INPUT);
    pinMode(A7, INPUT);
  }

  void readIRSensors(){
    IR1 = analogRead(A0);
    IR2 = analogRead(A1);
    IR3 = analogRead(A2);
    IR4 = analogRead(A3);
    IR5 = analogRead(A4);
    IR6 = analogRead(A5);
    IR7 = analogRead(A6);
    IR8 = analogRead(A7);
  }
  
  void setupDistanceSensor(){
    pinMode(echoPin, INPUT);
    pinMode(trigPin, OUTPUT);
  }
  void setupRotarySensors(){
    pinMode(rotaryRight, INPUT);
    pinMode(rotaryLeft, INPUT);
    pinMode(leftF, OUTPUT);
    pinMode(rightF, OUTPUT);
    attachInterrupt(digitalPinToInterrupt(rotaryLeft), tickLeft, CHANGE);
    attachInterrupt(digitalPinToInterrupt(rotaryRight), tickRight, CHANGE);
  }
  void setupMotors(){
    pinMode(rightF, OUTPUT);
    pinMode(leftF, OUTPUT);
  }

  void openGripper(){
    gripper.write(openPos);
  }

  void closeGripper(){
    gripper.write(closePos);
  }

  boolean squareDetected(){
    if(IR1 > 800 && IR2 > 800 && IR3 > 800 && IR4 > 800 && IR5 > 800 && IR6 > 800 && IR7 > 800 && IR8 > 800){
      return true;
    }
  }

//===== MOTOR FUNCTIONS =====
void adjustLeft(){
  rightSpeed = map(rightValue, 0, 1023, 0, 255);
  analogWrite(leftF, 255);
  analogWrite(rightF, rightSpeed);
}

void adjustRight(){
  leftSpeed = map(leftValue, 0, 1023, 0, 255);
  analogWrite(leftF, leftSpeed);
  analogWrite(rightF, 255);
}

void forwardDistance(int ticksRemaining, int speed){
  if(ticksLeft > ticksRemaining){
    idle();
  }else{
    Serial.println(ticksLeft);
    analogWrite(leftF, speed);
    analogWrite(rightF, speed);
  }
}

void turnLeft(int ticksRemaining, int speed){
  ticksLeft = 0;
  if(ticksLeft > ticksRemaining){
    idle();
  }else{
    Serial.println(ticksLeft);
    //setting right wheel on speed so robot will turn going forward, not backwards
    analogWrite(leftF, 0);
    analogWrite(rightF, speed);
  }
}

void turnRight(int ticksRemaining, int speed){
  ticksRight = 0;
  if(ticksRight > ticksRemaining){
    idle();
  }else{
    Serial.println(ticksRight);
    //setting left wheel on speed so robot will turn going forward, not backwards
    analogWrite(leftF, speed);
    analogWrite(rightF, 0);
  }
}

void backwards(){
  analogWrite(leftB, 255);
  analogWrite(rightB, 255);
}

void forward(){
  analogWrite(leftF, 200);
  analogWrite(rightF, 200);
}

void followLine(){
  readIRSensors();
  if(IR2 > 600 || IR3 > 600 || IR4 > 600 || IR5 > 600 || IR6 > 600 || IR7 > 600){
      int leftValue = (IR5 + IR6 + IR7) / 3;
      int rightValue = (IR2 + IR3 + IR4) / 3;

      if(squareDetected){
        raceFinished = true;
      }
      //if the sensor readings on the left are bigger, go slightly left - center the robot on the line
      if(leftValue > rightValue){ //checking if robot is reading more with sensors 5 and 6
        //if robot is more to the left, the right wheel is having more speed
        adjustRight();
      }else{
        //if robot is more to the right, the left wheel will have more speed
        adjustLeft();
      }
    }
}

//===== AVOID OBJECT FUNCTION =====
void readDistance(){
  //send a 10ms pulse to HC-SR04
  digitalWrite(trigPin, LOW); //initialize trigPin on low
  delayMicroseconds(2); // wait 2ms before sending pulse
  digitalWrite(trigPin,HIGH); //send pulse to trigPin to send UltraSonic wave for detection
  delayMicroseconds(10); //keep trigPin HIGH for 10ms
  digitalWrite(trigPin,LOW); //stop sending pulse
  //calculating distance
  duration = pulseIn(echoPin, HIGH); //storing how much time it took to get the pulse back
  distance = duration * 0.034 / 2; //calculating distance based on time from pulseIn
  delay(500); // waiting before taking another measurement
}

void avoidObject(){
    boolean turningLeft = true;
    boolean turningRight = false;
    boolean changedPosition1 = false;
    boolean changedPosition2 = false;

    if(turningLeft){
      idle(); //stopping the motors so the robot will not move
      turnLeft(30, 200);// turning left to avoid object
      turningLeft = false;
      changedPosition1 = true;
    }
    if(changedPosition1){
      idle(); //stopping the motors so the robot will not move
      forwardDistance(50, 200); // going forward - on the lateral side of the object
      changedPosition1 = false;
      turningRight = true;
    }
    if(turningRight){
      idle(); //stopping the motors so the robot will not move
      turnRight(30, 200); //moving right so robot will be headed back for the line
      delay(900);
      changedPosition2 = true;
      turningRight = false;
    }
    if(changedPosition2){
      idle(); // stopping the motor so the robot will not move
      forwardDistance(50, 200); // going forward to get to the line -- object avoided
      delay(1000);
      changedPosition1 = false;
      changedPosition2 = false;
      turningRight = false;
      turningLeft = false;
    }
  }

void getDistance(){
  //send a 10ms pulse to HC-SR04
  digitalWrite(trigPin, LOW); //initialize trigPin on low
  delayMicroseconds(2); // wait 2ms before sending pulse
  digitalWrite(trigPin,HIGH); //send pulse to trigPin to send UltraSonic wave for detection
  delayMicroseconds(10); //keep trigPin HIGH for 10ms
  digitalWrite(trigPin,LOW); //stop sending pulse
  //calculating distance
  duration = pulseIn(echoPin, HIGH); //storing how much time it took to get the pulse back
  distance = duration * 0.034 / 2; //calculating distance based on time from pulseIn
  Serial.println(distance);
   // waiting before taking another measurement
}

void idle(){
  analogWrite(leftF, 0);
  analogWrite(rightF, 0);
}

void tickLeft(){
  noInterrupts();
  ticksLeft++;
  interrupts();
}

void tickRight(){
  noInterrupts();
  ticksRight++;
  interrupts();
}
