#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <Keypad.h>
#include <RTClib.h>

char t[32];

// Initializer for Matrix keypad
const byte ROWS = 4; 
const byte COLS = 4; 

char hexaKeys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};
byte rowPins[ROWS] = {9, 8, 7, 6}; 
byte colPins[COLS] = {5, 4, 3, 2};
Keypad customKeypad = Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);

// Initializer for lcd i2c display
LiquidCrystal_I2C lcd(0x27, 20, 4);  

// RTC initializer
RTC_DS1307 RTC;

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

//struct student presenceList = [5];
    
void setup(){
  Serial.begin(9600);

  Wire.begin();
  RTC.begin();
  
  // Run this when in need to sync/update time
  //RTC.adjust(DateTime(F(__DATE__),F(__TIME__)));
  
  lcd.backlight();    // turn on lcd blacklight
  lcd.init();         // turn on lcd
  lcd.clear();        // clear screen for any leftover
  
  Serial.print('\n');
  
  pinMode(12, OUTPUT);    // Setup for Buzzer - pin 12
  
  // Initial Setup
  lcd.setCursor(0, 0);
  lcd.print("Insira sua matricula");
  lcd.setCursor(15, 1);
  lcd.print("SETUP");
  lcd.setCursor(0, 2);
  lcd.blink_on();
}

char id[8];
int index = 0;

void loop(){
  // Setup
  student aluno;                      // aluno init
  char key = customKeypad.getKey();   // Keypad init
  DateTime now = RTC.now();           // fetch time
  
  // sprintf(t, "%02d:%02d:%02d %02d/%02d/%02d", now.hour(), now.minute(), now.second(), now.day(), now.month(), now.year());  
  // Serial.print(F("Presenca time: "));
  // Serial.println(t);
  // delay(1000);

  // Key get
  if (key){
    // Submit routine
    if( key == '*'){
      lcd.clear();                // Clear lcd
      lcd.setCursor(0, 0);        // Set cursor at (0,0) - Top left
      lcd.print("Bem-vindo :)");  // Print to lcd
      tone(12, 2500, 125);        // Beep to audible response

      for(int i=0; i<sizeof(id); i++){
        aluno.id[i] = id[i];
      }

      memset(id, 0, sizeof(id));  // Set all elements of id[] to 0 which = NULL
      index = 0;                  // Set index=0 to remap counter

      lcd.clear();                
      lcd.setCursor(0, 0);      

      aluno.month = now.month();
      aluno.day = now.day();
      aluno.hour = now.hour();
      aluno.min = now.minute();
      aluno.attendances = aluno.attendances + 1;
      
      lcd.print("Aguarde..");
      delay(2200);

      Serial.print('\n');
                  
      lcd.clear();
      lcd.setCursor(0, 0);                // Set cursor at (0,0) - Top left
      lcd.print("Insira sua matricula");  // Print to lcd
      
      lcd.setCursor(0, 2);
      lcd.blink_on();                     // Cursor blink
    }

    // Typing routine
    else{
          id[index] = key;
          
          lcd.setCursor(index, 2);
          lcd.print(id[index]);
          Serial.print(id[index]);

          if( index <= sizeof(id) ){
            index++;
          }
    }
    
  }
}