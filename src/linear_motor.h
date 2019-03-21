/*
  file    linear_motor.h
  author  Urs Hofmann
  date    25. Dez 2017
  mail    urshofmann@gmx.net
*/

#ifndef LINEAR_MOTOR_H
#define LINEAR_MOTOR_H

class linear_motor
{
private:
  float mass; // [g]
  float volume; // [mm^3]
  float speed; // [mm/s]
  float force; // [N]
  float current; // [V]
  float voltage; // [A]
  float stroke; // [mm]

public:
  // Write / set functions
  void set_mass(const float& _mass); // [g]
  void set_volume(const float& _volume); // [cm^3]
  void set_force(const float& _force); // [N]
  void set_current(const float& _current); // [A
  void set_voltage(const float& _voltage); // [V]
  void set_speed(const float& _speed);
  void set_stroke(const float& _stroke);

  // Read out / get functions
  float get_mass() const {return mass;}; // [g]
  float get_volume() const {return volume;}; // [cm^3]
  float get_speed() const {return speed;}; // [Hz]
  float get_force() const {return force;}; // [N]
  float get_current() const {return current;}; // [A]
  float get_voltage() const {return voltage;}; // [V]
  float get_stroke() const {return stroke;}; // [mm]
  float get_power();

};

#endif
