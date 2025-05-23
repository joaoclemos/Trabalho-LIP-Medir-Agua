#include <TimerOne.h>
#include <Wire.h>
#include <MultiFuncShield.h>

float dist;
bool stateBotao = false;

void setup() {
  Serial.begin(9600);
  Timer1.initialize();
  MFS.initialize(&Timer1);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
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
  if (dist <= 100) {
    return dist;
  }
  return 0;
}

int Potenciometro() {
  int x = analogRead(POT_PIN);
  int result = map(x, 0, 1023, 0, 25);
  return result;
}

void Buzzer() {
  MFS.beep();
  delay(1000);
  MFS.beep(5, 5, 4, 3, 50);
}

void loop() {
  byte btn = MFS.getButton();
  byte buttonNumber = btn & B00111111;
  byte buttonAction = btn & B11000000;

  if (buttonAction == BUTTON_LONG_RELEASE_IND && buttonNumber == 1) {
    Serial.println("toggle sistema");
    stateBotao = !stateBotao;
  }

  if (stateBotao) {
    int result = Potenciometro();
    float X = CalcularPorcent();
    Serial.print("Distancia: ");
    Serial.println(X);
    MFS.write(X, 1);

    if (X <= result && X > 0) {
      MFS.writeLeds(LED_1 | LED_2 | LED_3 | LED_4, ON);
      Buzzer();
    } else if (X <= 50) {
      MFS.writeLeds(LED_1 | LED_2, ON);
      MFS.writeLeds(LED_3 | LED_4, OFF);
    } else if (X <= 75) {
      MFS.writeLeds(LED_1, ON);
      MFS.writeLeds(LED_2 | LED_3 | LED_4, OFF);
    } else {
      MFS.writeLeds(LED_1 | LED_2 | LED_3 | LED_4, OFF);
    }
  } else {
    MFS.writeLeds(LED_ALL, OFF);
    MFS.write("");
  }
  delay(200);
}