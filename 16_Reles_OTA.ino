#include <Wire.h>
#include <Adafruit_MCP23X17.h>
#include <WiFi.h>
#include <ESPAsyncWebSrv.h>
#include <ArduinoOTA.h>

Adafruit_MCP23X17 mcp;

const char* ssid = "CONTA4";
const char* password = "SENHA";

void setup() {
  // Inicialização da comunicação I2C
  Wire.begin();

  // Inicialização do MCP23017
  if (!mcp.begin_I2C()) {
    Serial.println("Falha ao inicializar o MCP23017");
    while (1);
  }

  // Configuração dos pinos como saída
  for (uint8_t i = 0; i < 16; i++) {
    mcp.pinMode(i, OUTPUT);
  }

  // Inicialização da conexão WiFi
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Conectando ao WiFi...");
  }
  Serial.println("Conexão WiFi estabelecida");

  // Configuração OTA
  ArduinoOTA.setHostname("meu_esp32");
  ArduinoOTA.begin();

  // Inicialização do servidor web assíncrono
  AsyncWebServer server(80);
  server.begin();

  Serial.println("Pronto");
}

void loop() {
  // Atualização OTA
  ArduinoOTA.handle();

  // Alterna o estado dos pinos de 0 a 15
  for (uint8_t i = 0; i < 16; i++) {
    mcp.digitalWrite(i, HIGH);
    delay(100);
    mcp.digitalWrite(i, LOW);
    delay(100);
  }
}
