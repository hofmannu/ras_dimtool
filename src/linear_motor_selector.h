/*
  file    linear_motor_selector.h
  author  Urs Hofmann
  date    25. Dez 2017
  mail    urshofmann@gmx.net
*/

#include "linear_motor.h"
#include "file_handling.h"
#include <vector>

#ifndef LINEAR_MOTOR_SELECTOR_H
#define LINEAR_MOTOR_SELECTOR_H

class linear_motor_selector
{
private:

  // requried output at slave cylinder
  float reqSpeed; // [mm/s]
  float reqForce; // [N]

  file_handling myFile;
  vector<linear_motor> LinearMotorDatabase;
  linear_motor linearMotorTemp;

  void read_in_and_sort_database();
  void exclude_weak_motors();

public:

  void set_req_speed(const float& _reqSpeed);
  void set_req_force(const float& _reqForce);
  vector<linear_motor> get_linear_motors();

};

#endif
