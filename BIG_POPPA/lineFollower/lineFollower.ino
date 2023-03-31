const int lineSensors[] = {A0, A1, A2, A3, A4, A5, A6, A7}; //IR Sensors
const int leftB = 9; //motor left - going backwards
const int leftF = 5; // motor left - going forward
const int rightB = 6; //motor right - going backwards
const int rightF = 11; //motor right - going forward

//defining variables for fine tuning on line following
int leftValue = 0;
int rightValue = 0;
int adjustSpeed = 0;

int sensorValue[8]; //for reading IR sensors values

int i;

//===============================
//************SETUP**************
//===============================
void setup(){
  Serial.begin(9600); // Initializes the serial communication
  setupIRSensors();
  setupMotors();
}

//===============================
//************LOOP***************
//===============================
void loop() {
  readSensors();
  followLine();
}

//===============================
//**********FUNCTIONS************
//===============================
void goForward() {
  analogWrite(leftF, 250);
  analogWrite(rightF, 250);
}

void followLine(){
  if(sensorValue[2] > 600 || sensorValue[3] > 600 || sensorValue[4] > 600 || sensorValue[5] > 600 || sensorValue[6] > 600 || sensorValue[7] > 600){
      int leftValue = (sensorValue[5] + sensorValue[6] + sensorValue[7]) / 3;
      int rightValue = (sensorValue[2] + sensorValue[3] + sensorValue[4]) / 3;
  
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

void adjustLeft(){
  adjustSpeed = map(rightValue, 0, 1023, 0, 255);
  analogWrite(leftF, 255);
  analogWrite(rightF, adjustSpeed);
  Serial.println(adjustSpeed);
}

void adjustRight(){
  adjustSpeed = map(leftValue, 0, 1023, 0, 255);
  analogWrite(leftF, adjustSpeed);
  analogWrite(rightF, 255);
  Serial.println(adjustSpeed);
}

bool squareDetected(){
  if(sensorValue[1] > 800 && sensorValue[2] > 800 && sensorValue[3] > 800 &&sensorValue[4] > 800 && sensorValue[5] > 800 && sensorValue[6] > 800 &&   sensorValue[7] > 800 && sensorValue[8] > 800){
    return true;
  }
}

void setupMotors(){
  //initializing motor pins
  pinMode(leftB, OUTPUT);
  pinMode(leftF, OUTPUT);
  pinMode(rightB, OUTPUT);
  pinMode(rightF, OUTPUT);
}

void setupIRSensors(){
  //initializing IR Sensors
  for(i=1; i<=8; i++){
    pinMode(lineSensors[i], INPUT);
  }
}

void readSensors(){
  for(i=1; i<=8; i++){
    sensorValue[i]=analogRead(lineSensors[i]);
  }
}
