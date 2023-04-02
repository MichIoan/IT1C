const int leftF = 5; // motor left - going forward
const int rightF = 11; //motor right - going forward

const int rotary1 = 2;
const int rotary2 = 3;

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

void setup(){
  Serial.begin(9600);
  pinMode(rightF, OUTPUT);
  pinMode(leftF, OUTPUT);
  pinMode(rotary1, INPUT);
  pinMode(rotary2, INPUT);
  pinMode(echoPin, INPUT);
  pinMode(trigPin, OUTPUT);
}

void loop(){
  getDistance();
  Serial.println(distance);
  if(distance < 12){
    avoidObject();
  }else{
    idle();
  }
}

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

void forward(){
  analogWrite(leftF, 200);
  analogWrite(rightF, 200);
}

void turnRight(){
  analogWrite(leftF, 0);
  analogWrite(rightF, 230);
}

void turnLeft(){
  analogWrite(leftF, 230);
  analogWrite(rightF, 0);
}

void avoidObject(){
    boolean turningLeft = true;
    boolean turningRight = false;
    boolean changedPosition1 = false;
    boolean changedPosition2 = false;

    if(turningLeft){
      idle(); //stopping the motors so the robot will not move
      turnLeft();// turning left to avoid object
      delay(600);
      turningLeft = false;
      changedPosition1 = true;
    }
    if(changedPosition1){
      idle(); //stopping the motors so the robot will not move
      forward(); // going forward - on the lateral side of the object
      delay(900);
      changedPosition1 = false;
      turningRight = true;
    }
    if(turningRight){
      idle(); //stopping the motors so the robot will not move
      turnRight(); //moving right so robot will be headed back for the line
      delay(900);
      changedPosition2 = true;
      turningRight = false;
    }
    if(changedPosition2){
      idle(); // stopping the motor so the robot will not move
      forward(); // going forward to get to the line -- object avoided
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
