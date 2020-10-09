#include <Arduino.h>
#include <tuple>  // Allows to easily return multiple values
                  // Source: https://stackoverflow.com/questions/321068/returning-multiple-values-from-a-c-function

class Joystick{
public:
  int JoystickXPin;
  int JoystickYPin;
  int ButtonPin;

  // Constructor for Joystick
  Joystick (int JoystickXPin, int JoystickYPin, int ButtonPin)
  {
    this->JoystickXPin = JoystickXPin;
    pinMode (this->JoystickXPin, INPUT);
    this->JoystickYPin = JoystickYPin;
    pinMode (this->JoystickYPin, INPUT);
    this->ButtonPin = ButtonPin;
    pinMode (this->ButtonPin, INPUT_PULLUP);

  }
  
  /*const int JoystickXPin = 33;
  const int JoystickYPin = 32;
  const int ButtonPin = 35;*/

  /*void setup() {
    pinMode (JoystickXPin, INPUT);
    pinMode (JoystickYPin, INPUT);
    pinMode (ButtonPin, INPUT_PULLUP);

    Serial.begin(9600);
  }*/

// updateState reads the breadboard input and passes it to the joystick variables.
std::tuple<int, int, int> updateState(){

  int JoystickXValue = map(analogRead(JoystickXPin), 0, 4095, -100, 100);
  int JoystickYValue = map(analogRead(JoystickYPin), 0, 4095, -100, 100);
  int ButtonPressed = digitalRead(ButtonPin);
  return std::make_tuple(JoystickXValue, JoystickYValue, ButtonPressed);
}

// checkDeadZone checks whether the X- and Y-Value are within a preset deadZone range and adjusts their values.
std::tuple<int, int> checkDeadZone(int JoystickXValue, int JoystickYValue){
  
  // Preset range values. Depending on position and which joystick is connected, the deadzone may be completely different.
  int UpperRange = 40; 
  int LowerRange = -40;
  boolean xIsHigh (JoystickXValue > UpperRange);
  boolean xIsLow (JoystickXValue < LowerRange);
  boolean yIsHigh (JoystickYValue > UpperRange);
  boolean yIsLow (JoystickYValue < LowerRange);

  // Remapping of the Value ranges based on what values X and Y have.
  if (xIsHigh) {
    JoystickXValue = map(JoystickXValue, UpperRange, 100, 0, 100);
  }
  else if (xIsLow) {
    JoystickXValue = map(JoystickXValue, LowerRange, -100, 0, -100);
  }
  else {
    JoystickXValue = 0;
  }

  if (yIsHigh) {
    JoystickYValue = map(JoystickYValue, UpperRange, 100, 0, 100);    
  }
  else if (yIsLow) {
    JoystickYValue = map(JoystickYValue, LowerRange, -100, 0, -100);
  }
  else {
    JoystickYValue = 0;
  }
  
  return std::make_tuple(JoystickXValue, JoystickYValue);
}

String printJoystickState(int JoystickXValue, int JoystickYValue, int ButtonPressed){
  String s;
  s += "The X value is: ";
  s += JoystickXValue;
  s += "\nThe Y Value is: ";
  s += JoystickYValue;

  s += "\n Button is: ";
  s += ButtonPressed;


  if (ButtonPressed == 1){
    s += "BANG! You hit the button!";
  }
  else{
    s += "Button is not pressed.";
  }
  
  return s;
}

/*// Send ButtonState as a message
String messageButtonState(int JoystickXValue, int JoystickYValue, int ButtonPressed){
  String s;
  s += "The X value is: ";
  s += JoystickXValue;
  s += "\nThe Y Value is: ";
  s += JoystickYValue;

  s += "\n Button is: ";
  s += ButtonPressed;


  if (ButtonPressed == 1){
    s += "BANG! You hit the button!";
  }
  else{
    s += "Button is not pressed.";
  }
  
  return s;
}*/

void printSpace(){
  Serial.println("");
  Serial.println("");
  Serial.println("Taking new measurement...");
  Serial.println("");
}

// Loop must be at the bottom for the code to compile without errors.
/*void loop() {

  int JoystickXValue = map(analogRead(JoystickXPin), 0, 4095, -100, 100);
  int JoystickYValue = map(analogRead(JoystickYPin), 0, 4095, -100, 100);
  int ButtonPressed = digitalRead(ButtonPin);
  
  checkDeadZone(JoystickXValue, JoystickYValue);

  printButtonState(JoystickXValue, JoystickYValue, ButtonPressed);

  printSpace();  
  delay(2000);

}*/

};
