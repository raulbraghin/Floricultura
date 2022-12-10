void FuncInicializaLCD() {

  //Mostra a versão do software
  lcd.setCursor(0, 0);
  lcd.print("  FLORICULTURA ");
  lcd.setCursor(5, 1);
  lcd.print("V. 1.00");

  delay(1000);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("   SISTEMA EM   ");
  lcd.setCursor(0, 1);

  if (Automatico == true) {
    lcd.print("   AUTOMATICO   ");
  } else {
    lcd.print("     MANUAL     ");
  }
}

void menu() {


  lcd.setCursor(0, 0);

  switch (TelaMenu) {

    case 1:
      lcd.print(F("<    SISTEMA   >"));
      lcd.setCursor(0, 1);
      lcd.print(F("                "));

      if (TeclaChar == 'D') {
        TelaSubmenu = 10;
        Serial.print("TelaSubmenu ");
        Serial.println(TelaSubmenu);
        delay(200);
      }
      break;

    case 2:
      lcd.print(F("< FUNC MANUAIS >"));
      lcd.setCursor(0, 1);
      lcd.print(F("                "));

      if (TeclaChar == 'D') {
        TelaSubmenu = 20;
        delay(200);
      }
      break;

    case 3:
      lcd.print(F("<  FUNC AUTO   >"));
      lcd.setCursor(0, 1);
      lcd.print(F("                "));

      if (TeclaChar == 'D') {
        TelaSubmenu = 30;
        delay(200);
      }
      break;


    case 4:
      lcd.print(F("<    STATUS    >"));
      lcd.setCursor(0, 1);
      lcd.print(F("                "));

      if (TeclaChar == 'D') {
        TelaSubmenu = 40;
        delay(200);
      }
      break;


    case 5:
      lcd.print("TEMP    ");
      lcd.print(temperatura);  //TEMPERATURA DA ÁGUA
      lcd.print(" oC");
      lcd.setCursor(0, 1);
      lcd.print("UMIDADE ");
      lcd.print(umidade);  //TEMPERATURA DA ÁGUA
      lcd.print("  %");

      break;

  }  //FIM DO SWITCH CASE

  //Torna o menu cíclico
  if (TelaMenu > 5) { TelaMenu = 1; }
  if (TelaMenu < 1) { TelaMenu = 5; }

}  // FIM DO VOID MENU

void submenu() {


  lcd.setCursor(0, 1);

  switch (TelaSubmenu) {

    case 10:

      if (Automatico == true) {  //Se estuver no automatico
        lcd.print(F("   AUTOMATICO   "));
      } else {
        lcd.print(F("     MANUAL     "));
      }


      if (TeclaChar == 'A' && Automatico == false) {
        Automatico = true;
        
        return;
      }

      if (TeclaChar == 'B' && Automatico == true) {
        Automatico = false;
        
        return;
      }

      if (TeclaChar == 'D') {
        EEPROMW(24, Automatico);
        
        FuncLeitAutomatico();

      }
      break;

    case 11:
      lcd.print(F("     VOLTAR     "));

      if (TeclaChar == 'D') {
        TelaSubmenu = 0;
        delay(200);
        lcd.clear();
      }
      break;



    case 20:
      lcd.print(F("LIGA CMB TELHADO"));

      if (TeclaChar == 'D') {
        LigaCMBT = true;
        Serial.println("Liga CMBT");
        delay(200);
      }
      break;

    case 21:
      lcd.print(F("DESL CMB TELHADO"));

      if (TeclaChar == 'D') {
        LigaCMBT = false;
        Serial.println("Desliga CMBT");
        delay(200);
      }
      break;

    case 22:
      lcd.print(F("LIGA CMB PULVERI"));

      if (TeclaChar == 'D') {
        LigaCMBP = true;
        Serial.println("Liga CMBP");
        delay(200);
      }
      break;

    case 23:
      lcd.print(F("DESL CMB PULVERI"));

      if (TeclaChar == 'D') {
        LigaCMBP = false;
        Serial.println("Desliga CMBP");
        delay(200);
      }
      break;

    case 24:
      lcd.print(F("     VOLTAR     "));

      if (TeclaChar == 'D') {
        TelaSubmenu = 0;
        delay(200);
        lcd.clear();
      }
      break;





    case 30:

      lcd.setCursor(0, 0);
      lcd.print(F("TEMP ACIONAMENTO"));
      lcd.setCursor(0, 1);
      lcd.print(F("     "));
      lcd.print(temperatura_acionamento);
      lcd.print(F(" oC      "));

      if (TeclaChar == 'C') {
        edicao = true;
      }

      if (edicao == true && TeclaNum <= 9 || TeclaChar == 'D') {
        Serial.println("edicao == true");
        temperatura_acionamento = FuncEdicao(TeclaNum);

        if (TeclaChar == 'D' && temperatura_acionamento != 0 && temperatura_acionamento < 99) { EEPROMW(0, temperatura_acionamento); }
      }

      if (TeclaChar == 'A' || TeclaChar == 'B' || TeclaChar == '*' || TeclaChar == '#') {
        FuncCancelEdit();
      }

      break;


    case 31:

      lcd.setCursor(0, 0);
      lcd.print(F("TEMP DESACIONAM "));
      lcd.setCursor(0, 1);
      lcd.print(F("     "));
      lcd.print(temperatura_desacionamento);
      lcd.print(F(" oC      "));

      if (TeclaChar == 'C') {
        edicao = true;
      }

      if (edicao == true && TeclaNum <= 9 || TeclaChar == 'D') {
        Serial.println("edicao == true");
        temperatura_desacionamento = FuncEdicao(TeclaNum);

        if (TeclaChar == 'D' && temperatura_desacionamento != 0 && temperatura_desacionamento < 99) { EEPROMW(4, temperatura_desacionamento); }
      }

      if (TeclaChar == 'A' || TeclaChar == 'B' || TeclaChar == '*' || TeclaChar == '#') {
        FuncCancelEdit();
      }

      break;

    case 32:

      lcd.setCursor(0, 0);
      lcd.print(F("UMID ACIONAMENTO"));
      lcd.setCursor(0, 1);
      lcd.print(F("     "));
      lcd.print(umidade_acionamento);
      lcd.print(F(" %       "));

      if (TeclaChar == 'C') {
        edicao = true;
      }

      if (edicao == true && TeclaNum <= 9 || TeclaChar == 'D') {
        Serial.println("edicao == true");
        umidade_acionamento = FuncEdicao(TeclaNum);

        if (TeclaChar == 'D' && umidade_acionamento != 0 && umidade_acionamento < 99) { EEPROMW(8, umidade_acionamento); }
      }

      if (TeclaChar == 'A' || TeclaChar == 'B' || TeclaChar == '*' || TeclaChar == '#') {
        FuncCancelEdit();
      }

      break;

    case 33:

      lcd.setCursor(0, 0);
      lcd.print(F("UMID DESACIONAM "));
      lcd.setCursor(0, 1);
      lcd.print(F("     "));
      lcd.print(umidade_desacionamento);
      lcd.print(F(" %       "));

      if (TeclaChar == 'C') {
        edicao = true;
      }

      if (edicao == true && TeclaNum <= 9 || TeclaChar == 'D') {
        Serial.println("edicao == true");
        umidade_desacionamento = FuncEdicao(TeclaNum);

        if (TeclaChar == 'D' && umidade_desacionamento != 0 && umidade_desacionamento < 99) { EEPROMW(12, umidade_desacionamento); }
      }

      if (TeclaChar == 'A' || TeclaChar == 'B' || TeclaChar == '*' || TeclaChar == '#') {
        FuncCancelEdit();
      }

      break;



    case 34:
      lcd.print(F("     VOLTAR     "));

      if (TeclaChar == 'D') {
        TelaSubmenu = 0;
        delay(200);
        lcd.clear();
      }
      break;




    case 40:

      if (nivel != 200) {
        lcd.print(F("  RESERV "));
        lcd.print(nivel);
        lcd.print(F(" %   "));
      } else {
        lcd.print(F("FALHA NO SENSOR "));
      }


      break;

    case 41:

      if (Automatico == true) {  //Se estuver no automatico
        lcd.print(F("  SIST EM AUTOM "));
      } else {
        lcd.print(F(" SIST EM MANUAL "));
      }

      break;

    case 42:

      if (CMBT == true) {  //Se estuver no automatico
        lcd.print(F("CMB TELHADO LIG "));
      } else {
        lcd.print(F("CMB TELHADO DESL"));
      }

      break;

    case 43:

      if (CMBP == true) {  //Se estuver no automatico
        lcd.print(F("CMB PULVERI LIG "));
      } else {
        lcd.print(F("CMB PULVERI DESL"));
      }

      break;


    case 44:

      lcd.print(F("     VOLTAR     "));

      if (TeclaChar == 'D') {
        TelaSubmenu = 0;
        delay(200);
        lcd.clear();
      }

      break;




  }  //FIM DO SWITCH CASE

  //Torna o SubMenu cíclico
  if (TelaSubmenu == 9) { TelaSubmenu = 11; }
  if (TelaSubmenu == 12) { TelaSubmenu = 10; }

  if (TelaSubmenu == 19) { TelaSubmenu = 24; }
  if (TelaSubmenu == 25) { TelaSubmenu = 20; }

  if (TelaSubmenu == 29) { TelaSubmenu = 34; }
  if (TelaSubmenu == 35) { TelaSubmenu = 30; }

  if (TelaSubmenu == 39) { TelaSubmenu = 44; }
  if (TelaSubmenu == 45) { TelaSubmenu = 40; }

}  //FIM DO VOID SUBMENU*/