#include <ESP.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include "capreravr.h"
#include "index.h"
#include "WiFiConfig.h"
#include "CapreraUtilities.h"


//IPAddress ip(192, 168, 1, 9);

ESP8266WebServer server(80);

const int LED_PIN = 13;

boolean handleCommands() {
  int command = 0;
  int track = 0;
  if (!server.arg("track").equals("")) {
    track = server.arg("track").toInt();
  }

  if (server.arg("volume").equals("up")) {
    command = VOLUME_UP;
  }else if (server.arg("volume").equals("down")) {
    command = VOLUME_DOWN;
  }else if (server.arg("button").equals("1")) {
    command = BUTTON1 | (track << 4);
  }else if (server.arg("button").equals("2")) {
    command = BUTTON2 | (track << 4);
  }else if (server.arg("button").equals("3")) {
    command = BUTTON3 | (track << 4);
  }else if (server.arg("button").equals("4")) {
    command = BUTTON4 | (track << 4);
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

  server.send(200, "text/html", INDEX);
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
  Serial.print("Connecting..");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    if (WiFi.status() == WL_NO_SSID_AVAIL) {
      Serial.println("SSID not available...");
      //ESP.reset();
    }
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
