const int sensorPin = A1;   // seleccionar la entrada para el sensor
float sensorValue;         // variable que almacena el valor raw (0 a 1023)
float value;            // variable que almacena el voltaje (0.0 a 5.0)
int fuel; // cantidad de combustible tanque
const int led = 13;

void setup()
{
  Serial.begin(9600);
}

void loop()
{
  lectura();
  alerta();
}


void lectura() {
  float sum;
  for (int i = 0; i < 100; i++) {
    analogReadResolution(12);
    sensorValue = analogRead(sensorPin);          // realizar la lectura
    sum += sensorValue;
    delayMicroseconds(50);
  }
  sum = sum /100;
  value = ((-sum +3755)/ 3755) * 100;
  Serial.println(value);
  delay(2000);
}

void alerta() {
  if (value < 15) {
    digitalWrite(led, HIGH);
  }
  else {
    digitalWrite(led, LOW);
  }
}

