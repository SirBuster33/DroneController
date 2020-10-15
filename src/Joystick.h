#include <Arduino.h>
// #include <tuple>  // Allows to easily return multiple values
                  // Source: https://stackoverflow.com/questions/321068/returning-multiple-values-from-a-c-function
                  // Was needed earlier but with generalisation (through getters) of the code it's no longer necessary.

class Joystick{
  private:
    int JoystickXPin;
    int JoystickYPin;
    int JoystickButtonPin;
    String JoystickName;

    int JoystickXValue;
    int JoystickYValue;
    int JoystickButton;

    // Add public: to make the methods below accessible, otherwise, the previous private: would make everything private.
  public:
    // Constructor for Joystick
    Joystick (String JoystickName, int JoystickXPin, int JoystickYPin, int JoystickButtonPin)
    {
      this->JoystickName = JoystickName;
      this->JoystickXPin = JoystickXPin;
      pinMode (this->JoystickXPin, INPUT);
      this->JoystickYPin = JoystickYPin;
      pinMode (this->JoystickYPin, INPUT);
      this->JoystickButtonPin = JoystickButtonPin;
      pinMode (this->JoystickButtonPin, INPUT_PULLUP);
      int JoystickXValue = 0;
      int JoystickYValue = 0;
      int JoystickButton = 0;

    }

    // Getters
    String getJoystickName(){
      return this->JoystickName;
    }
    int getJoystickXPin(){
      return this->JoystickXPin;
    }
    int getJoystickYPin(){
      return this->JoystickYPin;
    }
    int getJoystickButtonPin(){
      return this->JoystickButtonPin;
    }

    int getJoystickXValue(){
      return this->JoystickXValue;
    }
    int getJoystickYValue(){
      return this->JoystickYValue;
    }
    int getJoystickButton(){
      return this->JoystickButton;
    }
      

    // updateState reads the breadboard input and passes it to the joystick variables.
    void updateState(){
      this->JoystickXValue = map(analogRead(this->JoystickXPin), 0, 4095, -100, 100);
      this->JoystickYValue = map(analogRead(this->JoystickYPin), 0, 4095, -100, 100);
      this->JoystickButton = digitalRead(this->JoystickButtonPin);
      this->checkDeadZone(40, -40);
    }

    // checkDeadZone checks whether the X- and Y-Value are within a preset deadZone range and adjusts their values.
    // Created as separate method from updateState() to keep the code cleaner.
    void checkDeadZone(int UpperRange, int LowerRange){
  
      // Preset range values. Depending on position and which joystick is connected, the deadzone may be completely different.
      // int UpperRange = 40; 
      // int LowerRange = -40;
      boolean xIsHigh (this->JoystickXValue > UpperRange);
      boolean xIsLow (this->JoystickXValue < LowerRange);
      boolean yIsHigh (this->JoystickYValue > UpperRange);
      boolean yIsLow (this->JoystickYValue < LowerRange);

      // Remapping of the Value ranges based on what values X and Y have.
      if (xIsHigh) {
        this->JoystickXValue = map(this->JoystickXValue, UpperRange, 100, 0, 100);
      }
      else if (xIsLow) {
        this->JoystickXValue = map(this->JoystickXValue, LowerRange, -100, 0, -100);
      }
      else {
        this->JoystickXValue = 0;
      }

      if (yIsHigh) {
        this->JoystickYValue = map(this->JoystickYValue, UpperRange, 100, 0, 100);    
      }
      else if (yIsLow) {
        this->JoystickYValue = map(this->JoystickYValue, LowerRange, -100, 0, -100);
      }
      else {
        this->JoystickYValue = 0;
      }
  
    }

    // Returns the current state of the joystick as a String.
    String printJoystickState(){
      String s;
      s += "Printing state for: ";
      s += this->JoystickName;
      s += "\nThe X value is: ";
      s += this->JoystickXValue;
      s += "\nThe Y Value is: ";
      s += this->JoystickYValue;

      s += "\nButton is: ";
      s += this->JoystickButton;

      // Remainder of the joystick exercise but nice to have for quick checking without thinking.
      if (this->JoystickButton == 0){
        s += "\nBANG! You hit the button of ";
        s += this->JoystickName;
        s += "!\n";
      }
      else{
        s += "\nButton of ";
        s += this->JoystickName;
        s += " is not pressed.\n";
      }

      return s;
    }

    void movePixel(){
      ;
    }

    // Prints extra space so that the output is readible. May be implemented somewhere else.
    void printSpace(){
      Serial.println("");
      Serial.println("");
      Serial.println("Taking new measurement...");
      Serial.println("");
    }

};
