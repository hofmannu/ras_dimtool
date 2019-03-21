#include "hydraulic_housing_bk.h"


void hydraulic_housing_bk::set_length(const float& _length)
{
  length = _length;
}

void hydraulic_housing_bk::set_width(const float& _width)
{
  width = _width;
}

void hydraulic_housing_bk::calculate()
{
  //cout << "[hydraulic_housing_bk] E: I am not ready to be used." << endl;
  float dConnector = 3; // [mm]
  float VScrews = pow(dConnector/2, 2) * M_PI * length; // [mm^3]
  float massScrews = densityScrews * VScrews; // [g]

  mass = massScrews + 10; // [g]

  outerVolume = width * width * length; // [mm^3]
  outerVolume = outerVolume / 1000; // [cm^3]

}
