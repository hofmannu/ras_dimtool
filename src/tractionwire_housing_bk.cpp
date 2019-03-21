#include "tractionwire_housing_bk.h"


void tractionwire_housing_bk::set_winch_diameter(const float& _winchDiameter)
{
  winchDiameter = _winchDiameter;
}


void tractionwire_housing_bk::calculate_outer_volume()
{
  outerSideLength = winchDiameter + 2 * (tolerance + wallThickness); // [mm]
  outerVolume = outerSideLength * (outerSideLength + fixedLengthSEEs) *
    fixedHeight; // [mm^3]
}

void tractionwire_housing_bk::calculate_mass()
{
  materialVolume =
    outerSideLength * outerSideLength * wallThickness * 2 +
    outerSideLength * fixedHeight * wallThickness * 4 +
    fixedLengthSEEs * (outerSideLength + fixedHeight) * 2 * wallThickness;
  materialMass = materialVolume * materialDensity;

  mass = materialMass + 2 * fixedMassSEEs;
}

void tractionwire_housing_bk::calculate()
{
  calculate_outer_volume();
  calculate_mass();
}
