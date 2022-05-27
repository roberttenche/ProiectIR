#pragma once

#include "common.hpp"

class EBS {
public:
  void init();
  void main_task();

private:
  uint8 front_dist_cm = 0u;
  uint8 back_dist_cm = 0u;
  
  void front_compute_distance();
  void back_compute_distance();
  void CAN_send_stop();
};
