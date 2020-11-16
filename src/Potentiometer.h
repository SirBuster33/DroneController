#include <Arduino.h>
// #include <tuple>  // Allows to easily return multiple values
                  // Source: https://stackoverflow.com/questions/321068/returning-multiple-values-from-a-c-function
                  // Was needed earlier but with generalisation (through getters) of the code it's no longer necessary.
class Potentiometer{
  private:
    int potentiometerPin;
    int potentiometerValue;
    int mappedPotentiometerValue; // Used solely for adjustSpeed() method in main.
    double voltage;
    const int voltageMax = 3.3; // Maximum voltage on the breadboard.

    // Add public: to make the methods below accessible, otherwise, the previous private: would make everything private.
  public: 
    // Constructor
    Potentiometer (int potentiometerPin){
      this->potentiometerPin = potentiometerPin;
      pinMode (potentiometerPin, INPUT);
      this->potentiometerValue = 0;
      this->mappedPotentiometerValue;
      this->voltage = 0;
    }

    // Getters
    int getPotentiometerPin(){
      return this->potentiometerPin;
    }
    int getPotentiometerValue(){
      return this->potentiometerValue;
    }
    int getMappedPotentiometerValue(){
      return this->mappedPotentiometerValue;
    }

    int getVoltage(){
      return this->voltage;
    }
    int getVoltageMax(){
      return voltageMax;
    }


    // Reads the input on the breadboard.
    void updateState(){
      this->potentiometerValue = analogRead(this->potentiometerPin);
      this->mappedPotentiometerValue = map(analogRead(this->potentiometerPin), 0, 4095, 10, 100);
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