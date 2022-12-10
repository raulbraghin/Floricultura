void FuncLeTeclado() {

  e = Keypad.read();

  if (e.bit.EVENT == KEY_JUST_RELEASED) {
    FuncTeclaCharPress((char)e.bit.KEY);
  }

  telas();

  seg3 = 0;  //Reseta contador de tempo da tela
}

//**********************************************
void FuncTeclaCharPress(char Tecla) {  //Verifica se a tecla pressiona é um caracter ou um número e converte para inteiro se for número

  if (Tecla == 'A' || Tecla == 'B' || Tecla == 'C' || Tecla == 'D' || Tecla == '*' || Tecla == '#') {
    TeclaChar = Tecla;
    TeclaNum = 99;
    Serial.print("A tecla pressionada foi ");
    Serial.println(TeclaChar);
  } else {
    TeclaNum = int(Tecla) - 48;  //Conversão para int
    TeclaChar = 'Z';
    Serial.print("A tecla pressionada foi ");
    Serial.println(TeclaNum);
  }

  delay(200);
}

//**********************************************
void telas() {

  if (TeclaChar == '*' && TelaSubmenu == 0) {
    TelaMenu--;

    if (TelaMenu == 2 && Automatico == true) {
      TelaMenu--;
    }

    if (TelaMenu == 3 && Automatico == false) {
      TelaMenu--;
    }

    delay(200);
  }

  if (TeclaChar == '#' && TelaSubmenu == 0) {
    TelaMenu++;

    if (TelaMenu == 2 && Automatico == true) {
      TelaMenu++;
    }

    if (TelaMenu == 3 && Automatico == false) {
      TelaMenu++;
    }

    delay(200);
  }

  if (TeclaChar == '*' && TelaSubmenu > 0) {
    TelaSubmenu--;
    
    delay(200);
  }


  if (TeclaChar == '#' && TelaSubmenu > 0) {
    TelaSubmenu++;
    
    delay(200);
  }


}  // Fim Void Telas

int FuncEdicao(int Tecla) {

  if (Tecla != 99) {

    String caracter = String(Tecla);

    ValorTemp += caracter;

    Serial.print("Variavel caracter ");
    Serial.println(caracter);
  }





  Serial.println("Função FuncEdicao");
  Serial.print("Variavel ValorTemp ");
  Serial.println(ValorTemp);


  if (TeclaChar == 'D' && ValorTemp != "") {

    retornValor = atoi(ValorTemp.c_str());  //converte String para Char e depois para inteiro
    ValorTemp = "";
    Serial.print("Variavel retornValor ");
    Serial.println(retornValor);

    return retornValor;
  }
}

void FuncCancelEdit() {
  Serial.println("FuncCancelEdit");
  edicao = false;
  ValorTemp = "";
}