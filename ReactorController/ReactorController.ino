#include <thermistor.h>
#include <TimerOne.h>
#include <MsTimer2.h>

Thermistor temp1 (0);
Thermistor temp2 (1);
float temperatura_reator = 0.0;
float temperatura_banho = 0.0;

int comando = 0;

int timesolenoide = LOW;    // Variavel de tempo
int ValSolenoide = 9;       //pino 9 do Arduino

volatile int val1 = 0;
volatile int val2 = 0;
volatile int val3 = 0;
volatile int cont1 = 0;
volatile int valor1 = 0;
volatile boolean flag1 = false;

float cont = 0;
float TempM = 0;
float soma = 0;
int n_rodadas = 15;

int timecondensador = LOW;
int condensador = 11;
unsigned  long tempoatual;
unsigned  long tempoinicial;
unsigned  long tempotermino;
int tempoON = 0;
bool flag = false;
volatile int val4 = 0;
volatile int val5 = 0;
volatile int val6 = 0;
volatile float valor2 = 0;

void setup()
{
  Serial.begin(9600);
  pinMode(10, OUTPUT);                        // bomba reator
  pinMode(6, OUTPUT);                         // bomba resfriador
  pinMode(8, OUTPUT);                         // resistencia
  pinMode(9, OUTPUT);                         // solenoide
  pinMode(11, OUTPUT);                        // compressor
  Timer1.initialize(1000000);                 // Inicializa o Timer1 e configura para um período de 1 segundos
  MsTimer2::set(1000, startcondensador);      // Inicializa o MsTimer2 e configura para um período de 1 minuto (milisegundos)
  cont1 = 0;
  flag1 = false;
  valor1 = 0;
}

void loop()
{
  if (Serial.available() > 0)
  {
    comando = Serial.read();
    switch (comando)
    {
      case 'a': // liga bomba resfriador
        digitalWrite(6, HIGH);
        break;
      case 'b': // desliga bomba resfriador
        digitalWrite(6, LOW);
        break;
      case 'c': // liga bomba reator
        digitalWrite(10, HIGH);
        break;
      case 'd': // desliga bomba resfriador
        digitalWrite(10, LOW);
        break;
      case 'e': // aquisição da temperatura da saída do reator
        soma = 0;
        for (int i = 0; i < n_rodadas; i++) {
          temperatura_reator = temp1.getTemp();
          soma = soma + temperatura_reator;
          delay(100);
        }
        TempM = (soma / n_rodadas);
        Serial.print(" ");
        Serial.print(TempM);
        break;
      case 'f': // aquisição da tempereratura da saída do tanque de resistencia
        soma = 0;
        for (int i = 0; i < n_rodadas; i++) {
          temperatura_banho = temp2.getTemp();
          soma = soma + temperatura_banho;
          delay(100);
        }
        TempM = (soma / n_rodadas);
        Serial.print(" ");
        Serial.print(TempM);
        break;
      case 'R': // liga resistencia
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
        break;
      case 'j': // liga condensador
        delay(5);
        val4 = Serial.read();
        delay(5);
        val5 = Serial.read();
        delay(5);
        val6 = Serial.read();
        delay(5);
        if (val4 >= 176) val4 = val4 - 128;
        if (val5 >= 176) val5 = val5 - 128;
        if (val6 >= 176) val6 = val6 - 128;
        valor2 = int((val4 - 48) * 100 + (val5 - 48) * 10 + (val6 - 48));
        valor2 = valor2 / 100;
        tempoON = valor2 * 60000; // janela de 1 minuto
        tempoinicial = millis();
        tempotermino = tempoinicial + 60000;
        tempoatual = millis();
        MsTimer2::start();
        break;
      case 'k': // desliga condensador
        MsTimer2::stop();
        flag = false;
        digitalWrite(condensador, LOW);
        break;
      case 'l': // liga solenoide
        Timer1.attachInterrupt(startvalsolenoide);
        break;
      case 'm': // desliga solenoide
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
  if (flag)  // funcionamento do condensador
  {
    tempoatual = millis();
    if (tempoatual < tempoinicial + tempoON) {
      digitalWrite(condensador, HIGH);
    } else {
      digitalWrite(condensador, LOW);
    }
    if (tempoatual > tempotermino) { // depois de um minuto reinicia o contador
      tempoinicial = millis();
      tempotermino = tempoinicial + 60000;
    }
  }
}

void startvalsolenoide() //Função que aciona a válvula solenoide
{
  if (timesolenoide == LOW) {
    timesolenoide = HIGH;

  } else {
    timesolenoide = LOW;
  }
  digitalWrite(ValSolenoide, timesolenoide);
}

void startcondensador() //Função que aciona o condensador
{
  flag = true;
}
