// Define the pins
const int potPin = A5; // Potentiometer is connected to analog pin A0
const int ledPin = 7;  // LED is connected to a PWM pin (e.g., pin 9)

void setup() {
  // Set the LED pin as an output
  pinMode(ledPin, OUTPUT);
  // No need to set the potentiometer pin as input, it's the default for analog pins
}

void loop() {
  // Read the analog value from the potentiometer (0-1023)
  int potValue = analogRead(potPin);

  // Map the potentiometer value (0-1023) to the PWM range (0-255)
  int pwmValue = map(potValue, 0, 1023, 0, 255);

  // Write the PWM value to the LED pin
  analogWrite(ledPin, pwmValue);

  // Optional: add a small delay to avoid flickering
  delay(10);
}