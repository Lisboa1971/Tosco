A#include <WiFi.h>
#include <WebServer.h>

// Substitua pelas suas credenciais de Wi-Fi
const char* ssid = "CONTA";
const char* password = "SENHA";

// Configurações do servidor web
WebServer server(80);

// Pinos para controlar os relés
const int relayPins[] = {12, 13, 25, 26};
const int numRelays = sizeof(relayPins) / sizeof(relayPins[0]);

// Descrições dos relés
const char* relayDescriptions[] = {"Iluminação do Quarto", "Iluminação da Sala", "Iluminação da Cozinha", "Iluminação do Deck"};

void handleRoot() {
  String html = "<html><head>";
  html += "<meta charset=\"UTF-8\">";
  html += "<link href=\"https://maxcdn.bootstrapcdn.com/font-awesome/4.7.0/css/font-awesome.min.css\" rel=\"stylesheet\">";
  html += "<style>body{font-family: 'Comic Sans MS', cursive;text-align: center;margin: 0;padding: 20px;background-color: #f5f5f5;}.container{max-width: 400px;margin: 0 auto;}.relay{background-color: #fff;border-radius: 4px;box-shadow: 0 2px 4px rgba(0, 0, 0, 0.1);padding: 20px;margin-bottom: 10px;text-align: left;}.relay h3{margin: 0 0 10px;color: #333;}.relay a{display: inline-block;margin-right: 10px;padding: 10px 20px;border-radius: 4px;text-decoration: none;font-weight: bold;}.relay .button-on{background-color: #4CAF50;color: #fff;}.relay .button-off{background-color: #F44336;color: #fff;}.truck-icon{float: left;margin-top: 10px;margin-right: 10px;}</style>";
  html += "</head><body>";
  html += "<div class=\"container\">";
  html += "<h1 style=\"font-family: 'Comic Sans MS', cursive;\"><i class=\"fa fa-truck truck-icon\"></i> Controle da Iluminação</h1>";

  for (int i = 0; i < numRelays; i++) {
    html += "<div class=\"relay\">";
    html += "<h3>" + String(relayDescriptions[i]) + "</h3>";
    if (digitalRead(relayPins[i]) == HIGH) {
      html += "<a href=\"/toggle?relay=" + String(i) + "\" class=\"button-off\"><i class=\"fa fa-lightbulb-o\"></i> Desligar</a>";
    } else {
      html += "<a href=\"/toggle?relay=" + String(i) + "\" class=\"button-on\"><i class=\"fa fa-lightbulb-o\"></i> Ligar</a>";
    }
    html += "</div>";
  }

  // Botão "Ligar Tudo"
  html += "<div class=\"relay\">";
  html += "<h3>Ligar Tudo</h3>";
  html += "<a href=\"/toggleAll?state=1\" class=\"button-on\"><i class=\"fa fa-power-off\"></i> Ligar</a>";
  html += "<a href=\"/toggleAll?state=0\" class=\"button-off\"><i class=\"fa fa-power-off\"></i> Desligar</a>";
  html += "</div>";

  html += "</div></body></html>";
  server.send(200, "text/html", html);
}

void handleToggle() {
  String relayIndexStr = server.arg("relay");
  if (relayIndexStr != "") {
    int relayIndex = relayIndexStr.toInt();
    toggleRelay(relayIndex);
  }
  server.sendHeader("Location", "/");
  server.send(302);
}

void handleToggleAll() {
  String stateStr = server.arg("state");
  if (stateStr != "") {
    int state = stateStr.toInt();
    if (state == 1) {
      // Ligar todos os relés
      for (int i = 0; i < numRelays; i++) {
        digitalWrite(relayPins[i], LOW);
      }
    } else if (state == 0) {
      // Desligar todos os relés
      for (int i = 0; i < numRelays; i++) {
        digitalWrite(relayPins[i], HIGH);
      }
    }
  }
  server.sendHeader("Location", "/");
  server.send(302);
}

void setup() {
  // Inicialização do Serial para depuração
  Serial.begin(115200);

  // Conexão Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Conectando ao WiFi...");
  }

  Serial.println("Conectado ao Wi-Fi!");

  // Configuração dos pinos dos relés
  for (int i = 0; i < numRelays; i++) {
    pinMode(relayPins[i], OUTPUT);
    digitalWrite(relayPins[i], HIGH); // Inicia com os relés desligados
  }

  // Configuração do servidor web
  server.on("/", HTTP_GET, handleRoot);
  server.on("/toggle", HTTP_GET, handleToggle);
  server.on("/toggleAll", HTTP_GET, handleToggleAll);

  // Obtém o endereço IP atribuído
  IPAddress ip = WiFi.localIP();
  Serial.print("Endereço IP: ");
  Serial.println(ip);

  // Inicia o servidor
  server.begin();
}

void loop() {
  server.handleClient();
}

void toggleRelay(int relayIndex) {
  if (relayIndex >= 0 && relayIndex < numRelays) {
    digitalWrite(relayPins[relayIndex], !digitalRead(relayPins[relayIndex]));
  }
}
