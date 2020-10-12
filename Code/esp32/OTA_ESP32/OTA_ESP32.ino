/*
**************************************************************************************
                        OTA Programming Module
                   ================================
   Filename: ServerFinal.ino
   Version: 0.1.0
   Date: October 12, 2020

   Authors: Aditya Kumar Singh
***************************************************************************************
*/

// Including essential library files
#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <SPIFFS.h>
#include <WiFiClient.h>
#include <ESPmDNS.h>

//#define DEBUG_MODE                             //Do not uncomment this line
/*
   ==============================================================
                       USER CHANGEABLE AREA START
                YOU SHOULD CHANGE THIS PART OF THE CODE ONLY
*/

// Select the WiFi Mode from Station Mode and Access Point(AP) Mode.
// 0 -> AP Mode, 1 -> Station Mode
// Default is AP Mode.
#define OTA_WIFI_MODE 0

/*
   If Wifi mode is selected as Station i.e 1, ESP32 will connect to
   one of the exisitng Wifi Network. Hence, provide the existing Wifi
   network credentials.
*/
#if (OTA_WIFI_MODE == 1)
const char* ssid = "NETWORK_SSID_HERE";
const char* password = "NETWORK_PASSWORD_HERE";
#endif
/*
                      USER CHANGEABLE AREA ENDED
   ==============================================================
*/

/*
   If Wifi mode is selected as AP i.e 0, ESP32 will create its
   own Wifi Network. The credentials will be-
   SSID = SpiderOTA
   Password = spider.123
*/
#if (OTA_WIFI_MODE == 0)
const char* ssid = "SpiderOTA";
const char* password = "spider.123";
#endif

const char* host = "spiderOTA";                                  // Host name for mDNS server

AsyncWebServer server(80);                                       // Create AsyncWebServer object on port 80

/*
   Function to handle File upload to the server.
*/
void handleFileUpload(AsyncWebServerRequest *request, String filename, size_t index, uint8_t *data, size_t len, bool final) {
  Serial.println("\n============== Starting Upload ==============");
  if (!index) {
    Serial.printf("File Name: %s\n", filename.c_str());
  }
  Serial.println("File Contents: ");
  for (size_t i = 0; i < len; i++) {
    Serial.write(data[i]);
  }
  if (final) {
    Serial.printf("Size of the file %s is %u Bytes\n", filename.c_str(), index + len);
  }
  Serial.println("============== Upload Ended ==============\n\n");
}

void setup() {
  Serial.begin(115200);
  #ifdef DEBUG_MODE
      Serial.println("\n");
  #endif
  if (!SPIFFS.begin(true)) {                                        // Initialize SPIFFS
    #ifdef DEBUG_MODE
      Serial.println("An Error has occurred while mounting SPIFFS");
    #endif
    return;
  }
  #ifdef OTA_WIFI_MODE
    #if (OTA_WIFI_MODE == 0)
      #ifdef DEBUG_MODE
        Serial.println("Setting AP (Access Point)…");
      #endif
      WiFi.softAP(ssid, password);
      IPAddress IP = WiFi.softAPIP();
      #ifdef DEBUG_MODE
        Serial.print("AP IP address: "); Serial.println(IP);
      #endif
    #elif (OTA_WIFI_MODE == 1)
      #ifdef DEBUG_MODE
        Serial.print("Connecting to WiFi");
      #endif
      WiFi.begin(ssid, password);
      while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        #ifdef DEBUG_MODE
          Serial.print(".");
        #endif
      }
      #ifdef DEBUG_MODE
        Serial.print(".");
        Serial.println("");
        Serial.print("Connected to "); Serial.println(ssid);
        Serial.print("IP address: "); Serial.println(WiFi.localIP());
      #endif
    #else
      #error Un-identfied WiFi Mode
    #endif
  #else
    #error WiFi mode not defined
  #endif
  // Using mDNS for host name resolution
  if (!MDNS.begin(host)) {
    #ifdef DEBUG_MODE
        Serial.println("Error setting up MDNS responder!");
    #endif
    while (1) {
      delay(500);
    }
  }
  #ifdef DEBUG_MODE
    Serial.print("mDNS responder started. Serving at http://"); Serial.print(host); Serial.println(".local");
  #endif
  
  // Route for favicon icon
  server.on("/favicon.ico", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send(SPIFFS, "/favicon.ico", "image/ico");
  });
  // Route for root / web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send(SPIFFS, "/index.htm", String());
  });
  // Route to load css file
  server.on("/index.css", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send(SPIFFS, "/index.css", "text/css");
  });
  // Route to upload /upload handler
  server.on("/upload", HTTP_POST, [](AsyncWebServerRequest * request) {
    request->send(200, "File Uploaded Succesfully", "text/plain");
  }, [](AsyncWebServerRequest * request, const String & filename, size_t index, uint8_t *data, size_t len, bool final) {
    handleFileUpload(request, filename, index, data, len, final);
  }
           );
  // Start server
  server.begin();
  #ifdef DEBUG_MODE
    Serial.println("Server Started.");
  #endif
  //To Show that Server started. 
  pinMode(2,OUTPUT);
  digitalWrite(2,HIGH);
}

void loop() {

}
