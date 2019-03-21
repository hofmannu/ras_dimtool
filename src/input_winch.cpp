#include "input_winch.h"

// Set Functions

void input_winch::set_rope_tension(const float& _ropeTension)
{
  ropeTension = _ropeTension; // [N]
}

void input_winch::set_effective_motor_torque(const float& _motorTorque)
{
  motorTorque = _motorTorque; // [Nm]
}

// Calculations

void input_winch::calculate_radius()
{
  winchRadius = motorTorque / ropeTension * 1000; // [Nm] / [N] * 1000 = [mm]
}

void input_winch::calculate_mass()
{
  // Calculate material volume
  float VMaterial = 2 * M_PI * (
    pow((4 + tolerance), 2) * 2 +
    pow((winchRadius + addRadiusSeperator),2) * heightSeperator * 1.5 +
    pow(winchRadius, 2) * gapForRope); // [mm^3]

  // Calculate material Mass
  float massMaterial = VMaterial * density;

  // Include bearings in mass calculation
  mass = massMaterial + 2 * 3; // Added mass for bearings (fixed)
}

void input_winch::calculate()
{
  calculate_radius(); // [mm]
  calculate_mass(); // [g]
}
