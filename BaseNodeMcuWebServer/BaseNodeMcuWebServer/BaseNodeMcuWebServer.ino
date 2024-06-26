#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <secrets.h>

ESP8266WebServer server(80);
String deviceHostname = "nodemcu";

void setup() {
  Serial.begin(115200);

  //setup wifi
  Serial.println();
  Serial.print("Configuring access point...");
  // setting hostname has an order of operation or it wont work. dont change the next 3 lines
  WiFi.mode(WIFI_STA);
  WiFi.hostname(deviceHostname.c_str());
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  // end hostname setup
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  Serial.print("Device hostname: ");
  Serial.println(WiFi.hostname());
  //setup node server
  server.enableCORS(true);
  server.on ( "/", handleRoot);
  server.on("/healthcheck", healthCheck);
  server.onNotFound(notFound);
  
  server.begin();
  Serial.println ( "HTTP server started" );
}
void loop() {
  server.handleClient();
}
void healthCheck() {
  server.send(200, "text", "I'm not a teapot!");
}
void notFound() {
  String page = "<html>";
  page += "<head><title>NodeMcu - Page not found</title></head>";
  page += "<body><h1>Page Not Found</h1></body>";
  page += "</html>"; 
  server.send(404, "text/html", page);
}
void handleRoot() {
  String mainPage = "<html>";
  mainPage += "<head>";
  mainPage +=   "<title>NodeMCU</title>";
  mainPage +=   "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">";
  mainPage += "</head>";
  mainPage += "<body style=\"display: flex; flex-direction: column; align-items: center;\">";
  mainPage +=   "<h1 style=\"font-size: x-large;\">NodeMCU basic page</h1>";
  mainPage +=     "<p>Add your content here</p>";
  //dummy frame to block redirect on form submission
  mainPage +=     "<iframe name=\"dummyframe\" id=\"dummyframe\" style=\"display:none;\"></iframe>";
  mainPage += "</body>";
  mainPage += "</html>";

  server.send(200, "text/html", mainPage);
}
