#include <FastLED.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>

#define NUM_LEDS 1
#define DATA_PIN 14
#define CLOCK_PIN 12
CRGB leds[NUM_LEDS];

const char *ssid = "Frontier9600";
const char *password = "2331497786";
const char* host_name = "printer.rgbw";
ESP8266WebServer server(80);

void setup() {
  Serial.begin(115200);
  FastLED.addLeds<P9813, DATA_PIN, CLOCK_PIN, RGB>(leds, NUM_LEDS);

  //setup wifi
  Serial.println();
  Serial.print("Configuring access point...");
  WiFi.hostname(host_name);
  WiFi.begin(ssid, password);
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
  server.on("/selftest", selfTest);
  server.on("/off", off);
  server.begin();
  Serial.println ( "HTTP server started" );

  allOff();
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
  mainPage +=   "<h1>NodeMCU</h1>";
  mainPage +=   "<form method=\"post\" action=\"/rgbw\" target=\"dummyframe\">";
  mainPage +=     "<h1 style=\"font-size: x-large;\">RGBW LED Strip Interface</h1>";
  mainPage +=     "<label style=\"font-size: x-large;\"> Choose a Color";
  mainPage +=     "<input type=\"color\" name=\"color\" /> ";
  mainPage +=     "</label>";
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
  mainPage +=     "<div>Red, green, blue for 1 second each then off.</div>";
  mainPage +=     "<br />";
  mainPage +=     "<input type=\"submit\" />";
  mainPage +=   "</form>";
  //dummy frame to block redirect on form submission
  mainPage +=     "<iframe name=\"dummyframe\" id=\"dummyframe\" style=\"display:none;\"></iframe>";
  mainPage += "</body>";
  mainPage += "</html>";

  server.send(200, "text/html", mainPage);
}
void selfTest() {
  Serial.println("self test starting");

  allOff();
  delay(500);
  leds[0] = CRGB::Red;
  FastLED.show();
  delay(1000);
  leds[0] = CRGB::Green;
  FastLED.show();
  delay(1000);
  leds[0] = CRGB::Blue;
  FastLED.show();
  delay(1000);
  allOff();
  delay(500);
  
  Serial.println("self test complete");

  server.send(200, "text", "test complete");
}
void off() {
  allOff();
  
  server.send(200);
}
void allOff() {
  leds[0] = CRGB::Black;
  FastLED.show();
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
  
  leds[0] = CRGB(red, green, blue);
  FastLED.show();

  server.send(200);
}
