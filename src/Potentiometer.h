#include <Arduino.h>
// #include <tuple>  // Allows to easily return multiple values
                  // Source: https://stackoverflow.com/questions/321068/returning-multiple-values-from-a-c-function
                  // Was needed earlier but with generalisation (through getters) of the code it's no longer necessary.
class Potentiometer{
  private:
    int potentiometerPin;
    int potentiometerValue;
    double voltage;
  
    // Add public: to make the methods below accessible, otherwise, the previous private: would make everything private.
  public: 
    // Constructor
    Potentiometer (int potentiometerPin){
      this->potentiometerPin = potentiometerPin;
      pinMode (potentiometerPin, INPUT);
      this->potentiometerValue = 0;
      this->voltage = 0;
    }

    // Getters
    int getPotentiometerPin(){
      return this->potentiometerPin;
    }
    int getPotentiometerValue(){
      return this->potentiometerValue;
    }

    int getVoltage(){
      return this->voltage;
    }


    // Reads the input on the breadboard.
    void updateState(){
      this->potentiometerValue = analogRead(this->potentiometerPin);
      this->voltage = potentiometerValue / 4095.0 * 3.3;
    }

    // Prints out the current state of the potentiometer.
    String printPotentiometerState(){
      // this->updateState(); // Ensures that the current state is printed.
      String s;
      s += "The potentiometer is set at: ";
      s += this->potentiometerValue;
      s += "\nThe voltage is set at: ";
      s += this->voltage;
      s += "\n";
      return s;
    }

};