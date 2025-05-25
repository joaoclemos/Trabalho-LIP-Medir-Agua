#include <TimerOne.h> 
#include <Wire.h> 
#include <MultiFuncShield.h> 
float distancia;           
// distância medida pelo sensor 
bool estadoBotao = false;  // estado do botão liga/desliga 
// Setup inicial, roda só uma vez pra configurar o serial, timer, 
escudo e pinos 
void setup() { 
Serial.begin(9600); 
Timer1.initialize(); 
MFS.initialize(&Timer1); 
estadoBotao = false; 
pinMode(5, OUTPUT); 
pinMode(6, OUTPUT); 
} 
// Função que mede a distância usando sensor ultrassom 
float medirDistancia(int trigger, int echo) { 
pinMode(trigger, OUTPUT); 
digitalWrite(trigger, LOW); 
delay(2); 
digitalWrite(trigger, HIGH); 
delay(10); 
digitalWrite(trigger, LOW); 
pinMode(echo, INPUT); 
return pulseIn(echo, HIGH); 
} 
// Calcula a porcentagem de água no tanque (altura total 100cm menos a 
distância medida) 
float calcularPorcentagem() { 
distancia = 0.01723 * medirDistancia(6, 5); 
float porcentagem = 100 - distancia; 
if (porcentagem < 0) porcentagem = 0; 
if (porcentagem > 100) porcentagem = 100; 
return porcentagem; 
} 
// Lê o potenciômetro, mapeia valor para 0-25, escreve no display e 
retorna o valor 
int Potenciometro() { 
int x = analogRead(POT_PIN); 
int resultado = map(x, 0, 1023, 0, 25); 
delay(100); 
return resultado; 
} 
// Loop principal que lê botão, alterna sistema e controla leds/beeps 
conforme nível do tanque 
void loop() { 
byte botao = MFS.getButton(); 
byte numeroBotao = botao & B00111111; 
byte acaoBotao = botao & B11000000; 
// Se o botão 1 foi pressionado longo e solto, liga/desliga o sistema 
if (acaoBotao == BUTTON_LONG_RELEASE_IND && numeroBotao == 1) { 
Serial.println("toggle sistema"); 
estadoBotao = !estadoBotao; 
} 
if (estadoBotao) { 
int valorPotenciometro = Potenciometro(); 
float porcentagem = calcularPorcentagem(); 
Serial.println(valorPotenciometro); 
MFS.write(porcentagem, 1); 
// Acende leds e faz beep dependendo do nível de água comparado ao 
potenciômetro 
if (porcentagem < valorPotenciometro) { 
MFS.writeLeds(LED_1 | LED_2 | LED_3 | LED_4, ON); 
MFS.beep(); 
} else if (porcentagem <= 50) { 
MFS.writeLeds(LED_1 | LED_2, ON); 
MFS.writeLeds(LED_3 | LED_4, OFF); 
} else if (porcentagem <= 75) { 
MFS.writeLeds(LED_1, ON); 
MFS.writeLeds(LED_4 | LED_3 | LED_2, OFF); 
} else { 
MFS.writeLeds(LED_1 | LED_2 | LED_3 | LED_4, OFF); 
} 
} else { 
// Sistema desligado: apaga tudo 
MFS.writeLeds(LED_ALL, OFF); 
MFS.write(""); 
} 
delay(200); 
}