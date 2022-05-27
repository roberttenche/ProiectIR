#include "MotorController.hpp"

MotorController mc;
void setup() {
  mc.init();
  Serial.begin(9600); 
}

void loop() {
  mc.main_task();
}
