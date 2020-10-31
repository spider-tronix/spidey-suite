/*
**************************************************************************************
                       Spideydude for ESP32
                   ================================
   Filename: Spideydude.ino
   Version: 0.8.0
   Date: October 19, 2020
   Authors: Aditya Kumar Singh
***************************************************************************************
*/

// Including essential library files
#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <ESPmDNS.h>

#include "spideydude.h"
#include "spideyserver.h"

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

void setup(void) {
  Serial.begin(115200);
  Serial.println("");
  #ifdef OTA_WIFI_MODE
    #if (OTA_WIFI_MODE == 0)
      Serial.println("Setting AP (Access Point)..");
      WiFi.softAP(ssid, password);
      IPAddress IP = WiFi.softAPIP();
      Serial.print("Network SSID: ");Serial.println(ssid);
      Serial.print("Network Password: ");Serial.println(password);
      Serial.print("AP IP address: "); Serial.println(IP);
    #elif (OTA_WIFI_MODE == 1)
      Serial.print("Connecting to WiFi");
      WiFi.begin(ssid, password);
      while (WiFi.status() != WL_CONNECTED) {
        delay(250);
        Serial.print(".");
      }
      Serial.println("");
      Serial.print("Connected to "); Serial.println(ssid);
      Serial.print("IP address: "); Serial.println(WiFi.localIP());
    #else
      #error Un-identfied WiFi Mode
    #endif
  #else
    #error WiFi mode not defined
  #endif
  /* Using mdns for host name resolution */
  if (!MDNS.begin(host)) {                                 
    Serial.println("Error setting up MDNS responder!");
    while (1) {
      delay(1000);
    }
  }
  Serial.print("mDNS responder started.\nServing at http://"); Serial.print(host); Serial.println(".local      on Port 80");
  
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
    server.sendHeader("Connection", "close");
    server.send(200, "text/plain", resp);
  }, []() {
    HTTPUpload& upload = server.upload();
    if (upload.status == UPLOAD_FILE_START) {
      Serial.printf("\nIncoming file: %s\n", upload.filename.c_str());
    }
    else if (upload.status == UPLOAD_FILE_WRITE) {
      Serial.println("Reading file......");
    }
    else if (upload.status == UPLOAD_FILE_END) {
      for (int i = 0; i < upload.totalSize; i++) {
        Serial.write(upload.buf[i]);
      }
      Serial.println("");
      Serial.printf("\nUploading Success: %d bytes transferred.\n", upload.totalSize);
      Serial.println("Starting Spideydude......\n\n");
      // Call the Spideydude here to upload the file
      Spideydude spideydude;
      uint16_t page = 0x0080;
      resp = spideydude.begin(upload.filename, upload.buf, upload.totalSize, page);
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
