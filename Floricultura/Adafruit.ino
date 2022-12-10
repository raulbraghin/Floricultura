void FuncConexaowifi() {

  //SE CONECTANDO COM A REDE LOCAL
  WiFiMulti.addAP(WLAN_SSID, WLAN_PASS);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("CONECTANDO WIFI ");

  //Fica tentadno se comunicar com a rede local
  Serial.print("Conectando com rede WIFI");
  while (WiFiMulti.run() != WL_CONNECTED && millis() < 60000) {  //Aguarda conexão com wifi, se não se conectar em 60 segundos ele sai
    Serial.print(".");
    delay(500);
  }
  Serial.println();

  if (WiFiMulti.run() == WL_CONNECTED) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("CONECTADO A REDE");
    lcd.setCursor(0, 1);
    lcd.print(WLAN_SSID);
    ConWifi = true;
  } else {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("     FALHA      ");
    lcd.setCursor(0, 1);
    lcd.print("   REDE WIFI    ");
    ConWifi = false;
  }

  delay(1000);
}


void FuncConexaoAdafruit() {


  Serial.println(F("Conectando ao adafruit io"));
  io.connect();

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("   CONECTANDO   ");
  lcd.setCursor(0, 1);
  lcd.print("  ADAFRUIT IO   ");

  if (ConWifi == true) {

    //Tenta se conectar com o servidor Adafruit - pode levar até 180 segundos a depender do plano e da internet
    while (io.mqttStatus() < AIO_CONNECTED && millis() < 240000) {
      Serial.print(".");
      delay(1000);
    }



    if (io.mqttStatus() >= AIO_CONNECTED) {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("  CONECTADO AO  ");
      lcd.setCursor(0, 1);
      lcd.print("  ADAFRUIT IO   ");


      Serial.print("AIO_CONNECTED");
      Serial.println(AIO_CONNECTED);
      Serial.println(F("Conectado ao adafruit io"));
      Serial.println(io.statusText());

    } else {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("     FALHA      ");
      lcd.setCursor(0, 1);
      lcd.print("  ADAFRUIT IO   ");

      Serial.println(F("Não Conectado ao adafruit io"));
      Serial.println(io.statusText());
    }
  }

  delay(1000);
}

void FuncGetBotoes() {

  AutomaticoMQTT->get();
  LigaCMBTMQTT->get();
  LigaCMBPMQTT->get();
}

void FuncRecebeInfo() {

  AutomaticoMQTT->onMessage(le_auto);
  LigaCMBTMQTT->onMessage(le_LCMBT);
  LigaCMBPMQTT->onMessage(le_LCMBP);
}

void le_auto(AdafruitIO_Data *data) {

  Automatico = data->toPinLevel();
}

void le_LCMBT(AdafruitIO_Data *data) {

  LigaCMBT = data->toPinLevel();
}

void le_LCMBP(AdafruitIO_Data *data) {

  LigaCMBP = data->toPinLevel();
}

void FuncTempUmidAdafruit() {

  temperaturaMQTT->save(temperatura);
  umidadeMQTT->save(umidade);

  delay(500);
}

void FuncNivelAdafruit() {

  nivelMQTT->save(nivel);

  delay(500);
}

void FuncLeitAutomatico() {

  AutomaticoMQTT->save(Automatico);

  delay(500);
}

void FuncLigaBombas() {

  LigaCMBTMQTT->save(LigaCMBT);
  LigaCMBPMQTT->save(LigaCMBP);

  delay(500);
}

void FuncLeStatusBombas() {

  CMBTMQTT->save(CMBT);
  CMBPMQTT->save(CMBP);

  delay(500);
}