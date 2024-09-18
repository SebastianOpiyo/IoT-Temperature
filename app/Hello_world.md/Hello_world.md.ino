#include <Servo.h>
#include <SoftwareSerial.h> // Include SoftwareSerial for Bluetooth communication

// Define pins and variables
float temp;
int tempPin = A0; // Pin connected to LM35
int fanPin = 9; // Pin connected to fan or servo motor
int ledPin = 13; // Pin for LED
int tempMin = 25; // Minimum temperature threshold
int tempMax = 70; // Maximum temperature threshold
Servo myServo; // Create servo object if using a servo
SoftwareSerial BTSerial(18, 19); // RX, TX for HC-06

void setup() {
    Serial.begin(9600); // Start Serial Monitor
    BTSerial.begin(9600); // Start Bluetooth Serial
    pinMode(fanPin, OUTPUT);
    pinMode(ledPin, OUTPUT); // Set LED pin as output
    myServo.attach(10); // Attach servo if using a servo motor
}

void loop() {
    // Read temperature from LM35 sensor
    int reading = analogRead(tempPin);
    temp = (reading * 5000.0) / 1024.0; // Convert reading to millivolts
    temp /= 10; // Convert millivolts to Celsius

    Serial.print("Temperature: ");
    Serial.print(temp);
    Serial.println(" °C");

    // Control fan or servo based on temperature
    if (temp < tempMin) {
        digitalWrite(fanPin, LOW); // Turn off fan
        myServo.write(0); // Set servo position if applicable
    } else if (temp >= tempMin && temp <= tempMax) {
        int fanSpeed = map(temp, tempMin, tempMax, 32, 255);
        analogWrite(fanPin, fanSpeed); // Control fan speed
        myServo.write(map(temp, tempMin, tempMax, 0, 180)); // Adjust servo position if applicable
    } else {
        digitalWrite(fanPin, HIGH); // Turn on fan at full speed if above max temperature
        myServo.write(180); // Set servo position if applicable
    }

    // Bluetooth communication handling
    if (BTSerial.available()) { // Check if data is available from Bluetooth
        char command = BTSerial.read(); // Read command from Bluetooth
        Serial.println(command); // Print command to Serial Monitor

        if (command == '1') {
            digitalWrite(ledPin, HIGH); // Turn on LED
            BTSerial.println("LED ON");
        } else if (command == '0') {
            digitalWrite(ledPin, LOW); // Turn off LED
            BTSerial.println("LED OFF");
        }
    }

    delay(1000); // Delay for stability and to avoid rapid readings
}
