const int gripper=10;
const int gripperOpenPulse=1600;
const int gripperClosedPulse=971;
const int servoPulseRepeat = 10; // number of pulse send to servo

void setup(){
  
}

void loop(){
  openGripper();
  delay(500);
  closeGripper();
  delay(2000);
}


void gripperServo(int pulse)
{
    for(int i = 0; i < servoPulseRepeat;i++)
    {
        digitalWrite(gripper,HIGH);
        delayMicroseconds(pulse);
        digitalWrite(gripper,LOW);
        delay(20);
    }
}
void openGripper()
{
    gripperServo(gripperOpenPulse);
}
void closeGripper()
{
  gripperServo(gripperClosedPulse);
}
