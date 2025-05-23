#include <TimerOne.h>                   
#include <Wire.h>                         
#include <MultiFuncShield.h> 
float dist = 0; 
bool estadoBotao = false; 
void setup() 
{ 
Serial.begin(9600); 
Timer1.initialize();                    
MFS.initialize(&Timer1);                
estadoBotao = false; 
pinMode(5, OUTPUT); 
pinMode(6, OUTPUT); 
} 
void loop() 
{ 
byte btn = MFS.getButton(); 
byte buttonNumber = btn & B00111111; 
byte buttonAction = btn & B11000000; 
if(buttonAction == BUTTON_LONG_RELEASE_IND && buttonNumber == 1) { 
Serial.println("toggle sistema"); 
if(estadoBotao) 
estadoBotao = false; 
else 
estadoBotao = true; 
}//se o botao estiver ativado vai rodar isso 
if(estadoBotao) { 
//chamando a funcao do potenciometro 
int resultado=Potenciometro(); 
//calc porcent 
float X=CalcularPorcent(); 
Serial.println(X); 
MFS.write(X, 1); 
//se for menor que o potenciometro/25 
if(X<resultado) { 
MFS.writeLeds(LED_1 | LED_2 | LED_3 | LED_4, ON); 
MFS.beep(); 
//se for menor que 50 e maior que 25 
} else if(X<=50) { 
MFS.writeLeds(LED_1 | LED_2, ON); 
MFS.writeLeds(LED_3 | LED_4, OFF); 
//se for menor que 75 e maior que 50 
} else if(X<=75) { 
MFS.writeLeds(LED_1, ON); 
MFS.writeLeds(LED_4 | LED_3 | LED_2, OFF); 
//se nao pegar a distancia  
} else { 
MFS.writeLeds(LED_1 | LED_2 | LED_3 | LED_4, OFF); 
} 
//se o botao nao for apertado 
} else { 
MFS.writeLeds(LED_ALL, OFF); 
MFS.write(""); 
} 
delay(200); 
} 
//calcular a distancia 
float calcularDistancia(int trigger, int echo) { 
pinMode(trigger, OUTPUT); 
digitalWrite(trigger, LOW); 
delay(2); 
digitalWrite(trigger, HIGH); 
delay(10); 
digitalWrite(trigger, LOW); 
pinMode(echo, INPUT); 
return pulseIn(echo, HIGH); 
} 
//calcular a porcentegem 
float CalcularPorcent() 
{ 
dist = 0.01723 * calcularDistancia(6, 5); 
float X=0; 
Serial.println(X); 
X=(dist); 
if(X<=100) 
return X; 
} 
//codigo do potenciometro 
int Potenciometro() 
{ 
//potenciometro 
int x=analogRead(POT_PIN); 
int resultado = map(x,0,1023,0,25); 
MFS.write(resultado);        
delay(100);  
return resultado; 
} 