#include <WiFiS3.h>

const char* ssid = "Arduino_R4_AP";
const char* password = "12345678";

WiFiServer server(8080);

const int enbPin = 5;
const int in4Pin = 6;
const int in3Pin = 7;
const int in2Pin = 8;
const int in1Pin = 9;
const int enaPin = 10;

const int trigPin = 13;
const int echoPin = 12;

void setup() {
  Serial.begin(115200);

  // ตั้งค่าเป็น AP
  if (WiFi.beginAP(ssid, password) != WL_AP_LISTENING) {
    Serial.println("Failed to start AP!");
    while (1);
  }
  
  IPAddress IP = WiFi.localIP();
  Serial.print("AP IP address: ");
  Serial.println(IP);

  server.begin(); // เริ่มต้น server

  pinMode(enbPin, OUTPUT);
  pinMode(in4Pin, OUTPUT);
  pinMode(in3Pin, OUTPUT);
  pinMode(in2Pin, OUTPUT);
  pinMode(in1Pin, OUTPUT);
  pinMode(enaPin, OUTPUT);

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
}

void loop() {
  WiFiClient client = server.available(); // รอการเชื่อมต่อจาก Client

  if (client) {
    String message = client.readStringUntil('\n'); // อ่านข้อมูลที่รับมา
    Serial.print("Received: ");
    Serial.println(message);

    int commaIndex = message.indexOf(',');
    int joyXValue = message.substring(0, commaIndex).toInt();
    int joyYValue = message.substring(commaIndex + 1).toInt();

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

    // อ่านค่าจากเซ็นเซอร์ระยะ
    long duration;
    int distance;
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);

    duration = pulseIn(echoPin, HIGH);
    distance = duration * 0.034 / 2;

    Serial.print("Distance: ");
    Serial.print(distance);
    Serial.println(" cm");
  }
}
