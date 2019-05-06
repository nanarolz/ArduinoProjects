#include <Thermistor.h>
#include <TimerOne.h>
#include <MsTimer2.h>

Thermistor temp1 (0);
Thermistor temp2 (1);

int comando;
int timesolenoide = LOW;    // Variavel de tempo
int ValSolenoide = 9;

volatile int val1, val2, val3;
volatile int cont1;
volatile int valor1 = 0;
volatile boolean flag1;

float temperatura_reator = 0;
float temperatura_banho = 0;
const int analogIn0 = A0;
const int analogIn1 = A1;
int RawValue = 0;
double Voltage = 0;
double tempC = 0;

float cont = 0;
float TempM = 0;
float soma = 0;
int n_rodadas = 15;

int timecondensador = LOW;
int condensador = 11;

void setup() {
  Serial.begin(9600);
  pinMode(10, OUTPUT);                      // bomba reator
  pinMode(6, OUTPUT);                       // bomba resfriador
  pinMode(8, OUTPUT);                       // resistencia
  pinMode(9, OUTPUT);                       // solenoide
  pinMode(11, OUTPUT);                      // condensador
  Timer1.initialize(1000000);               // Inicializa o Timer1 e configura para um período de 1 segundos (microsegundos)
  MsTimer2::set(60000,startcondensador);      // Inicializa o MsTimer2 e configura para um período de 1 minuto (milisegundos)

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
      case 'd': // desliga bomba reator
        digitalWrite(10, LOW);
        break;
      case 'e': // temperatura do reator
        soma = 0;
        for(int i = 0; i < n_rodadas; i++)
        {
          RawValue = analogRead(analogIn0);
          Voltage = (RawValue / 1023.0) * 5000; // 5000 to get millivots.
          temperatura_reator = Voltage * 0.1;
          soma = soma + temperatura_reator;
          delay(100);
        }
        TempM = soma / n_rodadas;
        Serial.print(" ");
        Serial.print(TempM);
        break;
      case 'f': // temperatura do banho
        soma = 0;
        for(int i = 0; i < n_rodadas; i++)
        {
          RawValue = analogRead(analogIn1);
          Voltage = (RawValue / 1023.0) * 5000; // 5000 to get millivots.
          temperatura_banho = Voltage * 0.1;
          soma = soma + temperatura_banho;
          delay(100);
        }
        TempM = soma / n_rodadas;
        Serial.print(" ");
        Serial.print(TempM);
        break;
      case 'R': // controle da resistencia
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
        MsTimer2::start();
        break;
      case 'k': // desliga condensador
        MsTimer2::stop();
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
  /*
     MONITORAR AS TEMEPERATURAS

          RawValue = analogRead(analogIn0);
          Voltage = (RawValue / 1023.0) * 5000; // 5000 to get millivots.
          temperatura_reator = Voltage * 0.1;
          Serial.print("A0: ");
          Serial.print(temperatura_reator);
          Serial.print("\n");
          RawValue = analogRead(analogIn1);
          Voltage = (RawValue / 1023.0) * 5000; // 5000 to get millivots.
          temperatura_banho = Voltage * 0.1;
          Serial.print("A1: ");
          Serial.print(temperatura_banho);
          Serial.print("\n");
          delay(1000);
  */

  // Contagem para atuação no relé da resistência
  cont1++;
  delay(20);
  if (cont1 > 100) cont1 = 0;

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
}

//Função que aciona a válvula solenoide
void startvalsolenoide()
{
  if (timesolenoide == LOW) {
    timesolenoide = HIGH;

  } else {
    timesolenoide = LOW;
  }
  digitalWrite(ValSolenoide, timesolenoide);
}

//Função que aciona o condensador
void startcondensador()
{
  static boolean output = HIGH;
  digitalWrite(condensador, output);
  output = !output;
}
