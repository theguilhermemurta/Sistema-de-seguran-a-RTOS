#include <Arduino_FreeRTOS.h>

void Porta (void *pvParameters);
void Portao (void *pvParameters);
void Alarme (void *pvParameters);
void BotaoPanico (void *pvParameters);

void setup() {

  Serial.begin(9600); 

  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB, on LEONARDO, MICRO, YUN, and other 32u4 based boards.
  }

  xTaskCreate(Alarme,"Processo de Alarme", 100, NULL, 1, NULL);

  xTaskCreate(BotaoPanico, "Processo do Botao de Panico", 100, NULL, 1, NULL);

  xTaskCreate(Porta,(const portCHAR *)"Processo da tranca da Porta", 100, NULL, 1, NULL);

  xTaskCreate(PortaoEletronico,(const portCHAR *)"Processo do Portao Eletronico", 100, NULL, 1, NULL);

}

void loop() {
}

void Alarme (void *pvParameters) {

  (void) pvParameters;

  int StatusAlarme = 0;
  int SinalAlarme;
  int SensorPresenca;
  int BotaoReseta;

  pinMode(13, INPUT);
  pinMode(12, INPUT);
  pinMode(11, INPUT);

  for(;;){
    SinalAlarme = digitalRead(13);
    if (SinalAlarme == HIGH) {
      StatusAlarme = 1;
      Serial.println("O alarme foi ativado");
      vTaskDelay( 2000 / portTICK_PERIOD_MS );
    }
    while(StatusAlarme == 1){
      SensorPresenca = digitalRead(12);
      if (SensorPresenca == HIGH) {
        StatusAlarme = 2;
        Serial.println("O alarme esta disparado");
        while(StatusAlarme == 2){
            BotaoReseta = digitalRead(11);
            if (BotaoReseta == HIGH) {
              StatusAlarme = 0;
              Serial.println("O alarme foi resetado");
              vTaskDelay( 1000 / portTICK_PERIOD_MS );
            }
          } 
       }else{
         BotaoReseta = digitalRead(11);
           if (BotaoReseta == HIGH) {
             StatusAlarme = 0;
             Serial.println("O alarme foi resetado");
             vTaskDelay( 1000 / portTICK_PERIOD_MS );
           }
       }
    }
  }
}

void BotaoPanico (void *pvParameters) {

  (void) pvParameters;

  pinMode(10, INPUT);

  for(;;){
    int BotaoP = digitalRead(10);
    if (BotaoP == HIGH) {
      Serial.println("Todos os telefones cadastrados foram notificados com esta mensagem de panico");
      vTaskDelay( 1000 / portTICK_PERIOD_MS );
    }
  }
}

void Porta (void *pvParameters) {

  (void) pvParameters;

  pinMode(3, OUTPUT);
  pinMode(9, INPUT);

  for(;;){
    int AbrePorta = digitalRead(9);
    if (AbrePorta == HIGH) {
      digitalWrite(3, HIGH);
      vTaskDelay( 30000 / portTICK_PERIOD_MS );
      digitalWrite(3, LOW);
    }
  }
}

void PortaoEletronico (void *pvParameters) {

  (void) pvParameters;

  pinMode(2, OUTPUT);
  pinMode(8, INPUT);

  for(;;){
    int AbrePortao = digitalRead(8);
    if (AbrePortao == HIGH) {
      digitalWrite(2, HIGH);
      vTaskDelay( 300 / portTICK_PERIOD_MS );
      digitalWrite(2, LOW);
    }
  }
}
