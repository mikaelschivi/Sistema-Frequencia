#include <Wire.h>
#include <RTClib.h>

// // RTC initializer
//RTC_DS1307 RTC;

// logic for handling information
typedef struct student{
  char id[8];
  int attendances = 0;
  // fingerprint id

  int day;
  int month;
  int hour;
  int min;
} student;

student aluno;                  // aluno init
char id = ' ';


void setup() {
  Wire.begin(9); // Slave Arduino address
  Serial.begin(9600);
  Wire.onReceive(receiveEvent);
}

void loop() {
  // Slave Arduino loop
  
}

int i = 0;

void receiveEvent(int byteCount) {
  while (Wire.available()) {
    aluno.id[i] = Wire.read(); 
    i++;
  }
  Serial.print("Slave - id ");
  Serial.println(aluno.id);
  // aluno.month = now.month();
  // Serial.print(now.month());
  // aluno.day = now.day();
  // Serial.print(now.day());
  // aluno.hour = now.hour();
  // aluno.min = now.minute();
  // aluno.attendances = aluno.attendances + 1;
  
}