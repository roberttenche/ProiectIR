#include "common.hpp"
#include "EBS.hpp"

EBS ebs;

void setup() {
  ebs.init();
  
#ifdef PROJECT_DEBUGGING_ENABLED
  Serial.begin(115200);
#endif
}

void loop() {
  ebs.main_task();
}
