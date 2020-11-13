#include <Arduino.h>
#include "WiFi.h"
#include "AsyncUDP.h"
// #include <tuple>  // Allows to easily return multiple values
                  // Source: https://stackoverflow.com/questions/321068/returning-multiple-values-from-a-c-function
                  // Was needed earlier but with generalisation (through getters) of the code it's no longer necessary.

#include <Joystick.h> // Import Joystick class
#include <Potentiometer.h> // Import Potentiometer class

// Create a hotspot on your phone and connect to it via your ESP32. Remember to connect with your computer too!
const char * ssid = "A Monkey's Phone"; // Name of the network
const char * password = "Lucked6334";   // Password of the network

// Change the IPs when you run the code on your computer!
const int thirdIP = 43;
const int fourthIP = 81;
const int port = 6000; // Depending on what the receiver in the GUI is set to!

AsyncUDP udp;

// Note for what pins to use: The ADC2 analog pins do not work as intended due to the wifi configuration.
// Use Pin 32-36 and 39 instead for reading analog input.

const int Joystick1XPin = 33; // Analog Pin.
const int Joystick1YPin = 32; // Analog Pin.
const int Joystick1ButtonPin = 21;  // Digital Pin.
const int Joystick2XPin = 35; // Analog Pin.
const int Joystick2YPin = 34; // Analog Pin.
const int Joystick2ButtonPin = 19;  // Digital Pin.

const int potentiometerPin = 36; // Analog Pin.

const int voltageMax = 3.3; // Maximum voltage on the breadboard.

// Pixels needed for PixelEmulator
int pixelX = 10;
int pixelY = 10;

// Create a joystick object
Joystick joystick1("Joystick1", Joystick1XPin, Joystick1YPin, Joystick1ButtonPin);
Joystick joystick2("Joystick2", Joystick2XPin, Joystick2YPin, Joystick2ButtonPin);

// Create a potentiometer object
Potentiometer potentiometer(potentiometerPin);

// Used to adjust the strength of movement before sending it as a command.
int speedModifier = 1;

// Used to build the rc command.
String commandRC = "";

// Used for whether the drone has taken off or not.
boolean droneIsActive = false;

// Used in the beginning to activate the drone to receive commands.
boolean commandSent = false;

// sendMessage method using the last two digits of the IP and a port to send a message.
// Remember to change the UDP Port in your Packet Sender application to receive the messages
void sendMessage(String msg){
    udp.writeTo((const uint8_t *)msg.c_str(), msg.length(), IPAddress(192, 168, thirdIP, fourthIP), port);
}

// Send Tello drone rc command: a (left/right) where (-100 <= a <= 100)
String leftRight(){
    String leftRight = "";  
    if (joystick1.getJoystickYValue() == 100){
        leftRight += (1) * speedModifier;
    }
    else if (joystick1.getJoystickYValue() == -100){
        leftRight += (-1) * speedModifier;
    } else {
        leftRight += 0;
    }
    leftRight += " ";
    return leftRight;
}

// Send Tello drone rc command: b (forward/backward) = where (-100 <= b <= 100)
String forwardBackward(){
    String forwardBackward = "";
    if (joystick1.getJoystickXValue() == 100){
        forwardBackward += 1 * speedModifier;
    }
    else if (joystick1.getJoystickXValue() == -100){
        forwardBackward += (-1) * speedModifier;
    } else {
        forwardBackward += 0;
    }
    forwardBackward += " ";
    return forwardBackward;
}

// Send Tello drone rc command: c (up/down) = where (-100 <= c <= 100)
String upDown(){
    String upDown = "";
    if (joystick2.getJoystickYValue() == 100){
        upDown += 1 * speedModifier;
    }
    else if (joystick2.getJoystickYValue() == -100){
        upDown += (-1) * speedModifier;
    } else {
        upDown += 0;
    }
    upDown += " ";
    return upDown;
}

// Send Tello drone rc command: d (faceDirection) = where (-100 <= d <= 100)
String faceDirection(){
    String faceDirection = "";
    if (joystick2.getJoystickXValue() == 100){
        faceDirection += 1 * speedModifier;
    }
    else if (joystick2.getJoystickXValue() == -100){
        faceDirection += (-1) * speedModifier;
    } else {
        faceDirection += 0;
    }
    // faceDirection += " ";
    return faceDirection;
}

// Adjusts the speed modifier using the potentiometer, which modifies how strong the drone reacts to movement.
int adjustSpeed(){
    // If the potentiometer value is not cast to double, the division by 4095 will return 0.
    double potentiometerValueDouble = (double) potentiometer.getPotentiometerValue();
    int speedModifier = potentiometerValueDouble / 4095 * 100;
    Serial.println(speedModifier);
    return speedModifier;
}

// Builds the rc command for the (Tello) drone.
String buildCommandRC(){
    String commandRC = "rc ";
    commandRC += leftRight();
    commandRC += forwardBackward();
    commandRC += upDown();
    commandRC += faceDirection();

    return commandRC;
}

// Sends the land / take off commands based on whether the drone is set to active or not.
void landTakeOff(){
    if (droneIsActive){
        sendMessage("takeoff");
    }
    else if (!droneIsActive){
        sendMessage("land");
    }
}

// Changes the activity of the drone if the button of joystick1 is pressed.
boolean updateDroneActivity(){
    if (joystick1.checkButtonState()){
        droneIsActive = !droneIsActive;
        landTakeOff();
    }
    return droneIsActive;
}


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
    
    // Initialize the first pixel that represents the drone.
    // sendMessage("init " + String(pixelX) + " " + String(pixelY));
        
}

// Loop must be at the bottom for the code to compile without errors.
void loop(){

    // Activates the drone so it can receive commands.
    if (!commandSent){
        sendMessage("command");
        commandSent = true;
    }

    // Always updateState for all objects at the start of the loop!
    joystick1.updateState();
    joystick2.updateState();
    potentiometer.updateState();
    Serial.println(joystick1.printJoystickState());

    updateDroneActivity();

    if (droneIsActive){
    Serial.println(joystick2.printJoystickState());
    Serial.println(potentiometer.printPotentiometerState());
        speedModifier = adjustSpeed();
    
        commandRC = buildCommandRC();
        sendMessage(commandRC);
    }
    else {
        sendMessage("Drone inactive. Press left joystick to start the drone.");
        Serial.println("Press the left joystick to start the drone.");
    }

    // Serial.println(" This " + String(60) + " Also this " + String(50));


    // Send broadcast on port x
    // udp.broadcastTo("Anyone here?", x);

    // sendMessage("Hi Philipp, can you read this?");
    

    // Wait some time before running the loop again as to not flood the terminal with information.
    delay(3000);
}