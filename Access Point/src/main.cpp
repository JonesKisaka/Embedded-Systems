#include <Arduino.h>
#include<WiFi.h>
#include<WiFiClient.h>
#include<WiFiAP.h>

#define onDelay 500
#define offDelay 500
#define led 12

const char* ssid = "ESP32NIGGA";
const char* password = "ESP32NIGGA1";

WiFiServer server(80);

void setup() {
  // put your setup code here, to run once:
  pinMode(led, OUTPUT);
  Serial.begin(115200);
  Serial.println("We are on ma niggaa");
  Serial.println("Configuring Access point...");
//Access point configuration
  WiFi.softAP(ssid, password);
  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP Address: ");
  Serial.println(myIP);
  server.begin();

  Serial.println("Server started!");
}

void turn();

void loop() {
  // put your main code here, to run repeatedly:
  WiFiClient client = server.available();
  if (client){
    Serial.println("New Client.");
    String currentLine = "";
    while(client.connected()){
      if(client.available()){
        char c = client.read();
        Serial.write(c);

        if(c == '\n'){
          if(currentLine.length() == 0){
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println();

            client.println("Click <a href=\"/H\"> <button> <strong>ON</strong> </button> </a> to turn on the LED");
            client.println();
            client.println("Click <a href=\"/L\"> <button> <strong>OFF</strong>  </button></a> to turn off the LED");

            client.println();
            break;
          }
          else{
            currentLine = "";
          }
        }
        else if(c != '\r'){
          currentLine += c;
        }

        if(currentLine.endsWith("GET /H")){
          digitalWrite(led, HIGH);
        }
        else if(currentLine.endsWith("GET /L")){
          digitalWrite(led, LOW);
        }
        //turn();
      }
    }
  }
  

}


void turn(){
digitalWrite(led, HIGH);
Serial.println("We are on");
delay(onDelay);

digitalWrite(led, LOW);
Serial.println("We are off");
delay(offDelay);
}