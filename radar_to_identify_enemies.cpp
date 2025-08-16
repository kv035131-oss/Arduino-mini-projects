#include <Servo.h>

const int trigPin = 9;
const int echoPin = 10;
const int servoPin = 6;

Servo radarServo;

void setup() {
  Serial.begin(9600);
  radarServo.attach(servoPin);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
}

void loop() {
  for (int angle = 0; angle <= 180; angle += 2) {
    radarServo.write(angle);
    delay(100);
    long duration = getDistance();
    int distance = duration * 0.034 / 2;

    Serial.print("Angle: ");
    Serial.print(angle);
    Serial.print(" | Distance: ");
    Serial.println(distance);
  }

  for (int angle = 180; angle >= 0; angle -= 2) {
    radarServo.write(angle);
    delay(100);
    long duration = getDistance();
    int distance = duration * 0.034 / 2;

    Serial.print("Angle: ");
    Serial.print(angle);
    Serial.print(" | Distance: ");
    Serial.println(distance);
  }
}

long getDistance() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  return pulseIn(echoPin, HIGH);
}