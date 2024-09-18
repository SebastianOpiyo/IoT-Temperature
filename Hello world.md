# Hello world

void setup() {
    Serial.begin(9600); // Start serial communication at 9600 baud rate
}

void loop() {
    Serial.println("Hello, World!"); // Print Hello World to Serial Monitor
    delay(1000); // Wait for 1 second
}
