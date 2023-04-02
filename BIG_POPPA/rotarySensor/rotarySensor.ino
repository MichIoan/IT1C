const int rotaryRight = 2;
const int rotaryLeft = 3;
const int leftF = 5;
const int rightF = 11;

int ticksLeft;
int ticksRight;

int previousState;
int state;

void setup(){
  Serial.begin(9600);
  pinMode(rotaryRight, INPUT);
  pinMode(rotaryLeft, INPUT);
  pinMode(leftF, OUTPUT);
  pinMode(rightF, OUTPUT);
  attachInterrupt(digitalPinToInterrupt(rotaryLeft), tickLeft, CHANGE);
  attachInterrupt(digitalPinToInterrupt(rotaryRight), tickRight, CHANGE);
}

void loop(){
  forwardDistance(20, 250);
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

void idle(){
  analogWrite(leftF, 0);
  analogWrite(rightF, 0);
}
