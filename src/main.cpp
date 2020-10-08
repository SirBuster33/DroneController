#include <Arduino.h>
#include "WiFi.h"
#include "AsyncUDP.h"

const char * ssid = "A Monkey's Phone";
const char * password = "Lucked6334";

// Hello Philipp with two pp's 

AsyncUDP udp;

void setup()
{
    Serial.begin(9600);
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

    // Send broadcast on port 4000
    // udp.broadcastTo("Anyone here?", 4000);


    sendMessage("Hi Philipp, can you read this?", 43, 81, 333);
    //Serial.println("waiting for udp message...");
    // udp.writeTo((const uint8_t*)"Hi Philipp, can you read this?", 31, IPAddress(192,168,43,81), 54321); // Message, char count, IP, port
    // udp.writeTo((const uint8_t*)"Hi Hans, can you read this?", 27, IPAddress(192,168,43,249), 54321);
    // udp.writeTo((const uint8_t*)"Hi, you are still broadcasting. Could you comment it out?", 57, IPAddress(192,168,1,38), 54321);
}