#include <WiFi.h>
#include <HTTPClient.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebSrv.h>

const char* ssid = "CONTA"; // Nome da sua rede Wi-Fi
const char* password = "SENHA"; // Senha da sua rede Wi-Fi
const char* serverName = "http://exemplo.com/informar_estado.php"; // URL do arquivo PHP que receberá os dados

const int numEntradas = 34;

void handleRequest(AsyncWebServerRequest *request) {
  String mensagem = request->arg("mensagem");
  
  // Processar a mensagem recebida
  // Aqui você pode adicionar o código PHP para processar os dados recebidos do ESP32
  
  // Exemplo de resposta
  String resposta = "Mensagem recebida: " + mensagem;

  request->send(200, "text/plain", resposta);
}

AsyncWebServer server(80);

void setup() {
  Serial.begin(115200);
  delay(1000);

  // Conexão Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Conectando ao WiFi...");
  }

  Serial.println("Conexão Wi-Fi estabelecida!");

  // Configuração das rotas do servidor web
  server.on("/informar_estado", HTTP_POST, [](AsyncWebServerRequest *request){
    handleRequest(request);
  });

  // Inicia o servidor web
  server.begin();
}

void loop() {
  // Leitura das entradas digitais
  int estados[numEntradas];
  for (int i = 0; i < numEntradas; i++) {
    pinMode(i, INPUT);
    estados[i] = digitalRead(i);
  }

  // Envio dos dados para a página da Internet
  String mensagem;
  for (int i = 0; i < numEntradas; i++) {
    mensagem += "Entrada " + String(i) + ": " + (estados[i] ? "Fechada" : "Aberta") + "\n";
  }

  Serial.println("Enviando dados para a página da Internet...");
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    http.begin(serverName);
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");

    int httpCode = http.POST("mensagem=" + mensagem);

    if (httpCode > 0) {
      String response = http.getString();
      Serial.println("Resposta do servidor: " + response);
    } else {
      Serial.println("Falha na conexão com o servidor.");
    }

    http.end();
  }

  delay(5000); // Intervalo entre as leituras e envios de dados
}
