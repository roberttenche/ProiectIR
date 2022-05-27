#include "EBS.hpp"
#include "Arduino.h"
#include "common.hpp"

/* EBA CONFIGURATION */
#define MINIMUM_STOP_DISTANCE 15

#define ECHOPIN_FRONT 3 
#define TRIGPIN_FRONT 2 
#define ECHOPIN_BACK  6
#define TRIGPIN_BACK  9

#define SOUND_SPEED   0.034
/* END EBA CONFIGURATION */

void EBS::init() {
  pinMode(TRIGPIN_FRONT, OUTPUT); 
  pinMode(ECHOPIN_FRONT, INPUT);
  
  pinMode(TRIGPIN_BACK, OUTPUT);
  pinMode(ECHOPIN_BACK, INPUT);
}

void EBS::front_compute_distance() {
  
  digitalWrite(TRIGPIN_FRONT, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIGPIN_FRONT, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIGPIN_FRONT, LOW);

  uint32 temp = pulseIn(ECHOPIN_FRONT, HIGH);

  front_dist_cm = temp * SOUND_SPEED / 2;
  
}

void EBS::back_compute_distance() {

  digitalWrite(TRIGPIN_BACK, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIGPIN_BACK, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIGPIN_BACK, LOW);
  
  uint32 temp = pulseIn(ECHOPIN_BACK, HIGH);

  back_dist_cm = temp * SOUND_SPEED / 2;
  
}

void EBS::main_task() {
  
  front_compute_distance();
  back_compute_distance();

  if(front_dist_cm < MINIMUM_STOP_DISTANCE) {
    CAN_send_stop();
  }

  if(back_dist_cm < MINIMUM_STOP_DISTANCE) {
    CAN_send_stop();
  }

#ifdef PROJECT_DEBUGGING_ENABLED
  Serial.print("FRONT: ");
  Serial.print(front_dist_cm);
  Serial.println("cm");

  Serial.print("BACK: ");
  Serial.print(back_dist_cm);
  Serial.println("cm");
#endif
  
}

void EBS::CAN_send_stop() {

  Serial.println("EBS CAN STOP");

}
