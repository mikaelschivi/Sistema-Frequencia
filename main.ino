#include <SPI.h>
#include <SD.h>
#include <RTClib.h>
#include <Keypad.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>

RTC_DS3231 rtc;
File testFile;
LiquidCrystal_I2C lcd(0x27, 16, 2);

#define LOG "log.txt"
#define ATD "lista.txt"

// salva message[] to fileName
// utilidade -> catalogar erros no arquivo log
//           -> salvar item na lista de presenca
void textToFile(char message[], char fileName){
  File file = SD.open(fileName, FILE_WRITE);
  if (file) {
    Serial.print(message);
    Serial.print("\adicionado a");
    Serial.print(fileName);
    Serial.print("\n");
    
    file.println(message);
  } else {
    Serial.print("\n\narquivo'");
    Serial.print(fileName);
    Serial.print("nao encontrado");
    Serial.print("\n");
  } file.close();
}

// keypad
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

void setup() {
  /******** Inicializao dos componentes ******* */
  Serial.begin(9600);
  while (!Serial) {
    ;    
  }    
  rtc.begin();
  Wire.begin();
  lcd.init();
  lcd.backlight();

  if(!rtc.begin()) {
    Serial.println("RTC DS3231 não encontrado, reinicie o sistema.");
    textToFile(" rtc.begin() - falhou", LOG);
    while (1);
  }
  if (!SD.begin(10)) {
    Serial.println("Inacializacao do cartao SD falhou! Reinicie o sistema.");
    while (1);
  }
  Serial.println("Modulos carregados com sucesso!");

  if (!SD.exists("data.txt")) {
    Serial.println("Lista de presencas nao encontrada");
    Serial.println("Por favor crie dentro do cartao SD o arquivo 'data.txt'!");
    textToFile("Falha ao carregar o cartao SD -> !SD.exists() - falhou", LOG);
  } 
  Serial.println("Lista de presenca carregada");

  // Caso a pilha tenha perdido energia sincroniza data/hora
  if (rtc.lostPower()) {                                
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__))); //
    Serial.println("RTC DS3231 sincronizacao de data hora..");
    textToFile("RTC - Sincronizou", LOG);
  }
  delay(100); 
}

void loop() {
  // put your main code here, to run repeatedly:
}
