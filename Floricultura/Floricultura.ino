#include "DHT.h"                //biblioteca dht
#include "Wire.h"               //biblioteca I2C
#include "LiquidCrystal_I2C.h"  //LCD I2C
#include "Adafruit_Keypad.h"    //teclado matricial 4x4 ou 4x3
#include "EEPROM.h"             //Gravação da EEPROM

#include <WiFi.h>             //biblioteca wifi
#include <WiFiMulti.h>        //
#include "AdafruitIO_WiFi.h"  //Comunica com o adafruit

WiFiMulti WiFiMulti;  //da nome a rede

//Nome e senha do Wifi
#define WLAN_SSID "Raul "
#define WLAN_PASS "10203040"

//Dados do servidor Adafruit
#define AIO_SERVER "io.adafruit.com"
#define AIO_SERVERPORT 8883  // Using port 8883 for MQTTS

// Adafruit IO Account Configuration
#define IO_USERNAME "leooserafim"
#define IO_KEY "aio_NRgN875nOeEG0M1tbir66w3EhIfd"

//Se conecta com o Adafruit com os dados de usuario e senha do adafruit e da rede local
AdafruitIO_WiFi io(IO_USERNAME, IO_KEY, WLAN_SSID, WLAN_PASS);

bool ConWifi = false;

/*Comunicação dos variáveis do Sketch com o Adafruit
   AdafruitIO_Feed *xxxx = io.feed("yyy");
   xxx variável do Sketch
   yyy variável do Adafruit
*/
AdafruitIO_Feed *temperaturaMQTT = io.feed("Temperatura");
AdafruitIO_Feed *umidadeMQTT = io.feed("Umidade");
AdafruitIO_Feed *nivelMQTT = io.feed("nivel");
AdafruitIO_Feed *CMBTMQTT = io.feed("CMBT");
AdafruitIO_Feed *CMBPMQTT = io.feed("CMBP");

AdafruitIO_Feed *AutomaticoMQTT = io.feed("Automatico");
AdafruitIO_Feed *LigaCMBTMQTT = io.feed("Telhado");
AdafruitIO_Feed *LigaCMBPMQTT = io.feed("Rega");



//Pinagem e modelo do DHT
#define DHTPIN 23      // Digital pin connected to the DHT sensor
#define DHTTYPE DHT11  // DHT 11
DHT dht(DHTPIN, DHTTYPE);

float umidade;
float temperatura;


// Configura comunicação com LCD pelo I2C
LiquidCrystal_I2C lcd(0x27, 16, 2);

int TelaSubmenu = 0;
int TelaMenu = 5;  //Tela inicial

//Teclado Matricial
const byte ROWS = 4;  // rows
const byte COLS = 4;  // columns
//define the symbols on the buttons of the keypads
char keys[ROWS][COLS] = {
  { '1', '2', '3', 'A' },
  { '4', '5', '6', 'B' },
  { '7', '8', '9', 'C' },
  { '*', '0', '#', 'D' }
};
byte rowPins[ROWS] = { 13, 12, 14, 27 };  //connect to the row pinouts of the keypad
byte colPins[COLS] = { 26, 25, 33, 32 };  //connect to the column pinouts of the keypad

//initialize an instance of class NewKeypad
Adafruit_Keypad Keypad = Adafruit_Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);
keypadEvent e;
char TeclaChar;
int TeclaNum;


#define bomba_telhado 2
#define bomba_pulve 4

#define boia_inf 5
#define boia_meio 18
#define boia_sup 19

#define EEPROM_SIZE 60  //tamanho da eeprom

//Contador de tempo global
byte seg = 0;
byte seg1 = 0;
byte seg2 = 0;
byte seg3 = 0;
byte seg4 = 0;
byte min1 = 0;
byte min2 = 0;

//Variaveis de processo
int temperatura_acionamento;
int temperatura_desacionamento;
int umidade_acionamento;
int umidade_desacionamento;
bool edicao = false;
String ValorTemp;
int retornValor = 0;

bool CMBT = false;
bool CMBP = false;
bool LigaCMBT = false;
bool LigaCMBP = false;

int nivel = 0;
bool ReservOK = false;

int TpReserv = 2;
int TpDHT = 5;

bool Automatico = true;


//**************************************************
void setup() {
  Serial.begin(115200);

  dht.begin();

  lcd.init();       //Inicializa LCD
  lcd.backlight();  //Liga luz de fundo do LCD

  Keypad.begin();

  //Inicializa pinos das bombas
  pinMode(bomba_telhado, OUTPUT);
  pinMode(bomba_pulve, OUTPUT);

  digitalWrite(bomba_telhado, HIGH);
  digitalWrite(bomba_pulve, HIGH);

  pinMode(boia_inf, INPUT_PULLUP);
  pinMode(boia_meio, INPUT_PULLUP);
  pinMode(boia_sup, INPUT_PULLUP);

  //Primeira leitura de temperatura, umidade e nível
  temperatura = FuncTemperatura();
  umidade = FuncHumidade();
  nivel = Funcboias();


  //FAZ LEITURA DA EEPROM. A PRIMEIRA LEITURA DO HARDWARE, PODERÃO VIR VALORES ALEATÓRIOS OU O FUNDO DE ESCALA DO INT 16 BITS 65535
  EEPROM.begin(EEPROM_SIZE);  //Inicializa EEPROM
  EEPROM.get(0, temperatura_acionamento);
  EEPROM.get(4, temperatura_desacionamento);
  EEPROM.get(8, umidade_acionamento);
  EEPROM.get(12, umidade_desacionamento);
  EEPROM.get(16, TpReserv);
  EEPROM.get(20, TpDHT);
  EEPROM.get(24, Automatico);


  FuncInicializaLCD();
  delay(1000);

  FuncConexaowifi();  //Faz a conexão com Wifi

  FuncConexaoAdafruit();  //Faz a conexão com Adafruit

  FuncGetBotoes();    //Indica quais comandos vem do Adadfruit

  FuncTempUmidAdafruit();  //Manda os dados de Temp e Umidade para o Adafruit

  FuncNivelAdafruit();  //Manda os dados de nível para o Adafruit

  FuncLeitAutomatico();  //Manda o estado a flag automático para o Adafruit

  FuncLigaBombas();     //Manda para o Adafruit a informação dos botões das bombas quando estão no manual

  FuncLeStatusBombas(); //manda para o adafruit o status das bombas
}

//********************************************
void loop() {

  io.run();  //conecta com o Adafruit

  if (seg >= 30) {  //Leitura do Sensor de Umidade a Cada 30 segundos
    temperatura = FuncTemperatura();
    Serial.print(temperatura);
    Serial.println(" °C");
    umidade = FuncHumidade();
    Serial.print(umidade);
    Serial.println(" %");



    seg = 0;
  }

  if (seg1 >= 10) {  //Leitura do nível a cada 5 segundos
    nivel = Funcboias();
    if (nivel == 200) {
      Serial.println("Falha nos sensores de nível");
    } else {
      Serial.print(nivel);
      Serial.println(" %");
    }



    seg1 = 0;
  }

  //Leitura do teclado
  Keypad.tick();
  if (Keypad.available()) {
    FuncLeTeclado();
  } else {  //Se nenhuma tecla for pressionada, então retorna 99 e Z
    TeclaNum = 99;
    TeclaChar = 'Z';
  }


  if (TelaSubmenu == 0) {
    menu();
  } else {
    submenu();
  }




  if (Automatico == true) {
    FuncSistemaAuto();
  } else {
    FuncSistemaMan();
  }


  if (seg4 >= 30) {

    FuncRecebeInfo();

    FuncLigaBombas();

    FuncLeStatusBombas();

    FuncNivelAdafruit();

    FuncTempUmidAdafruit();

    Serial.println(io.statusText());

    seg4 = 0;
  }

  temporizador();

  FuncCMB();

  FuncError();

}  //FIM DO LOOP


//**************************************************************
void temporizador() {
  static unsigned long tempoins;  //Tempo instantâneo

  if ((millis() - tempoins) >= 1000) {  //CONTADOR DE SEGUNDOS
    seg++;
    seg1++;
    seg2++;
    seg3++;
    seg4++;
    tempoins = millis();
  }

  if (seg >= 59) {  //CONTADOR DE MINUTOS
    min1++;
    min2++;
    seg = 0;
  }

  if (millis() < tempoins) {
    tempoins = millis();
  }

}  //FIM DO temporizador

//**************************************************************
void EEPROMW(int PosMem, int NomeVariavel) {  //Grava variável na EEPROM e mostra na tela e na serial

  EEPROM.put(PosMem, NomeVariavel);
  EEPROM.commit();
  lcd.setCursor(0, 1);
  lcd.print(F("GRAVANDO VALOR.."));
  Serial.println("Gravando EERPOM");
  Serial.print("PosMem");
  Serial.println(PosMem);
  Serial.print("NomeVariavel");
  Serial.println(NomeVariavel);
  delay(500);



}  //Fim do void grava eeprom

//**************************************************************
void FuncError() {
  if (temperatura_acionamento <= temperatura_desacionamento) {
    temperatura_acionamento = 60;
    temperatura_desacionamento = (temperatura_acionamento - 10);
  }

  if (temperatura_acionamento == 0 || temperatura_desacionamento == 0) {
    temperatura_acionamento = 60;
    temperatura_desacionamento = 50;
  }

  if (temperatura_acionamento > 99) {
    temperatura_acionamento = 99;
  }

  if (umidade_acionamento >= umidade_desacionamento) {
    umidade_desacionamento = 80;
    umidade_acionamento = (umidade_desacionamento - 20);
  }

  if (umidade_acionamento == 0 || umidade_desacionamento == 0) {
    umidade_acionamento = 60;
    umidade_desacionamento = 80;
  }

  if (umidade_desacionamento > 99) {
    umidade_desacionamento = 99;
  }

  if (seg3 >= 300) {  //Caso nenhum botão seja pressionado por 300 segundos a tela volta para o menu principal
    seg3 = 0;         //Reseta contagem de tempo

    //Volta para o menu principal
    TelaMenu = 1;
    TelaSubmenu = 0;

    //Limpe Tela LCD
    lcd.clear();
  }
}