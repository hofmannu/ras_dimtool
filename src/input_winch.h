/*
  file    stepper_motor_selector.h
  author  Urs Hofmann
  date    08. Dez 2017
  mail    urshofmann@gmx.net
*/

// Note: This class already includes the bearing in its mass and volume

#include <math.h>

#ifndef INPUT_WINCH_H
#define INPUT_WINCH_H

class input_winch
{
private:

  // Input Variables
  float ropeTension; // [N]
  float motorTorque; // [Nm]

  // Calculated variables
  float winchRadius; // [mm]
  float mass; // [g]

  // Material density
  float density =  2.7 / 1000; // [g/mm^3];

  // Constants for dimensions
  float gapForRope = 5; // [mm]
  float heightSeperator = 1; // [mm]
  float addRadiusSeperator = 5; // [mm]
  float tolerance = 1; // [mm] Distance between bearing and winch

  // Functions
  void calculate_radius();
  void calculate_mass();

public:

  void set_effective_motor_torque(const float& _motorTorque);
  void set_rope_tension(const float& _ropeTension);

  float get_mass() const {return mass;};
  float get_outer_radius() const {return (winchRadius + addRadiusSeperator);};
  float get_winch_radius() const {return winchRadius;};

  void calculate();

};

#endif
