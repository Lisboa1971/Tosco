#include <Wire.h>
#include <Adafruit_MCP23X17.h>
#include <WiFi.h>
#include <ESPAsyncWebSrv.h>

const char* ssid = "CONTA";
const char* password = "SENHA";

Adafruit_MCP23X17 mcp;

const char* htmlPage = "<html>\
<head>\
  <title>Controle da Iluminação</title>\
  <style>\
    @import url('https://fonts.googleapis.com/css2?family=Indie+Flower&display=swap');\
    @import url('https://cdnjs.cloudflare.com/ajax/libs/font-awesome/5.15.3/css/all.min.css');\
    body { font-family: 'Indie Flower', cursive; background-color: #eaf2ff; text-align: center; padding-top: 50px; }\
    .truck { font-size: 80px; margin-bottom: 20px; }\
    h1 { color: #ff5722; font-size: 36px; margin-bottom: 20px; }\
    .button-container { display: flex; flex-wrap: wrap; justify-content: center; }\
    .button { background-color: #4CAF50; border: none; color: white; padding: 16px 40px; margin: 8px; text-align: center; text-decoration: none; display: inline-block; font-size: 20px; cursor: pointer; transition-duration: 0.4s; }\
    .button-on { background-color: #4CAF50; }\
    .button-on:hover { background-color: #45a049; }\
    .button-off { background-color: #f44336; }\
    .button-off:hover { background-color: #d32f2f; }\
    .button-all { background-color: #ff9800; }\
    .button-all:hover { background-color: #f57c00; }\
  </style>\
</head>\
<body>\
  <i class=\"fas fa-truck truck\"></i>\
  <h1>Controle da Iluminação</h1>\
  <div class=\"button-container\">\
    %BUTTONS%\
  </div>\
  <button id=\"toggle-all-button\" class=\"button button-all\" onclick=\"toggleAllRelays()\">Liga/Desliga Tudo</button>\
</body>\
<script src=\"https://cdnjs.cloudflare.com/ajax/libs/font-awesome/5.15.3/js/all.min.js\"></script>\
<script>\
  async function toggleRelay(relayIndex) {\
    const response = await fetch(`/toggle?relay=${relayIndex}`);\
    if (response.ok) {\
      const result = await response.json();\
      console.log(`Relé ${relayIndex + 1} ${result.state ? 'ligado' : 'desligado'}`);\
    } else {\
      console.error('Falha ao alterar o estado do relé');\
    }\
  }\
\
  async function toggleAllRelays() {\
    const response = await fetch('/toggle-all');\
    if (response.ok) {\
      const result = await response.json();\
      console.log(`Todos os relés ${result.state ? 'ligados' : 'desligados'}`);\
    } else {\
      console.error('Falha ao alterar o estado de todos os relés');\
    }\
  }\
</script>\
</html>";

AsyncWebServer server(80);

void setup() {
  // Inicialização da comunicação serial
  Serial.begin(115200);

  // Conexão com a rede Wi-Fi
  WiFi.begin(ssid, password);
  Serial.print("Conectando ao Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.println("Conexão Wi-Fi estabelecida");
  Serial.print("Endereço IP: ");
  Serial.println(WiFi.localIP());

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

  // Configuração das rotas do servidor web
  String buttonsHtml;
  for (uint8_t i = 0; i < 16; i++) {
    buttonsHtml += "<button class=\"button button-on\" onclick=\"toggleRelay(" + String(i) + ")\">Ligar/Desligar Relé " + String(i + 1) + "</button>";
  }
  String finalHtmlPage = String(htmlPage);
  finalHtmlPage.replace("%BUTTONS%", buttonsHtml);

  server.on("/", HTTP_GET, [finalHtmlPage](AsyncWebServerRequest *request){
    request->send(200, "text/html", finalHtmlPage);
  });

  server.on("/toggle", HTTP_GET, [](AsyncWebServerRequest *request){
    if (request->hasParam("relay")) {
      int relayIndex = request->getParam("relay")->value().toInt();
      toggleRelay(relayIndex);
      bool state = mcp.digitalRead(relayIndex);
      request->send(200, "application/json", "{\"state\": " + String(state ? 1 : 0) + "}");
    } else {
      request->send(400);
    }
  });

  server.on("/toggle-all", HTTP_GET, [](AsyncWebServerRequest *request){
    toggleAllRelays();
    bool state = checkAllRelaysState();
    request->send(200, "application/json", "{\"state\": " + String(state ? 1 : 0) + "}");
  });

  // Inicialização do servidor web
  server.begin();

  Serial.println("Servidor web iniciado");
}

void loop() {
  // Nenhum código adicional necessário no loop
}

void toggleRelay(int relayIndex) {
  if (relayIndex >= 0 && relayIndex < 16) {
    bool currentState = mcp.digitalRead(relayIndex);
    mcp.digitalWrite(relayIndex, !currentState);
  }
}

void toggleAllRelays() {
  bool allOn = checkAllRelaysState();
  for (uint8_t i = 0; i < 16; i++) {
    mcp.digitalWrite(i, allOn ? LOW : HIGH);
  }
}

bool checkAllRelaysState() {
  for (uint8_t i = 0; i < 16; i++) {
    if (mcp.digitalRead(i)) {
      return true;
    }
  }
  return false;
}
