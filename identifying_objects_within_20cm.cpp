int trigpin=3;
int echopin=5;
int ledpin=2;

long duration;
int distance;
void setup() {
  // put your setup code here, to run once:
  pinMode(trigpin,OUTPUT);
  pinMode(echopin,INPUT);
  pinMode(ledpin,OUTPUT);
  Serial.begin(9600);
  

}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(trigpin,LOW);
  delayMicroseconds(2);
  digitalWrite(trigpin,HIGH);
  delayMicroseconds(10);
  digitalWrite(trigpin,LOW);

  duration = pulseIn(echopin,HIGH);

  distance = duration * 0.034/2;

  Serial.print(" Distance:");
  Serial.print(distance);
  Serial.print(" cm ");
  

  if (distance <= 20){
    digitalWrite(ledpin,HIGH);
  }else{
    digitalWrite(ledpin,LOW);
  }
  delay(100);
  Serial.print("\n object Found");
}