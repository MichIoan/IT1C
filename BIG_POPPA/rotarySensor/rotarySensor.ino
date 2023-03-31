const int rotaryRight = 2;
const int rotaryLeft = 3;
const int leftF = 5;
const int rightF = 11;

int ticksLeft;

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
  idle();
}

void forwardDistance(int ticksLeft, int speed){
  analogWrite(leftF, speed);
  analogWrite(rightF, speed);
  while(ticksLeft > 0){
    previousState = state;
    state = digitalRead(rotary1);
    if(previousState != state){
      ticksLeft--;
      Serial.println(ticksLeft);
    }
  }
  idle();
  Serial.println("finish");
  delay(5000);
}

void idle(){
  analogWrite(leftF, 0);
}
