/*
  file      output_winch.h
  author    Urs Hofmann
  mail      urshofmann@gmx.net, hofmannu@student.ethz.ch
  date      01. Nov 2017
  version   1.0

  Class representing the end-effector of the tractionwire-approach.
*/

#include "output_winch.h"

using namespace std;

// Set functions

// Set required output force in [N]
void output_winch::set_output_force(const float&  _F_output)
{
  F_output = _F_output;
  status_output_force = 1;
}

// Set efficiency of transmission system in [0 ... 1]
void output_winch::set_eff_trans_system(const float& _eff_trans_system)
{
  eff_trans_system = _eff_trans_system;
  status_eff_trans_system = 1;
}
/*
  The transmission efficiency of the pull wires is important for us since the
  maximum allowable rope tension is occuring at the sending unit / actuation
  unit and therefore reduced by the factor eff_trans_system before it reaches
  the receiving unit / endeffector.
*/

// Set required length of rack [mm]
void output_winch::set_rack_length(const float& _length_rack)
{
  length_rack = _length_rack;
}

// Calculation procedure

void output_winch::calculate()
{

  // Check if output force was defined
  if(status_output_force == 0)
    cout << "E: Ouptput force was not defined" << endl;
  else if(status_eff_trans_system == 0)
    cout << "E: Efficiency of transmission system not defined" << endl;
  else
  {

    // Calculates the required radius of the output winch and rope tension
    calculate_required_radius();

    // Select rack as a function of out
    myRack = myRackSelector.select_rack(length_rack, F_output);

    // Includes calculation of forces acting on bearings
    select_bearings();

    // Calculate mass and volume of output winch
    calculate_material_volume();
    calculate_material_mass();

    calculate_outer_volume();
  }

}

void output_winch::calculate_required_radius()
{
  // Incoming force at receiving / output winch: The maximum force in the rope i
  // s occuring at the sending unit / actuation unit since it is reduced by fric
  // tion afterwards.

  if (eff_trans_system > 1)
    cout << "[output_winch] E: Efficiency of transmission system can only be < "
      "1." << endl;
  else
    F_rope = F_rope_max * eff_trans_system;

  // Radius of receiving / output winch
  r_winch = F_output / F_rope * r_pinion / eff_rack_and_pinion;

  if (r_winch < 2)
  {
    r_winch = 2;
    cout << "[output_winch] W: Lower limit of output radius reached" << endl;
  }

  // Write output radius
  //cout << "Required output radius = " << r_winch << " mm" << endl;
}

void output_winch::calculate_material_volume()
{
  // (1)  Winch
  // Winch and winch cover part
  float VWinch = M_PI * pow(r_winch, 2) * h_winch; // [mm^3]
  float VWinchCover = M_PI * pow(r_winch + 2, 2) * h_cover * 2; // [mm^3]

  // Bearing volume
  float VInnerBearing = M_PI * pow(myBearing.get_inner_radius(), 2) *
    (myBearing.get_height() + tolerance);

  // Gear volume
  float VGear = M_PI * pow(r_pinion, 2) * (myRack.get_height() + tolerance);

  // Sum up
  VMaterial = VWinch + VWinchCover + VInnerBearing * 2 + VGear;

  // (2)  Housing
  VHousing = M_PI * (
    2 * tHousing * pow(myBearing.get_outer_radius()+tHousing, 2)
    + myBearing.get_height() * (pow(myBearing.get_outer_radius() + tHousing, 2) -
    myBearing.get_outer_radius(), 2) * 2 + (pow(r_winch + tolerance + tHousing, 2) -
    pow(myBearing.get_outer_radius(), 2)) * tHousing * 2 + (pow(r_winch + tolerance
    + tHousing, 2) - pow(r_winch + tolerance, 2)) * (h_winch + h_cover * 2 +  2 * tolerance)
    + (pow(r_pinion + tolerance + tHousing, 2) - pow(r_pinion + tolerance, 2))
    * (tHousing + myRack.get_height() + 2 * tolerance));

}

void output_winch::calculate_outer_volume()
{
  float volumeBearing = M_PI * pow(myBearing.get_outer_radius() + tHousing, 2) *
    myBearing.get_height(); // [mm^3]
  float volumeWinch = M_PI * pow(r_winch + tolerance + tHousing, 2) *
    (h_winch + 2 * h_cover + 2 * tolerance + 2 * tHousing); // [mm^3]
  float volumeRackHosuing = (myRack.get_height() + 2 * tHousing) *
    (myRack.get_width() + 2 * tHousing) * 2 * r_winch; // [mm^3]
  outerVolume = 2 * volumeBearing + volumeWinch + volumeRackHosuing; // [mm^3]
  /*outerVolume = M_PI * pow(r_winch + tolerance + tHousing, 2) *
    (myBearing.get_height() * 2 + myRack.get_height() + h_winch +
    2 * h_cover + 4 * tolerance + 2 * tHousing); // [mm^3]*/
}

// calculate mass of material as a function of its volume and density

void output_winch::calculate_material_mass()
{
  massWinch = density * VMaterial;
  massHousing = density * VHousing;
}

void output_winch::select_bearings()
{
  d_r1_rope = h_winch / 2 + h_cover + h_buffer;
  d_rope_gear = h_winch / 2 + h_cover + myRack.get_height() / 2;
  d_gear_r2 = myRack.get_height() / 2 + h_buffer;

  // Careful: these values don't include the size of the bearing which might not
  // be neglegible. They will be added lateron in the bearing selection procedur
  // e since they are different between the bearings.

  myBearing = myBearingSelector.select_bearing(F_rope, F_output, d_r1_rope, d_rope_gear,
    d_gear_r2);

}
