#include <Thermistor.h>
#include <TimerOne.h>

Thermistor temp1 (0);
Thermistor temp2 (1);

int comando;
int timesolenoide = LOW;    // Variavel de tempo
int ValSolenoide = 9;       //pino 9 do Arduino
// controlador PWM
volatile int val1, val2, val3;
volatile int cont1;
volatile int valor1 = 0;
volatile boolean flag1;
// cálculo da temperatura
float temperatura_reator;
float temperatura_banho;
const int analogIn0 = A0;
const int analogIn1 = A1;
int RawValue = 0;
double Voltage = 0;
double tempC = 0;

void setup() {
  Serial.begin(9600);
  pinMode(10, OUTPUT);            // bomba reator
  pinMode(6, OUTPUT);             // bomba resfriador
  pinMode(8, OUTPUT);             // resistencia
  pinMode(9, OUTPUT);             // solenoide
  pinMode(11, OUTPUT);            // compressor
  Timer1.initialize(1000000);     // Inicializa o Timer1 e configura para um período de 10 segundos

  cont1 = 0;
  flag1 = false;
  valor1 = 0;
}

void loop() {
  if (Serial.available() > 0) {
    comando = Serial.read();
    switch (comando) {
      // comandos da bomba do resfriados
      case 'a': 
        digitalWrite(6, HIGH);
        break;
      case 'b':
        digitalWrite(6, LOW);
        break;
      // comandos da bomba do reator
      case 'c':
        digitalWrite(10, HIGH);
        break;
      case 'd':
        digitalWrite(10, LOW);
        break;
      // comandos da temperatura
      case 'e':
        //temperatura_reator = temp1.getTemp();
        RawValue = analogRead(analogIn0);
        Voltage = (RawValue / 1023.0) * 5000; // 5000 to get millivots.
        temperatura_reator = Voltage * 0.1;
        Serial.print(" ");
        Serial.print(temperatura_reator);
        break;
      case 'f':
        //temperatura_banho = temp2.getTemp();
        RawValue = analogRead(analogIn1);
        Voltage = (RawValue / 1023.0) * 5000; // 5000 to get millivots.
        temperatura_banho = Voltage * 0.1;
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
        break;
      // comandos do compressor
      case 'j':
        digitalWrite(11, HIGH);
        break;
      case 'k':
        digitalWrite(11, LOW);
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
}

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
