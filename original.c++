#include <TimerOne.h>
#include <Wire.h>
#include <MultiFuncShield.h>

void setup() {
  Serial.begin(9600);
  Timer1.initialize();
  MFS.initialize(&Timer1);
}

int dist = 0;
float porcent = 0;
bool ativo = false;
bool ultimoEstadoBotao = false;
bool alertaButton = false;
int potenciometro;
int num1, num2, num3, num4;
int potenciometroCorte;


long readUltrasonicDistance(int triggerPin, int echoPin) {
  pinMode(triggerPin, OUTPUT);
  digitalWrite(triggerPin, LOW);
  delayMicroseconds(2);
  digitalWrite(triggerPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(triggerPin, LOW);
  pinMode(echoPin, INPUT);
  return pulseIn(echoPin, HIGH);
}

void Buzzer() {
  MFS.beep();
  delay(1000);
  MFS.beep(5, 5, 4, 3, 50);
}

void loop() {
  byte btn = MFS.getButton();
  potenciometro = analogRead(POT_PIN);
  potenciometroCorte = map(potenciometro, 0, 1023, 0, 255);

  if (btn == BUTTON_1_PRESSED && !ultimoEstadoBotao) 
  {
    ativo = !ativo;
    MFS.writeLeds(LED_4, ativo ? ON : OFF);
  }

  if (btn == BUTTON_1_PRESSED && !alertaButton) 
        {
          ativo = !ativo;
          MFS.beep(0, 0, 0, 0, 0);        
        }

   

  num1 = 10000 /potenciometroCorte;
  num2 = 7500 /potenciometroCorte;
  num3 = 5000 /potenciometroCorte;
  num4 = 2500 /potenciometroCorte;

  ultimoEstadoBotao = (btn == BUTTON_1_PRESSED);
  alertaButton = (btn == BUTTON_1_PRESSED);

  if (ativo) {
    dist = 0.01723 * readUltrasonicDistance(6, 5);
    porcent =  100 - dist;
    if (porcent < 0){
      porcent = 0;
    }

    if (dist <= num1) {
      Serial.println(dist);
      // MFS.write(dist);
      MFS.write(porcent);
      delay(200);

      if (dist >= num2) {
        MFS.writeLeds(LED_1 | LED_2 | LED_3, OFF);
      } else if (dist >= num3) {
        MFS.writeLeds(LED_1, ON);
      } else if (dist >= num4) {
        MFS.writeLeds(LED_1 | LED_2, ON);
      } else {
        MFS.writeLeds(LED_1 | LED_2 | LED_3, ON);
        Buzzer();

      }
    } else {
      MFS.writeLeds(LED_1 | LED_2 | LED_3, OFF);
    }
  } else {
    MFS.writeLeds(LED_1 | LED_2 | LED_3, OFF);
  }
}