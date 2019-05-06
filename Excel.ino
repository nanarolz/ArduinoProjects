#include <Thermistor.h>
#include <TimerOne.h>

Thermistor temp1 (0);
Thermistor temp2 (1);
float temperatura_reator, temperatura_banho;
int comando;
int timesolenoide = LOW;    // Variavel de tempo
int ValSolenoide = 9;       //pino 9 do Arduino

volatile int val1, val2, val3;
volatile int cont1;
volatile int valor1;
volatile boolean flag1;


void setup() {
  Serial.begin(9600);
  pinMode(10, OUTPUT);            // bomba reator
  pinMode(6, OUTPUT);             // bomba resfriador
  pinMode(8, OUTPUT);             // resistencia
  pinMode(9, OUTPUT);             // solenoide
  pinMode(11, OUTPUT);            // compressor
  Timer1.initialize(1000000);     // Inicializa o Timer1 e configura para um período de 10 segundos
}

void loop() {
  if (Serial.available() > 0) {
    comando = Serial.read();
    switch (comando) {
      // comandos da bomba do resfriados
      case 'a':
        digitalWrite(6, HIGH);
        //Serial.print(" ");
        //Serial.print(char(comando));
        break;
      case 'b':
        digitalWrite(6, LOW);
        //Serial.print(" ");
        //Serial.print(char(comando));
        break;
      // comandos da bomba do reator
      case 'c':
        digitalWrite(10, HIGH);
        //Serial.print(" ");
        //Serial.print(char(comando));
        break;
      case 'd':
        digitalWrite(10, LOW);
        //Serial.print(" ");
        //Serial.print(char(comando));
        break;
      case 'e':                               // aquisição da temperatura da saída do reator
        temperatura_reator = temp1.getTemp();
        Serial.print(" ");
        Serial.print(temperatura_reator);
        break;
      case 'f':                               // aquisição da tempereratura da saída do tanque de resistencia
        temperatura_banho = temp2.getTemp();
        Serial.print(" ");
        Serial.print(temperatura_banho);
        break;
      // comandos da resistencia
      case 'R':
        delay(5);
        val1 = Serial.read();
        delay(5);
        val2 = Serial.read();
        delay(5);
        val3 = Serial.read();
        delay(5);
        if (val1 >= 176) val1 = val1 - 128;
        if (val2 >= 176) val2 = val2 - 128;
        if (val3 >= 176) val3 = val3 - 128;
        valor1 = int((val1 - 48) * 100 + (val2 - 48) * 10 + (val3 - 48));
        valor1 = valor1;
        //Serial.print(" ");
        //Serial.print(char(comando));
        break;
      // comandos do compressor
      case 'j':
        digitalWrite(11, HIGH);
        //Serial.print(" ");
        //Serial.print(char(comando));
        break;
      case 'k':
        digitalWrite(11, LOW);
        //Serial.print(" ");
        //Serial.print(char(comando));
        break;
      // comandos da solenoide
      case 'l':
        Timer1.attachInterrupt(startvalsolenoide);
        break;
      case 'm':
        Timer1.detachInterrupt();
        digitalWrite(ValSolenoide, LOW);
        break;
    }
  }
  
 // Contagem para atuação no relé da resistência
  cont1++;
  delay(20);
  if (cont1 > 100) cont1 = 0;
  /*
  if (cont1 == 100 && flag1 == false)
  {
    digitalWrite(8, HIGH);
    flag1 = true;
  }
  */
  if (cont1 > valor1 && flag1 == true)
  {
    digitalWrite(8, LOW);
    flag1 = false;
  }
  if (cont1 < valor1 && flag1 == false)
  {
    digitalWrite(8, HIGH);
    flag1 = true;
  }

} // FIM DO LOOP
//Função que aciona a válvula solenoide
void startvalsolenoide(void)
{
  if (timesolenoide == LOW) {
    timesolenoide = HIGH;

  } else {
    timesolenoide = LOW;
  }
  digitalWrite(ValSolenoide, timesolenoide);
}
