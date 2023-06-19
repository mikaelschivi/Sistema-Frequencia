#include <SPI.h>
#include <SD.h>
#include <RTClib.h>
#include <Keypad.h>
#include <LiquidCrystal.h>
#include <Adafruit_Fingerprint.h>
#include <Wire.h>

LiquidCrystal lcd(A5, A4, A3, A2, A1, A0);
RTC_DS3231 rtc;

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

void log(char logMessage[]){
  //File logFile = SD.open("log.txt", FILE_WRITE);
  int logFile = 0;
  if (logFile) {
    Serial.println( logMessage );
  } else {
    Serial.println("arquivo 'log' nao encontrado...");
  }; 
  //logFile.close();
}

void setup() {
  Serial.begin(9600);
  while (!Serial) {
  ;}
  Serial.println();
  
  lcd.begin(16, 2);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Oi Edite :)");
  delay(5000);
  
  rtc.begin();
  Wire.begin();
  if (!rtc.begin()) {
    Serial.println("RTC DS3231 não encontrado, reinicie o sistema.");    
    log("[2] Error: cannot begin rtc module.");
    while (1);
  }
  if (!SD.begin(10)) {
    Serial.println("Inacializacao do cartao SD falhou! Reinicie o sistema.");
    log("[2] Error: cannot begin SD card.");
    while (1);
  }
  if (!SD.exists("data.txt")) {
    Serial.println("Lista de presencas nao encontrada");
    Serial.println("Por favor crie dentro do cartao SD o arquivo 'data.txt'!");
    log("[1] Error: cannot find data.txt");
    while (1);
  }
  Serial.println("Lista de presenca carregada");
  if (rtc.lostPower()) {                                
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__))); //
  }

  Serial.println("Modulos carregados com sucesso!");
  Serial.print("Matricula: ");  
}

char id[8];
int index = 0;
void loop() {
  DateTime now = rtc.now();  
  char key = customKeypad.getKey();
  
  if (key) {
    if (key == '*') {
      File data = SD.open("data.txt", FILE_WRITE);
      if ( data ){
        Serial.println("\nAdicionando presenca..");
        for (int i=0; i<index; i++){
            Serial.print( id[i] );
            data.print( id[i] );
        }
        data.print("-");
        data.print(now.day(), DEC);
        data.print("/");
        data.print(now.month(), DEC);
        data.print("-");
        data.print(now.hour(), DEC);
        data.print(":");
        data.print(now.minute(), DEC);
        data.println();

        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Bem-vindo :)");
        Serial.print(" - Bem-vindo :)");
      }
      data.close();
      
      memset(id, 0, sizeof(id));  // zera a string id
      index = 0;
      
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Matricula:");
      Serial.print("\nMatricula: ");
    } else {
      id[index] = key;
      Serial.print(id[index]);
      lcd.setCursor(index, 1);
      lcd.print(id[index]);
      if (index < sizeof(id)) {
        index++;
      }
    }
  }
}
