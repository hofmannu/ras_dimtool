#include "hydraulic_tube.h"
#include <math.h>

void hydraulic_tube::set_length(const float& _length)
{
  length = _length;
}

void hydraulic_tube::set_flow_rate(const float& _flowRate)
{
  flowRate = _flowRate;
}

void hydraulic_tube::calculate()
{
  // Calculate inner radius as a function of flowRate
  // Scale  inner radius as a function of the flow rate
  rInner = 1 * sqrt(flowRate / (50 * M_PI * pow(4, 2))); // [mm]
  // Set a lower limit for inner radius
  if (rInner < 0.8)
    rInner = 0.8;

  // Calculate outer radius with Kesselformel
  float tWall = 0.5 / 1 * rInner; // [mm]
  if (tWall < 0.5)
    tWall = 0.5;
  rOuter = rInner + tWall;

  float VWater = pow(rInner, 2) * M_PI * length; // [mm^3]
  float VTube = (pow(rOuter, 2) - pow(rInner, 2)) * M_PI * length; // [mm^3]
  mass = VWater * densityWater + VTube * densityPolymer; // [g]

  outerVolume = pow(rOuter, 2) * M_PI * length; // [mm^3]
}
