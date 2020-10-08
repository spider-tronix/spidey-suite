#include<WiFi.h>
#include<WebServer.h>

const char* ssid = "OTA v1.0.0";
const char* password = "spider.123";

IPAddress local_ip(192, 168, 1, 1);
IPAddress gateway(192, 168, 1, 1);
IPAddress subnet(255, 255, 255, 0);

WebServer server(80);

void root() {
  Serial.println("Root Page served.");
  server.send(200, "text/html", Render());
}

void handle_NotFound() {
  Serial.println("Error 404");
  server.send(404, "text/plain", "Not found");
}

String Render() {
  String DOM = "";
  return DOM;
}

void setup() {
  Serial.begin(115200);

  WiFi.softAP(ssid, password);
  WiFi.softAPConfig(local_ip, gateway, subnet);

  server.on("/", root);
  server.onNotFound(handle_NotFound);
  server.begin();

  Serial.println("HTTP server started on port 80");
}

void loop() {
  server.handleClient();
}
