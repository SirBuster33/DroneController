#include <Arduino.h>

class Potentiometer{
  public:
    int potentiometerPin;

  Potentiometer (int potentiometerPin){
    this->potentiometerPin = potentiometerPin;
    pinMode (potentiometerPin, INPUT);
  }

  /*void setup() {
  pinMode (potentiometerPin, INPUT);
  Serial.begin(9600);
  }*/

  String printPotentiometerState(){
    int potentiometerValue = analogRead(potentiometerPin);
    double voltage = potentiometerValue / 4095.0 * 3.3;
    Serial.print("The voltage is set at: ");
    Serial.println(voltage);
    delay(1000);

  }

};