#pragma once

#include "common.hpp"

enum class CollisionDirection {
  NONE,
  BACK,
  FRONT
};

class EBS {
public:
  void init();
  void main_task();

private:
  uint8 front_dist_cm = 0u;
  uint8 back_dist_cm = 0u;

  bool move_forward_allowed = true;
  bool move_backward_allowed = true;
  
  void front_compute_distance();
  void back_compute_distance();
  
  void CAN_send_stop(CollisionDirection);
  void CAN_send_ok();
  void CAN_send_fatal_error();
};
