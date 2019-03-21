#include "tractionwire.h"

using namespace std;

void tractionwire::set_length(const float& _length)
{
  length = _length;
}

void tractionwire::set_bending_angle(const float& _bending_angle)
{
  bending_angle = _bending_angle;
}

void tractionwire::calculate_mass()
{
  mass = specific_mass * length;
  if (mass == 0)
    cout << " E: Cannot calculate mass before knowing length" << endl;
}

void tractionwire::calculate_volume()
{
  volume = pow(outer_diameter/2, 2) * M_PI * length;
  if (volume == 0)
    cout << " E: Cannot calculate volume before knowing length" << endl;
}

void tractionwire::calculate_efficiency()
{
  efficiency = exp(-coefficient_of_friction * bending_angle / 180 * M_PI);
  if (efficiency > 1)
    cout << " E: Efficiency can only be smaller than 0!" << endl;
}

void tractionwire::calculate()
{
  calculate_volume();
  calculate_mass();
  calculate_efficiency();
}
