#include <BluetoothSerial.h>
#include <PubSubClient.h>
#include <Adafruit_MCP23X17.h>

// Configuração do dispositivo Bluetooth
BluetoothSerial SerialBT;

// Configuração do servidor MQTT
const char* mqtt_server = "seuServidorMQTT";
const int mqtt_port = 1883;

// Configuração do cliente Bluetooth e MQTT
BluetoothSerial btClient;
PubSubClient client(btClient);

// Configuração do MCP23X17
Adafruit_MCP23X17 mcp;

void setup() {
  Serial.begin(115200);
  SerialBT.begin("ESP32"); // Nome do dispositivo Bluetooth

  // Configura o MCP23X17
  mcp.begin(0); // Endereço I2C padrão é 0
  
  // Configura o cliente MQTT
  client.setServer(mqtt_server, mqtt_port);
  
  Serial.println("Setup concluído");
}

void loop() {
  if (!client.connected()) {
    reconnectMQTT();
  }
  
  client.loop();
  
  // Seu código aqui
  
  delay(1000);
}

void reconnectMQTT() {
  while (!client.connected()) {
    Serial.println("Conectando ao servidor MQTT...");
    
    if (client.connect("ESP32Client")) {
      Serial.println("Conectado ao servidor MQTT");
    } else {
      Serial.print("Falha na conexão com o servidor MQTT, estado: ");
      Serial.print(client.state());
      Serial.println(" Tentando novamente em 5 segundos...");
      delay(5000);
    }
  }
}
