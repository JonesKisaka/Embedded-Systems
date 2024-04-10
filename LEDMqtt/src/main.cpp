#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>
#define BUFFER_SIZE 60;

const char *ssid = "TECNO CAMON 12";
const char *password = "Jones11688";
const char* mqttServer = "192.168.43.5";
const char* inTopic = "ESP32/B14/Connection";
const char* outTopic = "ESP32/B14/Lights";
int led = 12;

WiFiClient espClient;
PubSubClient client(espClient);


void setupWifi(){
  //Connecting to local WiFi.
  Serial.print("Connecting to: \t");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  
  //check whether connection is secure.
  while(WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.print(".");
  }

  //Print out if the WiFi is connected.
  Serial.println("");
  Serial.println("Connected to: ");
  Serial.print(ssid);
  Serial.println(".\t");
  Serial.println("IP Address is");
  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte*message, unsigned int length){
  client.subscribe(inTopic);
  Serial.print("Message arrived");
  Serial.print(topic);
  
  for(int i=0; i<length; i++){
    Serial.print((char)message[i]);
  }
  Serial.println("");

  if(message[0] = '0'){
    digitalWrite(led, LOW);
    client.publish(outTopic, "The bulb is off");
  }
  else if(message[0] = '1'){
    digitalWrite(led, HIGH);
    client.publish(outTopic, "The bulb is on");
  }
}

void reconnect(){
  while(!client.connected()){
    Serial.print("Attempting MQTT connection...");
    String clientId = "Publisher MCU";
    clientId += String(random(0xffff), HEX);
      // Attempt to connect
      if (client.connect(clientId.c_str())) {
        Serial.println("connected");
        // Once connected, publish an announcement...
        client.publish("inTopic", "Started");
        // ... and resubscribe
        client.subscribe("inTopic");
      } else {
        Serial.print("failed, rc=");
        Serial.print(client.state());
        Serial.println(" try again in 5 seconds");
        // Wait 5 seconds before retrying
        delay(500);
      }
  }
}

void setup() {
  Serial.begin(115200);
  pinMode(led, OUTPUT);
  Serial.println("Program has started \n");
  setupWifi();
}

void loop() {
  if (!client.connected())  // Reconnect if MQTT connection is lost
    {
      reconnect();
    }
    client.loop();
  
}