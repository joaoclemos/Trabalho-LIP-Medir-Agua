#include <TimerOne.h>
#include <Wire.h>
#include <MultiFuncShield.h>

float dist;
bool stateBotao = false;
bool ultimoEstadoBotao = false;

void setup() {
  Serial.begin(9600);
  Timer1.initialize();
  MFS.initialize(&Timer1);
  pinMode(5, OUTPUT); // Echo
  pinMode(6, OUTPUT); // Trigger
}

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

float CalcularPorcent() {
  dist = 0.01723 * readUltrasonicDistance(6, 5);
  float porcent = 100 - dist;
  if (porcent < 0) {
    porcent = 0;
  }
  return porcent;
}

int Potenciometro() {
  int x = analogRead(POT_PIN);
  int result = map(x, 0, 1023, 0, 25);
  return result;
}

void loop() {
  byte btn = MFS.getButton();
  byte buttonNumber = btn & B00111111;
  byte buttonAction = btn & B11000000;

  if (buttonAction == BUTTON_LONG_RELEASE_IND && buttonNumber == 1) {
    stateBotao = !stateBotao;
    ultimoEstadoBotao = true;
  } else if (buttonAction == BUTTON_RELEASED_IND) {
    ultimoEstadoBotao = false;
  }

  if (stateBotao) {
    int result = Potenciometro();
    float X = CalcularPorcent();
    Serial.println(X);
    MFS.write(X, 1);

    int num1 = 10000 / result;
    int num2 = 7500 / result;
    int num3 = 5000 / result;
    int num4 = 2500 / result;

    if (dist <= num1) {
      if (dist >= num2) {
        MFS.writeLeds(LED_1 | LED_2 | LED_3 | LED_4, OFF);
      } else if (dist >= num3) {
        MFS.writeLeds(LED_1, ON);
        MFS.writeLeds(LED_2 | LED_3 | LED_4, OFF);
      } else if (dist >= num4) {
        MFS.writeLeds(LED_1 | LED_2, ON);
        MFS.writeLeds(LED_3 | LED_4, OFF);
      } else {
        MFS.writeLeds(LED_1 | LED_2 | LED_3 | LED_4, ON);
        MFS.beep();
      }
    } else {
      MFS.writeLeds(LED_1 | LED_2 | LED_3 | LED_4, OFF);
    }
  } else {
    MFS.writeLeds(LED_ALL, OFF);
    MFS.write("");
  }
  delay(200);
}