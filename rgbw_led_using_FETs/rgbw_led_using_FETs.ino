#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <secrets.h>

ESP8266WebServer server(80);

#define RED 15
#define GREEN 13
#define BLUE 12
#define WHITE 14

void setup() {
  Serial.begin(115200);

  //setup pins
  pinMode(RED, OUTPUT);
  pinMode(GREEN, OUTPUT);
  pinMode(BLUE, OUTPUT);
  pinMode(WHITE, OUTPUT);
  allOff();

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
  server.on("/healthcheck", healthCheck);
  server.on("/rgbw", rgbw);
  server.on("/off", allOff); 
  
  server.begin();
  Serial.println ( "HTTP server started" );

  test();
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
  mainPage +=   "<h1 style=\"font-size: x-large;\">RGBW LED Strip Interface</h1>";
  mainPage +=   "<form method=\"post\" action=\"/rgbw\" target=\"dummyframe\">";
  mainPage +=     "<label style=\"font-size: x-large;\"> Choose a Color";
  mainPage +=       "<input type=\"color\" name=\"color\" /> ";
  mainPage +=     "</label>";
  mainPage +=     "<br />";
  mainPage +=     "<label style=\"font-size: x-large;\">White";
  mainPage +=       "<input type=\"number\" name=\"white\" /> ";
  mainPage +=     "</label>";
  mainPage +=     "<br />";
  mainPage +=     "<br />";
  mainPage +=     "<input type=\"submit\" />";
  mainPage +=   "</form>";
  //all off
  mainPage +=   "<form method=\"post\" action=\"/off\" target=\"dummyframe\">";
  mainPage +=     "<button type=\"submit\">Off</button>";
  mainPage +=   "</form>";
  //self test button
  mainPage +=   "<form method=\"post\" action=\"/selftest\" target=\"dummyframe\">";
  mainPage +=     "<h2>Self Test</h2>";
  mainPage +=     "<br />";
  mainPage +=     "<div>Red, green, blue, white for 1 second each then off.</div>";
  mainPage +=     "<br />";
  mainPage +=     "<input type=\"submit\" />";
  mainPage +=   "</form>";
  //dummy frame to block redirect on form submission
  mainPage +=     "<iframe name=\"dummyframe\" id=\"dummyframe\" style=\"display:none;\"></iframe>";
  mainPage += "</body>";
  mainPage += "</html>";

  server.send(200, "text/html", mainPage);
}

void test() {
  R(255);
  delay(3000);
  R(0);
  
  G(255);
  delay(3000);
  G(0);
  
  B(255);
  delay(3000);
  B(0);
  
  W(255);
  delay(3000);
  W(0);

  allOff();
}

void allOff() {
  R(0);
  G(0);
  B(0);
  W(0);
}

void rgbw() {
  allOff();
  
  String colorString = server.arg("color");
  Serial.print("color selected: ");
  Serial.println(colorString);
  
  char *ptr;
  Serial.println("Individual values");
  String redString = "0x"+colorString.substring(1, 3);
  Serial.println(redString);
  String greenString = "0x"+colorString.substring(3, 5);
  Serial.println(greenString);
  String blueString = "0x"+colorString.substring(5, 7);
  Serial.println(blueString);
  
  int red = strtoul(redString.c_str(), &ptr, 16);
  Serial.print("Red: ");
  Serial.println(red);
  int green = strtoul(greenString.c_str(), &ptr, 16);
  Serial.print("Green: ");
  Serial.println(green);
  int blue = strtoul(blueString.c_str(), &ptr, 16);
  Serial.print("Blue: ");
  Serial.println(blue);
  String whiteString = server.arg("white");
  int white = strtoul(whiteString.c_str(), &ptr, 16);
  Serial.print("White: ");
  Serial.println(white);
  
  R(red);
  G(green);
  B(blue);
  W(white);

  server.send(200);
}

void R(int intensity){
  analogWrite(RED, intensity);
}

void G(int intensity){
  analogWrite(GREEN, intensity);
}

void B(int intensity){
  analogWrite(BLUE, intensity);
}

void W(int intensity){
  analogWrite(WHITE, intensity);
}
