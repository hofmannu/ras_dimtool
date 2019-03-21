/*
  file    stepper_motor_selector.h
  author  Urs Hofmann
  date    27. Nov 2017
  mail    urshofmann@gmx.net
*/

#include "stepper_motor.h"
#include "file_handling.h"
#include <vector>

#ifndef STEPPER_MOTOR_SELECTOR_H
#define STEPPER_MOTOR_SELECTOR_H

class stepper_motor_selector
{

private:

  float reqRopeSpeed; // [mm/s]
  float reqRopeTension;

  file_handling myFile;

  vector<stepper_motor> StepperMotorDatabase;
  stepper_motor selectedStepperMotor;

  void read_in_and_sort_database();
  void exclude_weak_steppers();
  void select_best_matching_stepper();

public:

  void set_req_rope_speed(const float& _reqRopeSpeed);
  void set_req_rope_tension(const float& _reqRopeTension);
  vector<stepper_motor> select_stepper_motor();

};

#endif
