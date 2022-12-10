void FuncAjusteRTC() {

  RtcDateTime compiled = RtcDateTime(__DATE__, __TIME__);  //VARIÁVEL RECEBE DATA E HORA DE COMPILAÇÃO

  if (Rtc.GetIsWriteProtected()) {                                                          //SE O RTC ESTIVER PROTEGIDO CONTRA GRAVAÇÃO, FAZ
    Serial.println("RTC está protegido contra gravação. Habilitando a gravação agora...");  //IMPRIME O TEXTO NO MONITOR SERIAL
    Rtc.SetIsWriteProtected(false);                                                         //HABILITA GRAVAÇÃO NO RTC
    Serial.println();                                                                       //QUEBRA DE LINHA NA SERIAL
  }

  if (!Rtc.GetIsRunning()) {                                                           //SE RTC NÃO ESTIVER SENDO EXECUTADO, FAZ
    Serial.println("RTC não está funcionando de forma contínua. Iniciando agora...");  //IMPRIME O TEXTO NO MONITOR SERIAL
    Rtc.SetIsRunning(true);                                                            //INICIALIZA O RTC
    Serial.println();                                                                  //QUEBRA DE LINHA NA SERIAL
  }

  RtcDateTime now = Rtc.GetDateTime();                                                                                //VARIÁVEL RECEBE INFORMAÇÕES
  if (now < compiled) {                                                                                               //SE A INFORMAÇÃO REGISTRADA FOR MENOR QUE A INFORMAÇÃO COMPILADA, FAZ
    Serial.println("As informações atuais do RTC estão desatualizadas. Atualizando informações...");                  //IMPRIME O TEXTO NO MONITOR SERIAL
    Rtc.SetDateTime(compiled);                                                                                        //INFORMAÇÕES COMPILADAS SUBSTITUEM AS INFORMAÇÕES ANTERIORES
    Serial.println();                                                                                                 //QUEBRA DE LINHA NA SERIAL
  } else if (now > compiled) {                                                                                        //SENÃO, SE A INFORMAÇÃO REGISTRADA FOR MAIOR QUE A INFORMAÇÃO COMPILADA, FAZ
    Serial.println("As informações atuais do RTC são mais recentes que as de compilação. Isso é o esperado.");        //IMPRIME O TEXTO NO MONITOR SERIAL
    Serial.println();                                                                                                 //QUEBRA DE LINHA NA SERIAL
  } else if (now == compiled) {                                                                                       //SENÃO, SE A INFORMAÇÃO REGISTRADA FOR IGUAL A INFORMAÇÃO COMPILADA, FAZ
    Serial.println("As informações atuais do RTC são iguais as de compilação! Não é o esperado, mas está tudo OK.");  //IMPRIME O TEXTO NO MONITOR SERIAL
    Serial.println();                                                                                                 //QUEBRA DE LINHA NA SERIAL
  }
}

int FuncMonitoraHora(const RtcDateTime& dt) {

  char datestring[20];  //VARIÁVEL ARMAZENA AS INFORMAÇÕES DE DATA E HORA

  snprintf_P(datestring,
             countof(datestring),
             PSTR("%02u/%02u/%04u %02u:%02u:%02u"),  //FORMATO DE EXIBIÇÃO DAS INFORMAÇÕES
             dt.Day(),                               //DIA
             dt.Month(),                             //MÊS
             dt.Year(),                              //ANO
             dt.Hour(),                              //HORA
             dt.Minute(),                            //MINUTOS
             dt.Second());                           //SEGUNDOS
  Serial.print(datestring);                          //IMPRIME NO MONITOR SERIAL AS INFORMAÇÕES

  String HORA;

  HORA = dt.Hour();

  int iHora = atoi(HORA.c_str());

  return iHora;
}