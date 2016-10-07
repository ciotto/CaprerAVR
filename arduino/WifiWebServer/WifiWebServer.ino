#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include "capreravr.h"
#include "WiFiConfig.h"
#include "CapreraUtilities.h"


//IPAddress ip(192, 168, 1, 9);

ESP8266WebServer server(80);

const int LED_PIN = 13;

boolean handleCommands() {
  int command = 0;
  if (server.arg("volume").equals("up")) {
    command = VOLUME_UP;
  }else if (server.arg("volume").equals("down")) {
    command = VOLUME_DOWN;
  }else if (server.arg("button").equals("1")) {
    command = BUTTON1;
  }else if (server.arg("button").equals("2")) {
    command = BUTTON2;
  }else if (server.arg("button").equals("3")) {
    command = BUTTON3;
  }else if (server.arg("button").equals("4")) {
    command = BUTTON4;
  }else if (server.arg("mode").equals("1")) {
    command = MODE_SWITCH | (1 << 4);
  }else if (server.arg("mode").equals("2")) {
    command = MODE_SWITCH | (2 << 4);
  }else if (server.arg("mode").equals("3")) {
    command = MODE_SWITCH | (3 << 4);
  }else if (server.arg("mode").equals("4")) {
    command = MODE_SWITCH | (4 << 4);
  }
  // Send command over Serial1
  if (command != 0) {
    char buffer[COMMANDS_SIZE + 1];
    
    Serial1.print(int2BitString(buffer, command, COMMANDS_SIZE + 1));
    
    return true;
  }

  return false;
}

void handleJson() {
  digitalWrite(LED_PIN, 1);

  boolean sent = handleCommands();
  
  server.send(200, "application/json", sent ? "true" : "false");
  digitalWrite(LED_PIN, 0);
}

void handleRoot() {
  digitalWrite(LED_PIN, 1);

  handleCommands();

  int bufferSize = 400;
  char* html =
"<html>\
  <head>\
    <title>CaprerAVR</title>\
    <style>\
      body { background-color: #ccc; font-family: Arial, Helvetica, Sans-Serif; color: #333; }\
      h1 { color: #c00; }\
      a { color: #333; text-decoration: none; }\
      a:hover { font-weight: bold; }\
    </style>\
  </head>\
  <body>\
    <center><h1>Hello from CaprerAVR!</h1></center>\
\
    <ul>\
      <li><a href=\"?volume=up\">Volume Up</a></li>\
      <li><a href=\"?volume=down\">Volume Down</a></li>\
    </ul>\
\
    <ul>\
      <li><a href=\"?button=1\">Button 1</a></li>\
      <li><a href=\"?button=2\">Button 2</a></li>\
      <li><a href=\"?button=3\">Button 3</a></li>\
      <li><a href=\"?button=4\">Button 4</a></li>\
    </ul>\
\
    <ul>\
      <li><a href=\"?mode=1\">Mode 1</a></li>\
      <li><a href=\"?mode=2\">Mode 2</a></li>\
      <li><a href=\"?mode=3\">Mode 3</a></li>\
      <li><a href=\"?mode=4\">Mode 4</a></li>\
    </ul>\
  </body>\
</html>";

  server.send(200, "text/html", html);
  digitalWrite(LED_PIN, 0);
}

void handleNotFound(){
  digitalWrite(LED_PIN, 1);
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET)?"GET":"POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i=0; i<server.args(); i++){
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }

  server.send(404, "text/plain", message);
  digitalWrite(LED_PIN, 0);
}

void setup(void){
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, 0);
  Serial.begin(115200);
  Serial1.begin(9600);
  WiFi.begin(SSID, PASSWORD);
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(SSID);
  Serial.print("URL: http://");
  Serial.print(WiFi.localIP());
  Serial.println("/");

  if (MDNS.begin("esp8266")) {
    Serial.println("MDNS responder started");
  }

  server.on("/", handleRoot);
  server.on("/json", handleJson);

  server.on("/inline", [](){
    server.send(200, "text/plain", "this works as well");
  });

  server.onNotFound(handleNotFound);

  server.begin();
  Serial.println("HTTP server started");
}

void loop(void){
  server.handleClient();
}
