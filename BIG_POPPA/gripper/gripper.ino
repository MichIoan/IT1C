#include <Servo.h>
Servo gripper;
int openPos = 125;
int closePos = 48;

void setup() {
  // Attach servo to pin 10
  gripper.attach(10);
}

void loop() {
  openGripper();
  delay(1500);
  closeGripper();
  delay(1500);
}

void openGripper(){
  gripper.write(openPos);
}

void closeGripper(){
  gripper.write(closePos);
}
