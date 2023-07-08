#include <WiFi.h>
#include <WebServer.h>

const char* ssid = "CLARO_2G872534";
const char* password = "D4872534";

WebServer server(80);

const int relay1Pin = 12;
const int relay2Pin = 13;

void setupRelays() {
  pinMode(relay1Pin, OUTPUT);
  pinMode(relay2Pin, OUTPUT);

  digitalWrite(relay1Pin, HIGH);
  digitalWrite(relay2Pin, HIGH);
}

void handleRoot() {
  String html = "<html><head></head><body>";
  html += "<h1>Controle da Cabine</h1>";

  html += "<h3>Sobe Vidro Motorista:</h3>";
  html += "<p><a href=\"/toggle1\" onMouseDown=\"toggleRelay(1, true)\" onMouseUp=\"toggleRelay(1, false)\">Acionar Sobe Vidro Motorista</a></p>";

  html += "<h3>Desce Vidro Motorista:</h3>";
  html += "<p><a href=\"/toggle2\" onMouseDown=\"toggleRelay(2, true)\" onMouseUp=\"toggleRelay(2, false)\">Acionar Desce Vidro Motorista</a></p>";

  html += "<script>";
  html += "function toggleRelay(relay, state) {";
  html += "  var xhr = new XMLHttpRequest();";
  html += "  if (state) {";
  html += "    xhr.open('GET', '/toggle' + relay + '?state=1', true);";
  html += "  } else {";
  html += "    xhr.open('GET', '/toggle' + relay + '?state=0', true);";
  html += "  }";
  html += "  xhr.send();";
  html += "}";
  html += "</script>";

  html += "</body></html>";
  server.send(200, "text/html", html);
}

void handleToggle1() {
  if (server.arg("state") == "1") {
    digitalWrite(relay1Pin, LOW);
  } else {
    digitalWrite(relay1Pin, HIGH);
  }
  server.sendHeader("Location", "/");
  server.send(302);
}

void handleToggle2() {
  if (server.arg("state") == "1") {
    digitalWrite(relay2Pin, LOW);
  } else {
    digitalWrite(relay2Pin, HIGH);
  }
  server.sendHeader("Location", "/");
  server.send(302);
}

void setup() {
  Serial.begin(115200);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Conectando ao WiFi...");
  }

  Serial.println("Conectado ao Wi-Fi!");

  setupRelays();

  server.on("/", HTTP_GET, handleRoot);
  server.on("/toggle1", HTTP_GET, handleToggle1);
  server.on("/toggle2", HTTP_GET, handleToggle2);

  IPAddress ip = WiFi.localIP();
  Serial.print("Endereço IP: ");
  Serial.println(ip);

  server.begin();
}

void loop() {
  server.handleClient();
}
