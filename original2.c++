#include <TimerOne.h>
#include <Wire.h>
#include <MultiFuncShield.h>

void setup() {
  Serial.begin(9600);
  Timer1.initialize();
  MFS.initialize(&Timer1);
}

int dist = 0;
float porcent;
bool ativo = false;
bool ultimoEstadoBotao = false;
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
  MFS.beep(5, 5, 4, 3, 50);
}

void loop() {
  dist = 0.01723 * readUltrasonicDistance(5, 6);
  Serial.println(dist);
  Serial.println(potenciometroCorte);
  byte btn = MFS.getButton();
  potenciometro = analogRead(POT_PIN);
  potenciometroCorte = map(potenciometro, 0, 1023, 1, 100);

  if (btn == BUTTON_1_PRESSED && !ultimoEstadoBotao) {
    ativo = !ativo;
    MFS.writeLeds(LED_4, ativo ? ON : OFF);
    MFS.beep(5, 5, 1, 1, 50);
  }
  ultimoEstadoBotao = (btn == BUTTON_1_PRESSED);

  num1 = 1000 / potenciometroCorte;
  num2 = 750 / potenciometroCorte;
  num3 = 500 / potenciometroCorte;
  num4 = 250 / potenciometroCorte;

  if (ativo) {
    dist = 0.01723 * readUltrasonicDistance(6, 5);
    potenciometro = analogRead(POT_PIN);
    potenciometroCorte = map(potenciometro, 0, 1023, 0, 100);

    porcent = 100 - dist;
    if (porcent < 0) {
      porcent = 0;
    } else if (porcent > 100) {
      porcent = 100;
    }
    MFS.write((int)porcent);

    if (dist <= num1) {
      Serial.print("Distance: ");
      Serial.println(dist);
      Serial.println(num1);
      Serial.println(num2);
      Serial.println(num3);
      Serial.print(" cm, Percentage: ");
      Serial.println(porcent);


      if (dist >= num2) {
        MFS.writeLeds(LED_1 | LED_2 | LED_3, OFF);
      } else if (dist >= num3) {
        MFS.writeLeds(LED_1, ON);
        MFS.writeLeds(LED_2 | LED_3, OFF);
      } else if (dist >= num4) {
        MFS.writeLeds(LED_1 | LED_2, ON);
        MFS.writeLeds(LED_3, OFF);
      } else {
        MFS.writeLeds(LED_1 | LED_2 | LED_3, ON);
        Buzzer();
      }
    } else {
      MFS.writeLeds(LED_1 | LED_2 | LED_3, OFF);
    }
  } else {
    MFS.writeLeds(LED_1 | LED_2 | LED_3 | LED_4, OFF);
    //MFS.write(0);
  }

  delay(100);
}