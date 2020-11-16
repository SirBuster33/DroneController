#include <Arduino.h>
#include "WiFi.h"
#include "AsyncUDP.h"
// #include <tuple> // Allows to easily return multiple values
                    // Source: https://stackoverflow.com/questions/321068/returning-multiple-values-from-a-c-function
                    // Was needed earlier but with generalisation (through getters) of the code it's no longer necessary.

#include <Joystick.h>               // Import Joystick class
#include <Potentiometer.h>          // Import Potentiometer class

// Create a hotspot on your phone and connect to it via your ESP32. Remember to connect with your computer too!
const char * ssid = "A Monkey's Phone"; // Name of the network.     Tello Drone: "TELLO-59F752" Philipp: "A Monkey's Phone"
const char * password = "Lucked6334";   // Password of the network. Tello Drone: ""             Philipp: "Lucked6334"      

// Change the IPs when you run the code on your computer!
const int thirdIP = 43;
const int fourthIP = 81; 
const int port = 6000; // Depending on what the receiver in the GUI is set to!
// Emulator: (Port) 6000
// Tello Drone: - To send UdpPackages:      (IP) 192.168.10.1   (Port) 8889     Philipp (IP): 192.168.43.81
//              - To receive drone state:   (IP) 0.0.0.0        (Port) 8890
//              - To receive video stream:  (IP) 0.0.0.0        (Port) 11111

AsyncUDP udp;

// Note for what pins to use: The ADC2 analog pins do not work as intended due to the wifi configuration.
// Use Pin 32-36 and 39 instead for reading analog input.

const int Joystick1XPin = 33;       // Analog Pin.
const int Joystick1YPin = 32;       // Analog Pin.
const int Joystick1ButtonPin = 21;      // Digital Pin.
const int Joystick2XPin = 35;       // Analog Pin.
const int Joystick2YPin = 34;       // Analog Pin.
const int Joystick2ButtonPin = 19;      // Digital Pin.

const int potentiometerPin = 36;    // Analog Pin.

const int voltageMax = 3.3;         // Maximum voltage on the breadboard.

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
boolean droneIsHovering = false;

// Used to make a flip on joystick2 buttonpress.
boolean needsFlippin = false;

// Used in the beginning to activate the drone to receive commands.
boolean droneIsActive = false;

// Used to check whether a command was already sent during the loop.
boolean commandSent = false;

// sendMessage method using the last two digits of the IP and a port to send a message.
// Remember to change the UDP Port in your Packet Sender application to receive the messages
void sendMessage(String msg){
    udp.writeTo((const uint8_t *)msg.c_str(), msg.length(), IPAddress(192, 168, thirdIP, fourthIP), port);
    commandSent = true;
}

// Send Tello drone left / right x command. X is set to 100 as a default but can be: 20 <= X <= 500
void leftRight(){
    String leftRight = "";  
    if (joystick1.getJoystickYValue() == 100){
        leftRight += "left ";
        leftRight += 100;
        sendMessage(leftRight);
    }
    else if (joystick1.getJoystickYValue() == -100){
        leftRight += "right ";
        leftRight += 100;
        sendMessage(leftRight);
    }
}

// Send Tello drone forward / backward x command. X is set to 100 as a default but can be: 20 <= X <= 500
void forwardBackward(){
    String forwardBackward = "";
    if (joystick1.getJoystickXValue() == 100){
        forwardBackward += "forward ";
        forwardBackward += 100;
        sendMessage(forwardBackward);
    }
    else if (joystick1.getJoystickXValue() == -100){
        forwardBackward += "back ";
        forwardBackward += 100;
        sendMessage(forwardBackward);
    }
}

// Send Tello drone up / down x command. X is set to 100 as a default but can be: 20 <= X <= 500
void upDown(){
    String upDown = "";
    if (joystick2.getJoystickYValue() == 100){
        upDown += "up ";
        upDown += 100;
        sendMessage(upDown);
    }
    else if (joystick2.getJoystickYValue() == -100){
        upDown += "down ";
        upDown += 100;
        sendMessage(upDown);
    }
}

// Send Tello drone cw / ccw x command. X is set to 100 as a default but can be: 20 <= X <= 500
void faceDirection(){
    String faceDirection = "";
    if (joystick2.getJoystickXValue() == 100){
        faceDirection += "cw ";
        faceDirection += 100;
        sendMessage(faceDirection);
    }
    else if (joystick2.getJoystickXValue() == -100){
        faceDirection += "ccw ";
        faceDirection += 100;
        sendMessage(faceDirection);
    }
}

// Adjusts the speed modifier using the potentiometer, which modifies how strong the drone reacts to movement.
void adjustSpeed(){
    // If the potentiometer value is not cast to double, the division by 4095 will return 0.
    double potentiometerValueDouble = (double) potentiometer.getPotentiometerValue();
    // Serial.println(potentiometerValueDouble);
    // Serial.println(speedModifier);
    speedModifier = (int) (potentiometerValueDouble / 4095.0 * 100.0);
    String s = "";
    s += "Speedmodifier is set to: ";
    s += speedModifier;
    Serial.println(s);
    // Serial.println("Speedmodifier is set to: " + speedModifier);
    // --> Causes trouble because int needs to be converted to a string number first.
}

// Builds the command for the (Tello) drone.
String buildCommand(){
    leftRight();
    forwardBackward();
    upDown();
    faceDirection();
}

// Sends the land / take off commands based on whether the drone is set to active or not.
void landTakeOff(){
    if (droneIsHovering){
        sendMessage("takeoff");
    }
    else if (!droneIsHovering){
        sendMessage("land");
    }
}

// Changes the activity of the drone if the button of joystick1 is pressed.
void updateDroneActivity(){
    if (joystick1.checkButtonState()){
        droneIsHovering = !droneIsHovering;
        landTakeOff();
    }
    if (joystick2.checkButtonState()){
        needsFlippin = true;
    }
    else{
        needsFlippin = false;
    }
}

void activateDrone(){
    if (joystick2.checkButtonState()){
        sendMessage("command");
        Serial.println("Activating drone...");
        droneIsActive = true;
        
    }
}

void setup(){    

    Serial.begin(9600);

    // Wifi setup
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
    Serial.println(ssid);
    Serial.println(password);
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
            //packet.printf("Got %u bytes of data", packet.length());
        
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

    // Always updateState for all objects at the start of the loop!
    joystick1.updateState();
    joystick2.updateState();
    potentiometer.updateState();

    // Print the state of the right joystick.
    // Since the drone needs to be activated first, the state of the other components are unnecessary at this point.
    Serial.println(joystick2.printJoystickState());

    // Reset the boolean meaning that no command was sent to the drone yet.
    // If any command is sent, this will turn true and no other command will be sent.
    commandSent = false;

    // Sends the first command "command" which activates the drone to receive other commands.
    // The if/else statements make sure that only one command is sent per update to avoid errors or overriding of commands.
    if (!droneIsActive){
        activateDrone();
    }

    if (droneIsActive && !commandSent){
        
        // Print the state of the left joystick.
        Serial.println(joystick1.printJoystickState());

        // Tell the drone to either take off or land on joystick1 button press.
        updateDroneActivity();

        // If the drone is in the air, send movement commands.
        if (droneIsHovering && !commandSent){
            
            // Print the state of the potentiometer.
            Serial.println(potentiometer.printPotentiometerState());


            if (needsFlippin && !commandSent) {
                // Make a backward (b) flip. For other flips, choose left (l), right (r) or forward (f).
                sendMessage("flip b");
            }
            else if (!commandSent) {
                adjustSpeed();
                // Send basic movement commands based on the joystick movements. Only one input will be read and sent at a time.
                buildCommand();
            }
        }
        else if (!commandSent) {
            sendMessage("Drone not hovering. Hold the left joystick button pressed to take off.");
            Serial.println("Hold the left joystick button pressed to take off.\n");
        }

    }

    // Wait some time before running the loop again as to not flood the terminal with information.
    delay(3000);
}