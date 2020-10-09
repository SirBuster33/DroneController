#include <Arduino.h>
#include "WiFi.h"
#include "AsyncUDP.h"

// Create a hotspot on your phone and connect to it via your esp32. Remember to connect with your computer too!
const char * ssid = "A Monkey's Phone"; // Name of the network
const char * password = "Lucked6334";   // Password of the network

AsyncUDP udp;

// Note for what pins to use: The ADC2 analog pins get fucked by the wifi configuration. Use Pin 32-36 and 39 instead.
const int potentiometerPin = 36; 

/*const int ControllerXPin = 15;
const int ControllerYPin = 2;
const int ButtonPin = 4;


int checkDeadZone(int ControllerXValue, int ControllerYValue){
  
  int UpperRange = 40;
  int LowerRange = -40;
  boolean xIsHigh (ControllerXValue > UpperRange);
  boolean xIsLow (ControllerXValue < LowerRange);
  boolean yIsHigh (ControllerYValue > UpperRange);
  boolean yIsLow (ControllerYValue < LowerRange);

  
  if (xIsHigh) {
    ControllerXValue = map(ControllerXValue, UpperRange, 100, 0, 100);
  }
  else if (xIsLow) {
    ControllerXValue = map(ControllerXValue, LowerRange, -100, 0, -100);
  }
  else {
    ControllerXValue = 0;
  }

  if (yIsHigh) {
      ControllerYValue = map(ControllerYValue, UpperRange, 100, 0, 100);    
  }
  else if (yIsLow) {
          ControllerYValue = map(ControllerYValue, LowerRange, -100, 0, -100);
  }
  else {
    ControllerYValue = 0;
  }
}

void printButtonState(int ControllerXValue, int ControllerYValue, int ButtonPressed){
  Serial.print("The X-Value is: ");
  Serial.println(ControllerXValue);
  Serial.print("The Y-Value is: ");
  Serial.println(ControllerYValue);

  Serial.print("Button is: ");
  Serial.println(ButtonPressed);

  if (ButtonPressed == 0){
    Serial.println("BANG! You hit the button!");
  }
  else{
    Serial.println("Button is not pressed.");
  }
}

void printSpace(){
  Serial.println("");
  Serial.println("");
  Serial.println("Taking new measurement...");
  Serial.println("");
}*/

void setup(){

    // Controller Setup
    /*pinMode (ControllerXPin, INPUT);
    pinMode (ControllerYPin, INPUT);
    pinMode (ButtonPin, INPUT_PULLUP);*/

    pinMode (potentiometerPin, INPUT);

    Serial.begin(9600);

    // Wifi setup
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
    if (WiFi.waitForConnectResult() != WL_CONNECTED) {
        Serial.println("WiFi Failed");
        while(1) {
            delay(1000);
        }
    }
    if(udp.listen(4000)) {
        Serial.print("UDP Listening on IP: ");
        Serial.println(WiFi.localIP());
        udp.onPacket([](AsyncUDPPacket packet) {
            Serial.print("UDP Packet Type: ");
            Serial.print(packet.isBroadcast()?"Broadcast":packet.isMulticast()?"Multicast":"Unicast");
            Serial.print(", From: ");
            Serial.print(packet.remoteIP());
            Serial.print(":");
            Serial.print(packet.remotePort());
            Serial.print(", To: ");
            Serial.print(packet.localIP());
            Serial.print(":");
            Serial.print(packet.localPort());
            Serial.print(", Length: ");
            Serial.print(packet.length());
            Serial.print(", Data: ");
            Serial.write(packet.data(), packet.length());
            Serial.println();
            
            //reply to the client/sender
            packet.printf("Got %u bytes of data", packet.length());
        
        });
    }

        //Send unicast
        //udp.print("Hello Server!");
        //udp.
        
    }

    // sendMessage method using the last two digits of the IP and a port to send a message.
    // Remember to change the UDP Port in your Packet Sender application to receive the messages
    void sendMessage(String msg, int thirdIP, int fourthIP, int port){
    udp.writeTo((const uint8_t *)msg.c_str(), msg.length(),IPAddress(192, 168, thirdIP, fourthIP), port);
}


void loop()
{   
    delay(5000);

    int potentiometerValue = analogRead(potentiometerPin);
    double voltage = potentiometerValue / 4095.0 * 3.3;
    Serial.print("The voltage is set at: ");
    Serial.println(voltage);

  /*int ControllerXValue = map(analogRead(ControllerXPin), 0, 4095, -100, 100);
  int ControllerYValue = map(analogRead(ControllerYPin), 0, 4095, -100, 100);
  int ButtonPressed = digitalRead(ButtonPin);
  
  checkDeadZone(ControllerXValue, ControllerYValue);

  printButtonState(ControllerXValue, ControllerYValue, ButtonPressed);

  printSpace();  
  delay(2000);*/

    // Send broadcast on port 4000
    // udp.broadcastTo("Anyone here?", 4000);

    if (voltage >= (3.3/2)){
    sendMessage("V is high.", 43, 81, 333);
    } else{
    sendMessage("V is low.", 43, 81, 333);
    }

    // sendMessage("Hi Philipp, can you read this?", 43, 81, 333);
    //Serial.println("waiting for udp message...");
    // udp.writeTo((const uint8_t*)"Hi Philipp, can you read this?", 31, IPAddress(192,168,43,81), 54321); // Message, char count, IP, port
    // udp.writeTo((const uint8_t*)"Hi Hans, can you read this?", 27, IPAddress(192,168,43,249), 54321);
    // udp.writeTo((const uint8_t*)"Hi, you are still broadcasting. Could you comment it out?", 57, IPAddress(192,168,1,38), 54321);
}