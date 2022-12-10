void FuncCMB() {

  if (CMBT == true) {
    digitalWrite(bomba_telhado, LOW);
  } else {
    digitalWrite(bomba_telhado, HIGH);
  }

  if (CMBP == true && seg2 >= 2) {  //Liga bomba de pulverização após atraso de 2 segundos para nunca ligar junto com a bomba do telhado
    digitalWrite(bomba_pulve, LOW);
  } else {
    digitalWrite(bomba_pulve, HIGH);
  }
}

int Funcboias() {

  bool vazio = digitalRead(boia_inf);
  bool medio = digitalRead(boia_meio);
  bool cheio = digitalRead(boia_sup);

  ReservOK = vazio;


  if (vazio == false && medio == false && cheio == false) {
    return 0;
  } else if (vazio == true && medio == false && cheio == false) {
    return 10;
  } else if (vazio == true && medio == true && cheio == false) {
    return 50;
  } else if (vazio == true && medio == true && cheio == true) {
    return 100;
  } else {
    return 200;
  }
}

void FuncSistemaAuto() {

  if (Automatico == true && ReservOK == true) {  //Sistema no automático com água no reservatório


    if (temperatura > temperatura_acionamento && CMBT == false) {
      CMBT = true;
    }

    if (temperatura < temperatura_desacionamento && CMBT == true) {
      CMBT = false;
    }

    if (umidade < umidade_acionamento && CMBP == false) {
      CMBP = true;
      seg2 = 0;
    }

    if (umidade > umidade_desacionamento && CMBP == true) {
      CMBP = false;
    }
  }

  if (Automatico == true && ReservOK == false) {  //Sistema no automático sem água no reservatório
    CMBT = false;
    CMBP = false;
  }
}

void FuncSistemaMan() {

  if (Automatico == false && LigaCMBT == true) {
    CMBT = true;
  }

  if (Automatico == false && LigaCMBT == false) {
    CMBT = false;
  }

  if (Automatico == false && LigaCMBP == true) {
    CMBP = true;
  }

  if (Automatico == false && LigaCMBP == false) {
    CMBP = false;
  }
}