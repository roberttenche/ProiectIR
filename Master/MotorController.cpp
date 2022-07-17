#include "MotorController.hpp"
#include "common.hpp"
#include "Arduino.h"
#include <IRremote.h>
#include <SPI.h>

/* MOTOR CONTROLLER CONFIG */
// forward    - 70
// backward   - 21
// left       - 68
// right      - 67
// OK         - 64 - stop
// 1          - 22 - reset
// 2          - 25
// 3          - 13
#define IR_FORWARD    70
#define IR_BACKWARD   21
#define IR_LEFT       68
#define IR_RIGHT      67
#define IR_STOP       64
#define IR_RESET      22

#define CS_PIN 10
#define CAN_EBS_ID 0x0
#define CAN_MASTER_ID 0xFF

#define EBS_OK 0
#define EBS_INC_COL 1
#define EBS_FATAL_ERROR 2

MCP2515 mcp2515(CS_PIN);

#define RECV_PIN 7
#define RESET_PIN 12

#define WHEELS_LEFT_SIDE_FORWARD    3
#define WHEELS_LEFT_SIDE_BACKWARD   4
#define WHEELS_RIGHT_SIDE_BACKWARD  5
#define WHEELS_RIGHT_SIDE_FORWARD   6

IRrecv irrecv(RECV_PIN);
decode_results results;
/* END MOTOR CONTROLLER CONFIG */

void MotorController::init() {
  IrReceiver.begin(RECV_PIN, ENABLE_LED_FEEDBACK);

  pinMode(WHEELS_LEFT_SIDE_FORWARD, OUTPUT);
  pinMode(WHEELS_LEFT_SIDE_BACKWARD, OUTPUT);
  pinMode(WHEELS_RIGHT_SIDE_BACKWARD, OUTPUT);
  pinMode(WHEELS_RIGHT_SIDE_FORWARD, OUTPUT);

  mcp2515.reset();
  mcp2515.setBitrate(CAN_500KBPS, MCP_8MHZ);
  mcp2515.setNormalMode();
  
}

void MotorController::main_task() {
  
  irrecv_task();

  execute_IRinput_task();

  CAN_handle_task();
  
}

void MotorController::CAN_handle_task(){

  struct can_frame can_msg;

  CAN_read(&can_msg);

  if(can_msg.can_id != CAN_MASTER_ID)
    return;

  uint8 byte1 = can_msg.data[0];
  uint8 byte2 = can_msg.data[1];

//  Serial.print("byte1: ");
//  Serial.println(byte1);
//  Serial.print("byte2: ");
//  Serial.println(byte2);

  switch(byte1)
  {
    case EBS_OK: {
      move_forward_allowed = true;
      move_backward_allowed = true;

      break;
    }
    
    case EBS_INC_COL: {

      if(byte2 == 1) {
        move_backward_allowed = false;
        if(moving_direction == MovingDirection::BACKWARD)
          move_stop();
      }
        
      if(byte2 == 2) {
        move_forward_allowed = false;
        if(moving_direction == MovingDirection::FORWARD)
          move_stop();
      }

      break;
      
    }
    
    case EBS_FATAL_ERROR: {
      move_forward_allowed = false;
      move_backward_allowed = false;

      break;
    }
  }
  
}

void MotorController::CAN_read(can_frame * can_msg) {
  if(mcp2515.readMessage(can_msg) == MCP2515::ERROR_OK) {
    
  }
}

void MotorController::move_forward() {
  if( moving_direction != MovingDirection::FORWARD ) {
    move_stop();
    if( move_forward_allowed ) {
      Serial.println("forward ok");
      digitalWrite(WHEELS_LEFT_SIDE_FORWARD, HIGH);
      digitalWrite(WHEELS_RIGHT_SIDE_FORWARD, HIGH);
      moving_direction = MovingDirection::FORWARD;
    }
  }
  
}

void MotorController::move_backward() {
  if( moving_direction != MovingDirection::BACKWARD ) {
    move_stop();
    if( move_backward_allowed ) {
      Serial.println("backward ok");
      digitalWrite(WHEELS_LEFT_SIDE_BACKWARD, HIGH);
      digitalWrite(WHEELS_RIGHT_SIDE_BACKWARD, HIGH);
      moving_direction = MovingDirection::BACKWARD;
    }
  }
}

void MotorController::move_left() {
  if( moving_direction != MovingDirection::LEFT ) {
    move_stop();
    digitalWrite(WHEELS_LEFT_SIDE_BACKWARD, HIGH);
    digitalWrite(WHEELS_RIGHT_SIDE_FORWARD, HIGH);
    moving_direction = MovingDirection::LEFT;
  }
}

void MotorController::move_right() {
  if( moving_direction != MovingDirection::RIGHT ) {
    move_stop();
    digitalWrite(WHEELS_LEFT_SIDE_FORWARD, HIGH);
    digitalWrite(WHEELS_RIGHT_SIDE_BACKWARD, HIGH);
    moving_direction = MovingDirection::RIGHT;
  }
}

void MotorController::move_stop() {
  if( moving_direction != MovingDirection::STOPPED ) {
    digitalWrite(WHEELS_LEFT_SIDE_FORWARD, LOW);
    digitalWrite(WHEELS_LEFT_SIDE_BACKWARD, LOW);
    digitalWrite(WHEELS_RIGHT_SIDE_BACKWARD, LOW);
    digitalWrite(WHEELS_RIGHT_SIDE_FORWARD, LOW);
    moving_direction = MovingDirection::STOPPED;
  }
}

void MotorController::irrecv_task() {
  if (IrReceiver.decode()) {
    key_code = IrReceiver.decodedIRData.command;
    IrReceiver.resume();

#ifdef PROJECT_DEBUGGING_ENABLED
    if(key_code != 0) 
      Serial.println(key_code);
#endif
  }
}

void MotorController::execute_IRinput_task() {
  switch(key_code) {
    case IR_FORWARD: {
      move_forward();
      break;
    }
    case IR_BACKWARD: {
      move_backward();
      break;
    }
    case IR_LEFT: {
      move_left();
      break;
    }
    case IR_RIGHT: {
      move_right();
      break;
    }
    case IR_STOP: {
      move_stop();
      break;
    }
    case IR_RESET: {
      Serial.println("RESET");
      break;
    }
  }
}
