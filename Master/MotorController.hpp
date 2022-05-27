#pragma once
#include "common.hpp"

enum class MovingDirection {
  STOPPED,
  FORWARD,
  BACKWARD,
  LEFT,
  RIGHT
};

class MotorController {
public:
  void init();
  void main_task();

private:

  MovingDirection moving_direction = MovingDirection::STOPPED;

  bool move_forward_allowed = true;
  bool move_backward_allowed = true;

  uint32 key_code = 0xFFFFFFFF;

  void irrecv_task();

  void execute_IRinput_task();

  void move_forward();
  void move_backward();
  void move_left();
  void move_right();
  void move_stop();
  
};
