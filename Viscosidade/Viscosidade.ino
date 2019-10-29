
const int pinoLed1 = 12; //PINO DIGITAL UTILIZADO PELO LED
const int pinoSensor1 = 4; //PINO DIGITAL UTILIZADO PELO SENSOR
const int pinoSensor2 = 6; //PINO DIGITAL UTILIZADO PELO SENSOR
const int pinoSensor3 = 8; //PINO DIGITAL UTILIZADO PELO SENSOR

int contador1 = 0;
int contador2 = 0;
int contador3 = 0;

int comando;

unsigned long tempoinicial;
unsigned long tempodetectado;

void setup()
{
  pinMode(pinoSensor1, INPUT); //DEFINE O PINO COMO ENTRADA

  pinMode(pinoLed1, OUTPUT); //DEFINE O PINO COMO SAÍDA
  digitalWrite(pinoLed1, LOW); //LED INICIA DESLIGADO

  Serial.begin(9600);
}

void loop()
{
  comando = Serial.read();
  
  if (digitalRead(pinoSensor1) == HIGH && contador1 <= 0)
  {
    Serial.print("SENSOR 1: ");
    tempoinicial = millis();
    Serial.println("Detectado!");
    contador1++;
  }
  if (digitalRead(pinoSensor2) == HIGH && contador2 <= 0)
  {
    Serial.print("SENSOR 2: ");
    tempodetectado = millis();
    Serial.print(tempodetectado - tempoinicial);  
    Serial.println(" ms");
    contador2++;
  }
  if (digitalRead(pinoSensor3) == HIGH && contador3 <= 0)
  { 
    Serial.print("SENSOR 3: ");    
    tempodetectado = millis();
    Serial.print(tempodetectado - tempoinicial);  
    Serial.println(" ms");
    contador3++;
  }
  if(comando == 'a'){
    Serial.println("contadores zerados!");
    contador1 = 0;
    contador2 = 0;
    contador3 = 0;
  }

}
