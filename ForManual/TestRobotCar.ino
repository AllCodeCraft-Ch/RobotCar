const int enbPin = 5;
const int in4Pin = 6;
const int in3Pin = 7;
const int in2Pin = 8;
const int in1Pin = 9;
const int enaPin = 10;

const int joyXPin = A0;
const int joyYPin = A1;

const int trigPin = 13;
const int echoPin = 12;

const int sensorLeftPin = 2;
const int sensorRightPin = 3;

volatile long leftMotorCount = 0;
volatile long rightMotorCount = 0;

unsigned long prevTime = 0;
float leftMotorSpeed = 0.0;
float rightMotorSpeed = 0.0;

void setup() {
  pinMode(enbPin, OUTPUT);
  pinMode(in4Pin, OUTPUT);
  pinMode(in3Pin, OUTPUT);
  pinMode(in2Pin, OUTPUT);
  pinMode(in1Pin, OUTPUT);
  pinMode(enaPin, OUTPUT);

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  pinMode(sensorLeftPin, INPUT);
  pinMode(sensorRightPin, INPUT);

  attachInterrupt(digitalPinToInterrupt(sensorLeftPin), countLeftMotor, RISING);
  attachInterrupt(digitalPinToInterrupt(sensorRightPin), countRightMotor, RISING);

  Serial.begin(9600);
}

void loop() {
  long duration;
  int distance;

  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH);

  distance = duration * 0.034 / 2;

  int joyXValue = analogRead(joyXPin);
  int joyYValue = analogRead(joyYPin);

  int motorSpeedA = map(joyYValue, 0, 1023, -255, 255); 
  int motorSpeedB = map(joyXValue, 0, 1023, -255, 255); 

  if (motorSpeedA > 0) {
    digitalWrite(in1Pin, HIGH);
    digitalWrite(in2Pin, LOW);
  } else if (motorSpeedA < 0) {
    digitalWrite(in1Pin, LOW);
    digitalWrite(in2Pin, HIGH);
    motorSpeedA = -motorSpeedA;
  } else {
    digitalWrite(in1Pin, LOW);
    digitalWrite(in2Pin, LOW);
  }
  analogWrite(enaPin, motorSpeedA);

  if (motorSpeedB > 0) {
    digitalWrite(in3Pin, HIGH);
    digitalWrite(in4Pin, LOW);
  } else if (motorSpeedB < 0) {
    digitalWrite(in3Pin, LOW);
    digitalWrite(in4Pin, HIGH);
    motorSpeedB = -motorSpeedB;
  } else {
    digitalWrite(in3Pin, LOW);
    digitalWrite(in4Pin, LOW);
  }
  analogWrite(enbPin, motorSpeedB);

  unsigned long currentTime = millis();
  if (currentTime - prevTime >= 1000) {
    noInterrupts();
    leftMotorSpeed = (leftMotorCount / 20.0) * 60.0;
    rightMotorSpeed = (rightMotorCount / 20.0) * 60.0;
    leftMotorCount = 0;
    rightMotorCount = 0;
    prevTime = currentTime;
    interrupts();

    Serial.print("Left Motor Speed (RPM): ");
    Serial.print(leftMotorSpeed);
    Serial.print(" | Right Motor Speed (RPM): ");
    Serial.println(rightMotorSpeed);  
    Serial.print("พบวัตุในระยะ: ");
    Serial.print(distance);
    Serial.println(" เซ็นติเมตร");
  }

  delay(5);
}

void countLeftMotor() {
  leftMotorCount++;
}

void countRightMotor() {
  rightMotorCount++;
}
