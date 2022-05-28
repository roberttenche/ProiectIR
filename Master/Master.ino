#include "MotorController.hpp"

MotorController mc;

void setup() {
  mc.init();
#ifdef PROJECT_DEBUGGING_ENABLED
  Serial.begin(115200);
#endif
}

void loop() {
  mc.main_task();
}
