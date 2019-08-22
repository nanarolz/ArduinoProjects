
const int pinoLed1 = 12; //PINO DIGITAL UTILIZADO PELO LED
const int pinoLed2 = 11; //PINO DIGITAL UTILIZADO PELO LED
const int pinoLed3 = 10; //PINO DIGITAL UTILIZADO PELO LED
const int pinoSensor1 = 4; //PINO DIGITAL UTILIZADO PELO SENSOR
const int pinoSensor2 = 6; //PINO DIGITAL UTILIZADO PELO SENSOR
const int pinoSensor3 = 8; //PINO DIGITAL UTILIZADO PELO SENSOR

void setup()
{
  pinMode(pinoSensor1, INPUT); //DEFINE O PINO COMO ENTRADA
  pinMode(pinoSensor2, INPUT); //DEFINE O PINO COMO ENTRADA
  pinMode(pinoSensor3, INPUT); //DEFINE O PINO COMO ENTRADA

  pinMode(pinoLed1, OUTPUT); //DEFINE O PINO COMO SAÍDA
  digitalWrite(pinoLed1, LOW); //LED INICIA DESLIGADO

  pinMode(pinoLed2, OUTPUT); //DEFINE O PINO COMO SAÍDA
  digitalWrite(pinoLed2, LOW); //LED INICIA DESLIGADO

  pinMode(pinoLed3, OUTPUT); //DEFINE O PINO COMO SAÍDA
  digitalWrite(pinoLed3, LOW); //LED INICIA DESLIGADO

  Serial.begin(9600);
}

void loop()
{
  if (digitalRead(pinoSensor1) == LOW)
  { //SE A LEITURA DO PINO FOR IGUAL A LOW, FAZ
    digitalWrite(pinoLed1, HIGH); //ACENDE O LED
  } else
  { //SENÃO, FAZ
    digitalWrite(pinoLed1, LOW); //APAGA O LED
    Serial.println("SENSOR 1");
    delay(1000);
  }
  if (digitalRead(pinoSensor2) == LOW)
  { //SE A LEITURA DO PINO FOR IGUAL A LOW, FAZ
    digitalWrite(pinoLed2, HIGH); //ACENDE O LED
  } else
  { //SENÃO, FAZ
    digitalWrite(pinoLed2, LOW); //APAGA O LED
    Serial.println("SENSOR 2");
    delay(1000);
  }
  if (digitalRead(pinoSensor3) == LOW)
  { //SE A LEITURA DO PINO FOR IGUAL A LOW, FAZ
    digitalWrite(pinoLed3, HIGH); //ACENDE O LED
  } else
  { //SENÃO, FAZ
    digitalWrite(pinoLed3, LOW); //APAGA O LED
    Serial.println("SENSOR 3");
    delay(1000);
  }
}
