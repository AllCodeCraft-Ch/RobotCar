#include <WiFiS3.h>

const char* ssid = "Arduino_R4_AP";
const char* password = "12345678";
const char* host = "192.168.4.1";  // AP IP ของ Receiver
const int serverPort = 8080;

WiFiClient client;

const int joyXPin = A0;
const int joyYPin = A1;

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("Connected to AP");
}

void loop() {
  if (client.connect(host, serverPort)) {
    int joyXValue = analogRead(joyXPin);
    int joyYValue = analogRead(joyYPin);

    String message = String(joyXValue) + "," + String(joyYValue);
    client.println(message);  // ส่งข้อมูลไปยัง Server
    client.stop();  // ปิดการเชื่อมต่อหลังจากส่งข้อมูลเสร็จ
  } else {
    Serial.println("Connection to server failed");
  }

  delay(50); // ส่งข้อมูลทุก 100ms
}
