/*
  file    stepper_motor.h
  author  Urs Hofmann
  date    27. Nov 2017
  mail    urshofmann@gmx.net
*/

#ifndef STEPPER_MOTOR_H
#define STEPPER_MOTOR_H

class stepper_motor
{

private:

  float mass; // [g]
  float volume; // [mm^3]
  float holdingTorque; // [Nm]
  // Static values, see force_over_rpm calculation
  float optimalSpeed = 5; // [Hz]
  float holdingTorqueToTorque = 0.18 / 0.273;
  float torque; // [Nm]
  float current; // [A]
  float voltage; // [V]

public:

  // Write / set functions
  void set_mass(const float& _mass); // [g]
  void set_volume(const float& _volume); // [cm^3]
  void set_holding_torque(const float& _holdingTorque); // [Nm]
  void set_current(const float& _current); // [A
  void set_voltage(const float& _voltage); // [V]

  // Read out / get functions
  float get_mass() const {return mass;}; // [g]
  float get_volume() const {return volume;}; // [cm^3]
  float get_speed() const {return optimalSpeed;}; // [Hz]
  float get_torque(); // [Nm]
  float get_current() const {return current;}; // [A]
  float get_voltage() const {return voltage;}; // [V]
  float get_power();
};

#endif
