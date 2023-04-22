#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <Keypad.h>


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

typedef struct Dates{
  int day;
  int month;
  int hour;
  int min;
  
} Dates;

typedef struct Aluno{
  char id[8];
  int attendances = 0;
      
  // fingerprint id

} Aluno;

void setup(){
  lcd.backlight();    // turn on lcd blacklight
  lcd.init();         // turn on lcd
  lcd.clear();        // clear screen for any leftover

  Serial.begin(9600);
  Serial.print('\n');
  
  pinMode(12, OUTPUT);    // Setup for Buzzer - pin 12
  
  lcd.setCursor(0, 0);                // Initial Setup
  lcd.print("Insira sua matricula");  //
  lcd.setCursor(15, 1);
  lcd.print("SETUP");
  lcd.setCursor(0, 2);
  lcd.blink_on();
}

char id[8];
int index = 0;
void loop(){
  char key = customKeypad.getKey();
  
  if (key){
    // Submit routine
    if( key == '*'){
      lcd.clear();                // Clear lcd
      lcd.setCursor(0, 0);        // Set cursor at (0,0) - Top left
      lcd.print("Bem-vindo :)");  // Print to lcd
      tone(12, 2500, 125);        // Beep to audible response

      delay(1000);      

      memset(id, 0, sizeof(id));  // Set all elements of id[] to 0 which = NULL
      index = 0;                  // Set index=0 to remap counter

      lcd.clear();                
      lcd.setCursor(0, 0);      
      lcd.print("Aguarde..");
      delay(3000);

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

          if( index<= sizeof(id) ){
            index++;
          }

    }
  }
}