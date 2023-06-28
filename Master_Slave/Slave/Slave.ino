#include <Wire.h>


void receiveEvent(int numBytes) {
  while (Wire.available()) {
    char key = Wire.read();
    
    // Process the received key value
    switch (key) {
      case '1':
        Serial.println("True");
        //delay(2000);
        tone(5,800);
        delay(1000);
        noTone(5);
        delay(1000000);
        break;
      case '2':
        Serial.println("Triste");
        break;
      case '3':
        // Do something for key '3'
        break;
      // Add more cases for other keys as needed
      
      default:
        // Invalid key or unrecognized command
        break;
    }
  }
}

void setup() {
  Wire.begin(9); // Slave Arduino address
  Wire.onReceive(receiveEvent);
  Serial.begin(9600);
}

void loop() {
  // Slave Arduino loop
}