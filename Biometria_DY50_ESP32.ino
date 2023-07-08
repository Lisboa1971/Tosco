#include <Adafruit_Fingerprint.h>
#include <HardwareSerial.h>

// Configuração do sensor de impressão digital
HardwareSerial fingerSerial(2);
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&fingerSerial);

const int pinLed = 2;        // LED azul embutido no ESP32
const int pinOutputValid = 25;    // Pino GPIO para acionar a saída quando a digital for válida
const int pinOutputInvalid = 26;  // Pino GPIO para acionar a saída quando a digital for inválida

void setup()
{
  Serial.begin(9600);
  while (!Serial)
    ;  // Aguarda a abertura da porta serial
  delay(100);

  Serial.println("Inicializando o sensor de impressão digital...");

  // Inicializa o sensor de impressão digital
  fingerSerial.begin(57600, SERIAL_8N1, 16, 17);
  delay(5);

  // Verifica se o sensor está conectado corretamente
  if (finger.verifyPassword())
  {
    Serial.println("Sensor de impressão digital encontrado!");
  }
  else
  {
    Serial.println("Falha ao encontrar o sensor de impressão digital. Verifique as conexões.");
    while (1)
      ;
  }

  pinMode(pinLed, OUTPUT);
  digitalWrite(pinLed, LOW);

  pinMode(pinOutputValid, OUTPUT);
  digitalWrite(pinOutputValid, HIGH);  // Configura o pino GPIO 25 como HIGH inicialmente

  pinMode(pinOutputInvalid, OUTPUT);
  digitalWrite(pinOutputInvalid, HIGH);  // Configura o pino GPIO 26 como HIGH inicialmente
}

void loop()
{
  verificarDigital();
  delay(1000);
}

void verificarDigital()
{
  Serial.println("Coloque o dedo no sensor...");

  // Aguarda até que o dedo seja detectado
  while (!finger.getImage())
    ;

  Serial.println("Remova o dedo...");

  // Aguarda até que o dedo seja removido
  while (finger.getImage())
    ;

  // Captura a imagem do dedo
  if (finger.image2Tz(1) == FINGERPRINT_OK)
  {
    Serial.println("Impressão digital capturada. Verificando...");

    // Verifica a correspondência da impressão digital
    if (finger.fingerFastSearch() == FINGERPRINT_OK)
    {
      digitalWrite(pinLed, HIGH);
      digitalWrite(pinOutputValid, LOW);    // Inverte o estado do pino GPIO 25
      Serial.println("Impressão digital válida encontrada!");
      delay(100);
      digitalWrite(pinLed, LOW);
      digitalWrite(pinOutputValid, HIGH);   // Retorna o estado do pino GPIO 25 ao valor inicial
    }
    else
    {
      digitalWrite(pinOutputInvalid, LOW);  // Inverte o estado do pino GPIO 26
      Serial.println("Impressão digital inválida.");
      delay(5000);
      digitalWrite(pinOutputInvalid, HIGH); // Retorna o estado do pino GPIO 26 ao valor inicial
    }
  }
  else
  {
    Serial.println("Erro ao capturar a impressão digital.");
  }
}
