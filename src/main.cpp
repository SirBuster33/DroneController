#include <Arduino.h>
#include "WiFi.h"
#include "AsyncUDP.h"
#include <tuple>  // Allows to easily return multiple values, used in chechDeadZone.
                  // Source: https://stackoverflow.com/questions/321068/returning-multiple-values-from-a-c-function

#include <Joystick.h> // Import Joystick class
#include <Potentiometer.h> // Import Potentiometer class

// Create a hotspot on your phone and connect to it via your ESP32. Remember to connect with your computer too!
const char * ssid = "A Monkey's Phone"; // Name of the network
const char * password = "Lucked6334";   // Password of the network

AsyncUDP udp;

// Note for what pins to use: The ADC2 analog pins do not work as intended due to the wifi configuration.
// Use Pin 32-36 and 39 instead for reading analog input.

const int Joystick1XPin = 33; // Analog Pin.
const int Joystick1YPin = 32; // Analog Pin.
const int Joystick1ButtonPin = 21;  // Digital Pin.

const int potentiometerPin = 36; // Analog Pin. 

// Create a joystick object
Joystick joystick1(Joystick1XPin, Joystick1YPin, Joystick1ButtonPin);

// Create a potentiometer object
Potentiometer potentiometer(potentiometerPin);

void setup(){    

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

// Loop must be at the bottom for the code to compile without errors.
void loop(){   
    // Wait some time before running the loop as to not flood the terminal with information.
    delay(5000);

    potentiometer.updateState();
    
    potentiometer.printPotentiometerState();

    if (potentiometer.getVoltage() >= (3.3/2)){
    sendMessage("V is high.", 43, 81, 333);
    } else{
    sendMessage("V is low.", 43, 81, 333);
    }

  /*int JoystickXValue = map(analogRead(JoystickXPin), 0, 4095, -100, 100);
  int JoystickYValue = map(analogRead(JoystickYPin), 0, 4095, -100, 100);
  int ButtonPressed = digitalRead(ButtonPin);
  
  checkDeadZone(JoystickXValue, JoystickYValue);

  printButtonState(JoystickXValue, JoystickYValue, ButtonPressed);

  printSpace(); */ 
  delay(2000);

    // Send broadcast on port 4000
    // udp.broadcastTo("Anyone here?", 4000);

    // sendMessage("Hi Philipp, can you read this?", 43, 81, 333);
    //Serial.println("waiting for udp message...");
    // udp.writeTo((const uint8_t*)"Hi Philipp, can you read this?", 31, IPAddress(192,168,43,81), 54321); // Message, char count, IP, port
    // udp.writeTo((const uint8_t*)"Hi Hans, can you read this?", 27, IPAddress(192,168,43,249), 54321);
}