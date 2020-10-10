#include <Arduino.h>
#include <tuple>  // Allows to easily return multiple values
                  // Source: https://stackoverflow.com/questions/321068/returning-multiple-values-from-a-c-function

class Potentiometer{
  public:
    int potentiometerPin;

  // Constructor
  Potentiometer (int potentiometerPin){
    this->potentiometerPin = potentiometerPin;
    pinMode (potentiometerPin, INPUT);
  }

  /*void setup() {
  pinMode (potentiometerPin, INPUT);
  Serial.begin(9600);
  }*/

  // Reads the input on the breadboard.
  std::tuple<int, int> updateState(){
    int potentiometerValue = analogRead(potentiometerPin);
    double voltage = potentiometerValue / 4095.0 * 3.3;
    return std::make_tuple(potentiometerValue, voltage);
  }

  String printPotentiometerState(int potentiometerValue, int voltage){
    String s;
    s += "The potentiometer is set at: ";
    s += potentiometerValue;
    s += "\nThe voltage is set at: ";
    s += voltage;
    return s;
  }

};