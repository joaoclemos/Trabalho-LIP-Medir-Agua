#include <TimerOne.h>                   
#include <Wire.h>                         
#include <MultiFuncShield.h> 
float dist; 
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


int Potenciometro() 
{
int x=analogRead(POT_PIN); 
int result = map(x,0,1023,0,25); 
MFS.write(result);        
delay(100);  
return result; 
}


float CalcularPorcent() 
{ 
dist = 0.01723 * calcularDistancia(6, 5); 
float X=0; 
Serial.println(X); 
X=(dist); 
if(X<=100) 
return 100 - X; 
}


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
}
if(estadoBotao) { 
int result=Potenciometro();  
float X=CalcularPorcent(); 
Serial.println(X); 
MFS.write(X, 1); 
if(X<result) { 
MFS.writeLeds(LED_1 | LED_2 | LED_3 | LED_4, ON); 
MFS.beep();  
} else if(X<=50) { 
MFS.writeLeds(LED_1 | LED_2, ON); 
MFS.writeLeds(LED_3 | LED_4, OFF);  
} else if(X<=75) { 
MFS.writeLeds(LED_1, ON); 
MFS.writeLeds(LED_4 | LED_3 | LED_2, OFF);   
} else { 
MFS.writeLeds(LED_1 | LED_2 | LED_3 | LED_4, OFF); 
} 
} else { 
MFS.writeLeds(LED_ALL, OFF); 
MFS.write(""); 
} 
delay(200); 
}  
