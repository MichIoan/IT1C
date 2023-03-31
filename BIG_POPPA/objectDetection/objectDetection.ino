const int echoPin = 8; // attach pin D2 Arduino to pin Echo of HC-SR04
const int trigPin = 13; // attach pin D3 Arduino to pin Trig of HC-SR04
long duration; // Variable to store time taken to the pulse to reach receiver
long distance; // Variable to store distance calculated using formula
const int leftF = 5; // motor left - going forward
const int rightF = 11; //motor right - going forward


void setup(){
  Serial.begin(9600);
  pinMode(echoPin, INPUT);
  pinMode(trigPin, OUTPUT);
}

void loop(){
  getDistance();
  Serial.println(distance);
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
  delay(500); // waiting before taking another measurement
}
