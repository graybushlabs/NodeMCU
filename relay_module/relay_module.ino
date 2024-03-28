#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <secrets.h>

#define RELAY_PIN 14

bool isRelayOn = false;

ESP8266WebServer server(80);

void setup() {
  Serial.begin(115200);
  //setup relay and turn off
  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, LOW);
  
  //setup wifi
  Serial.println();
  Serial.print("Configuring access point...");
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
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
  server.on("/relaytoggle", relaytoggle);
  server.on("/healthcheck", healthCheck);
  
  server.begin();
  Serial.println ( "HTTP server started" );
}
void loop() {
  server.handleClient();
}
void healthCheck() {
  server.send(200, "text", "I'm not a teapot!");
}
void handleRoot() {
  String mainPage = "<html>";
  mainPage += "<head>";
  mainPage +=   "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">";
  mainPage += "</head>";
  mainPage += "<body style=\"display: flex; flex-direction: column; align-items: center;\">";
  mainPage +=   "<h1 style=\"font-size: x-large;\">NodeMCU Relay Control</h1>";
  mainPage +=   "<form method=\"post\" action=\"/relaytoggle\" target=\"dummyframe\">";
  
  mainPage +=     "<label style=\"font-size: x-large;\"> Toggle Relay";
  mainPage +=       "<input type=\"checkbox\" name=\"relaystate\" disabled";
  mainPage +=       isRelayOn ? " checked " : " ";
  mainPage +=       "/> ";
  mainPage +=     "</label>";
  mainPage +=     "<br />";
  mainPage +=     "<br />";
  mainPage +=     "<input type=\"submit\" />";
  mainPage +=   "</form>";
  //dummy frame to block redirect on form submission
  mainPage +=     "<iframe name=\"dummyframe\" id=\"dummyframe\" style=\"display:none;\"></iframe>";
  mainPage += "</body>";
  mainPage += "</html>";

  server.send(200, "text/html", mainPage);
}

String relayState(bool relayOn) {
  if(relayOn){
    return "On";
  }
  return "Off";
}

void relaytoggle() {
  Serial.print("Relay state is currently ");
  Serial.println(relayState(isRelayOn));
  Serial.print("You asked for it to be ");
  Serial.println(relayState(!isRelayOn));
  
  if(isRelayOn){
   Serial.println("Turning off");
   isRelayOn = false;
   digitalWrite(RELAY_PIN, LOW);
  } else {
   Serial.println("Turning on");
   isRelayOn = true;
   digitalWrite(RELAY_PIN, HIGH); 
  }
  
  Serial.print("Relay state is now ");
  Serial.println(relayState(isRelayOn));
  
  handleRoot();
}
