/*
  file    stepper_motor.cpp
  author  Urs Hofmann
  date    27. Nov 2017
  mail    urshofmann@gmx.net
*/

#include "stepper_motor.h"

// Set functions

void stepper_motor::set_mass(const float& _mass)
{
  mass = _mass; // [g]
}

void stepper_motor::set_volume(const float& _volume)
{
  volume = _volume; // [mm^3]
}

void stepper_motor::set_holding_torque(const float& _holdingTorque)
{
  holdingTorque = _holdingTorque; // [Nm]
}

void stepper_motor::set_current(const float& _current)
{
  current = _current; // [A]
}

void stepper_motor::set_voltage(const float& _voltage)
{
  voltage = _voltage; // [V]
}

// Get functions

float stepper_motor::get_torque()
{
  // Calculate torque at optimal working point
  torque = holdingTorqueToTorque * holdingTorque;
  return torque;

}

float stepper_motor::get_power()
{
  float power = 3.25 * holdingTorque / 0.27;
  return power;
}
