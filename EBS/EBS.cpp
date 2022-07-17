#include "EBS.hpp"
#include "Arduino.h"
#include <SPI.h>
#include <mcp2515.h>
#include "common.hpp"

/* EBS CONFIGURATION */
#define CS_PIN 10
#define CAN_EBS_ID 0x0
#define CAN_MASTER_ID 0xFF

MCP2515 mcp2515(CS_PIN);

#define MINIMUM_STOP_DISTANCE 15

#define ECHOPIN_FRONT 4 
#define TRIGPIN_FRONT 3 
#define ECHOPIN_BACK  6
#define TRIGPIN_BACK  9

#define SOUND_SPEED   0.034
/* END EBS CONFIGURATION */

void EBS::init() {
  pinMode(TRIGPIN_FRONT, OUTPUT); 
  pinMode(ECHOPIN_FRONT, INPUT);
  
  pinMode(TRIGPIN_BACK, OUTPUT);
  pinMode(ECHOPIN_BACK, INPUT);

  mcp2515.reset();
  mcp2515.setBitrate(CAN_500KBPS, MCP_8MHZ);
  mcp2515.setNormalMode();
}

void EBS::front_compute_distance() {
  
  digitalWrite(TRIGPIN_FRONT, LOW);
  delayMicroseconds(5);
  digitalWrite(TRIGPIN_FRONT, HIGH);
  delayMicroseconds(5);
  digitalWrite(TRIGPIN_FRONT, LOW);

  uint32 temp = pulseIn(ECHOPIN_FRONT, HIGH);

  front_dist_cm = temp * SOUND_SPEED / 2;
  
}

void EBS::back_compute_distance() {

  digitalWrite(TRIGPIN_BACK, LOW);
  delayMicroseconds(5);
  digitalWrite(TRIGPIN_BACK, HIGH);
  delayMicroseconds(5);
  digitalWrite(TRIGPIN_BACK, LOW);
  
  uint32 temp = pulseIn(ECHOPIN_BACK, HIGH);

  back_dist_cm = temp * SOUND_SPEED / 2;
  
}

void EBS::main_task() {
  
  front_compute_distance();
  back_compute_distance();

  if(front_dist_cm < MINIMUM_STOP_DISTANCE) {
    CAN_send_stop(CollisionDirection::FRONT);
    move_forward_allowed = false;
  } else if ( move_forward_allowed == false) {
    move_forward_allowed = true;
  }

  if(back_dist_cm < MINIMUM_STOP_DISTANCE) {
    CAN_send_stop(CollisionDirection::BACK);
    move_backward_allowed = false;
  } else if ( move_backward_allowed == false) {
    move_backward_allowed = true;
  }

  if(move_forward_allowed && move_backward_allowed) {
    CAN_send_ok();
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

/*
 * CAN message data
 * 
 * byte 1:  0 - all good
 *          1 - collision incoming
 *          2 - fatal error on EBS
 *          
 * byte 2:  0 - no collision
 *          1 - collision from back
 *          2 - collision from front
 */               
void EBS::CAN_send_stop(CollisionDirection colision_direction) {

  struct can_frame can_msg;

  can_msg.can_id = CAN_MASTER_ID;
  can_msg.can_dlc = 2;
  can_msg.data[0] = 1;
  can_msg.data[1] = (int)colision_direction;

  mcp2515.sendMessage(&can_msg);
  
}

void EBS::CAN_send_ok() {
  
  struct can_frame can_msg;

  can_msg.can_id = CAN_MASTER_ID;
  can_msg.can_dlc = 2;
  can_msg.data[0] = 0;
  can_msg.data[1] = 0;

  mcp2515.sendMessage(&can_msg);

}

void EBS::CAN_send_fatal_error() {
  struct can_frame can_msg;

  can_msg.can_id = CAN_MASTER_ID;
  can_msg.can_dlc = 2;
  can_msg.data[0] = 2;
  can_msg.data[1] = 0;

  mcp2515.sendMessage(&can_msg);
}
