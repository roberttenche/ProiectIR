#include "common.hpp"
#include "EBS.hpp"

EBS ebs;

void setup() {
  ebs.init();

  pinMode(LED_BUILTIN, OUTPUT); 
  digitalWrite(LED_BUILTIN, HIGH);
  
  Serial.begin(9600);
}

void loop() {
  ebs.main_task();
}
