#include <ESP8266WiFiMulti.h> 
#include <ESP8266mDNS.h>
#include <ESP8266WebServer.h>
#include<SPI.h>

ESP8266WiFiMulti wifiMulti;     
ESP8266WebServer server(80);    

String global_buffer="000000n";

bool power_on = false;

void handleRoot();           
void handlePower();
void handleNotFound();

void values_to_slave()
{
  for(int i=0; i< sizeof global_buffer; i++)  
    SPI.transfer(global_buffer[i]);
}

void setup(void){
  Serial.begin(9600);       
  delay(10);
  SPI.begin();       
  Serial.println('\n');

  wifiMulti.addAP("", "");   
//  wifiMulti.addAP("ssid_from_AP_3", "your_password_for_AP_3");

  Serial.println("Connecting ...");
  int i = 0;
  while (wifiMulti.run() != WL_CONNECTED) { 
    delay(250);
    Serial.print('.');
  }
  Serial.println('\n');
  Serial.print("Connected to ");
  Serial.println(WiFi.SSID());           
  Serial.print("IP address:\t");
  Serial.println(WiFi.localIP());       

  if (MDNS.begin("esp8266")) {              
    Serial.println("mDNS responder started");
  } else {
    Serial.println("Error setting up MDNS responder!");
  }

  server.on("/", HTTP_GET, handleRoot);  
  server.on("/POWER", HTTP_POST, handlePower); 
  server.onNotFound([](){
    server.send(404, "text/plain", "404: Not found");
  });
  server.begin();                           
  Serial.println("HTTP server started");
  values_to_slave();
}

void loop(void){
  server.handleClient();        
}

void handleRoot() { 
  server.send(200, "text/html", "<form action=\"/POWER\" method=\"POST\"><input type=\"text\" name=\"power\" placeholder=\"Power\"></br><input type=\"submit\" value=\"Send power\"></form>");
}

bool check_buffer(){
  if(global_buffer == "auton") return true;
  if(sizeof global_buffer > 50) return false;
  Serial.println(sizeof global_buffer);
  if(global_buffer[0] == '-')
  {
    for(int i = 1; i < 3; i++)
    {
      if(isDigit(global_buffer[i]) == false) return false;
    }
    if(global_buffer[4] == '-')
    {
      for(int i = 5; i < 7; i++)
      {
        if(isDigit(global_buffer[i]) == false) return false;
      }
      if(global_buffer[8] != 'n') return false;
    }
  }
  else
  {
    for(int i = 0; i < 2; i++)
    {
      if(isDigit(global_buffer[i]) == false) return false;
    }
    if(global_buffer[3] == '-')
    {
      for(int i = 4; i < 6; i++)
      {
        if(isDigit(global_buffer[i]) == false) return false;
      }
      if(global_buffer[7] != 'n') return false;
    }
  }
  return true;
}

void handlePower() {                         
  if( ! server.hasArg("power") || server.arg("power") == NULL) { 
    server.send(400, "text/plain", "400: Invalid Request");         
    server.send(400, "text/plain", server.arg("power"));
    return;
  }
  global_buffer = server.arg("power");
  Serial.println(global_buffer);
  
  if(check_buffer() == true)
  {
    server.send(303, "text/plain", "ok!");
    values_to_slave();
  }  
  else
  {
    server.send(400, "text/plain", "400: Invalid Format");
    Serial.println("Invalid Format");
  }
    
  return;
}
