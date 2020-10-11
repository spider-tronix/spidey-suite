#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <SPIFFS.h>
#include <WiFiClient.h>
#include <ESPmDNS.h>

/*
 * USER CHANGEABLE AREA START
 * USER SHOULD CHANGE THIS PART OF CODE ONLY 
 * Replace with your network credentials
*/
const char* ssid = "MaataRaani";
const char* password = "698630LAK";
/*
 * USER CHANGEABLE AREA ENDED  
*/
const char* host = "spiderOTA";

AsyncWebServer server(80); // Create AsyncWebServer object on port 80

void handleFileUpload(AsyncWebServerRequest *request, String filename, size_t index, uint8_t *data, size_t len, bool final){
  Serial.println("\n\n============== Starting Upload ==============");
  if(!index){
    Serial.printf("File Name: %s\n", filename.c_str());
  }
  Serial.println("File Contents: ");
  for(size_t i=0; i<len; i++){
    Serial.write(data[i]);
  }
  if(final){
    Serial.printf("Size of the file %s is %u Bytes\n", filename.c_str(), index+len);
  }
  Serial.println("============== Upload Ended ==============\n\n");
}

void setup(){
  Serial.begin(115200);
  Serial.println("\n");
  if(!SPIFFS.begin(true)){          // Initialize SPIFFS
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }
  Serial.print("Connecting to WiFi");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");Serial.println(ssid);
  Serial.print("IP address: ");Serial.println(WiFi.localIP());
  /*use mdns for host name resolution*/
  if (!MDNS.begin(host)) {                  
    Serial.println("Error setting up MDNS responder!");
    while (1) {
      delay(1000);
    }
  }
  Serial.print("mDNS responder started. Serving at http://");Serial.print(host);Serial.println(".local");
  // Route for favicon icon
  server.on("/favicon.ico", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/favicon.ico", "image/ico");
  });
  // Route for root / web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/index.htm", String());
  });
  // Route to load css file
  server.on("/index.css", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/index.css", "text/css");
  });
  // Route to upload /upload handler 
  server.on("/upload", HTTP_POST, [](AsyncWebServerRequest *request) {
    request->send(200, "File Uploaded Succesfully", "text/plain");
  },[](AsyncWebServerRequest *request, const String& filename, size_t index, uint8_t *data,size_t len, bool final){
      handleFileUpload(request, filename, index, data, len, final);
    }
  );
  // Start server
  server.begin();
  Serial.println("Server Started.");
}
 
void loop(){
  
}
