#include <Wire.h>
#include <Adafruit_MCP23X17.h>

Adafruit_MCP23X17 mcp;

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
}

void loop() {
  // Alterna o estado dos pinos de 0 a 15
  for (uint8_t i = 0; i < 16; i++) {
    mcp.digitalWrite(i, HIGH);
    delay(500);
    mcp.digitalWrite(i, LOW);
    delay(500);
  }
}
