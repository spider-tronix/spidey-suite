/*
**************************************************************************************
                       Spideydude for ESP32
                   ================================
   Filename: Spideydude.ino
   Version: 0.9.0
   Date: October 19, 2020
   Authors: Aditya Kumar Singh
***************************************************************************************
*/
// Including essential library files
#include <stdint.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <ESPmDNS.h>

#include "spideydude.h"
#include "spideyserver.h"
#include "HexParser.h"

/*
   ==============================================================
                       USER CHANGEABLE AREA START
                USER SHOULD CHANGE THIS PART OF THE CODE ONLY
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
   SSID = Spideydude
   Password = spider.123
*/
#if (OTA_WIFI_MODE == 0)
  const char* ssid = "Spideydude";
  const char* password = "spider.123";
#endif

const char* host = "spideydude";                                  // Host name for mDNS server

WebServer server(80);                                            // Creating WebServer object on port 80

String resp = "Some error occured. Please re-try";               // Response from spideydude will be stored here 
uint8_t file[32768] PROGMEM;
uint8_t prog[32768] PROGMEM;
int file_index = 0;
String fileName = "";

void flashProgram(){
  Serial.print(F("\nUploading Success: "));Serial.print(file_index);Serial.println(F(" bytes transferred.\n"));
  for(int i=0;i<file_index;i++){
    Serial.write(file[i]);
  }
  // Call the HexParser to parse the file first
  size_t len=0;
  Parser(file, file_index, &len, prog);
  // Call the Spideydude here to upload the file
  Serial.println(F("\nStarting Spideydude......\n\n"));
  for(int i=0;i<200;i++){
    Serial.print(" ");
  }
  Spideydude spideydude;
  uint16_t page = 0x0080;
  resp = spideydude.begin(fileName, prog, len, page);
  file_index = 0;
}

void setup(void) {
  Serial.begin(115200);
  Serial.println("");
  #ifdef OTA_WIFI_MODE
    #if (OTA_WIFI_MODE == 0)
      Serial.println("Setting AP (Access Point)..");
      WiFi.softAP(ssid, password);
      IPAddress IP = WiFi.softAPIP();
      Serial.print(F("Network SSID: "));Serial.println(ssid);
      Serial.print(F("Network Password: "));Serial.println(password);
      Serial.print(F("AP IP address: ")); Serial.println(IP);
    #elif (OTA_WIFI_MODE == 1)
      Serial.print(F("Connecting to WiFi"));
      WiFi.begin(ssid, password);
      while (WiFi.status() != WL_CONNECTED) {
        delay(250);
        Serial.print(F("."));
      }
      Serial.println(F(""));
      Serial.print(F("Connected to ")); Serial.println(ssid);
      Serial.print(F("IP address: ")); Serial.println(WiFi.localIP());
    #else
      #error Un-identfied WiFi Mode
    #endif
  #else
    #error WiFi mode not defined
  #endif
  /* Using mdns for host name resolution */
  if (!MDNS.begin(host)) {                                 
    Serial.println(F("Error setting up MDNS responder!"));
    while (1) {
      delay(1000);
    }
  }
  Serial.print(F("mDNS responder started.\nServing at http://")); Serial.print(host); Serial.println(F(".local      on Port 80"));
  
  // Route for root / web page
  server.on("/", HTTP_GET, []() {
    server.sendHeader("Connection", "close");
    server.send(200, "text/html", root());
  });
  // Route to /upload handler for GET Request
  server.on("/upload", HTTP_GET, []() {
    server.sendHeader("Connection", "close");
    server.send(404);
  });
  // Route to /upload handler for POST Request
  server.on("/upload", HTTP_POST, []() {
    flashProgram();
    server.sendHeader("Connection", "close");
    server.send(200, "text/plain", resp);
  }, []() {
    HTTPUpload& upload = server.upload();
    if (upload.status == UPLOAD_FILE_START) {
      Serial.printf("\nIncoming file: %s\n", upload.filename.c_str());
      Serial.println(F("Reading file......"));
    }
    else if (upload.status == UPLOAD_FILE_WRITE) {
      for (int i = 0; i < upload.currentSize; i++) {
        file[file_index++] = upload.buf[i];
      }
    }
    else if (upload.status == UPLOAD_FILE_END) {
      fileName = upload.filename;
    }
  });
  // Route to handle not found
  server.onNotFound([]() {
    server.sendHeader("Connection", "close");
    server.send(404);
  });
  // Start server
  server.begin();
}

void loop(void) {
  server.handleClient();
  delay(500);
}
