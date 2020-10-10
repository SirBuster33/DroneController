#include <Arduino.h>
#include <tuple>  // Allows to easily return multiple values
                  // Source: https://stackoverflow.com/questions/321068/returning-multiple-values-from-a-c-function

class Potentiometer{
  private:
    int potentiometerPin;
    int potentiometerValue;
    int voltage;

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

  /*void setup() {
  pinMode (potentiometerPin, INPUT);
  Serial.begin(9600);
  }*/

  // Reads the input on the breadboard.
  void updateState(){
    this->potentiometerValue = analogRead(potentiometerPin);
    this->voltage = potentiometerValue / 4095.0 * 3.3;
  }

  // Prints out the current state of the potentiometer.
  String printPotentiometerState(){
    this->updateState(); // Ensures that the current state is printed.
    String s;
    s += "The potentiometer is set at: ";
    s += this->potentiometerValue;
    s += "\nThe voltage is set at: ";
    s += this->voltage;
    return s;
  }

};