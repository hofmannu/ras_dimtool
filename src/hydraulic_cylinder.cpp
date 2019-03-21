#include "hydraulic_cylinder.h"
#include <math.h>

void hydraulic_cylinder::set_rod_force(const float& _rodForce)
{
  rodForce = _rodForce;
  statusRodForce = 1;
}

void hydraulic_cylinder::set_stroke(const float& _stroke)
{
  stroke = _stroke;
  statusStroke = 1;
}

void hydraulic_cylinder::set_fluid_volume(const float& _fluidVolume)
{
  fluidVolume = _fluidVolume;
  statusFluidVolume = 1;
}

void hydraulic_cylinder::set_rod_bore_ratio(const float& _rodBoreRatio)
{
  rodBoreRatio = _rodBoreRatio;
  statusRodBoreRatio = 1;
}

void hydraulic_cylinder::calculate_material_mass()
{
  // Surface of wall
  float AWall = pow(dBore / 2 + tWall, 2) * M_PI - pow(dBore / 2, 2) * M_PI;

  // Volume of wall
  float VWall = AWall * (stroke + tORing + tPiston);
  //cout << "Calculated wall volume = " << VWall << " mm^3" << endl;

  // Surface of piston
  float APiston = pow((dBore / 2), 2) * M_PI;

  // Volume of tPiston
  float VPiston = APiston * (tORing + tPiston);
  //cout << "Calculated piston volume = " << VPiston << " mm^3" << endl;

  // Surface of rod
  float ARod = pow(dRod / 2, 2) * M_PI; // [mm^2]

  // Volume of rod
  float VRod = ARod * (stroke + addRodLength);
  //cout << "Calculated rod volume = " << VRod << " mm^3" << endl;

  // Surface of cylinder lid
  float ALid = pow(dBore/2 + tWall, 2) * M_PI;

  // Volume of cylinder lid
  float VLid = ALid * tLid;
  //cout << "Calculated cylinder lid volume = " << VLid << " mm^3" << endl;

  // Calculate material mass
  massMaterial = (VRod + VPiston + VWall + 2 * VLid) * densityMaterial;
  //cout << "Calculated material mass = " << massMaterial << " g" << endl;
}

void hydraulic_cylinder::calculate_fluid_mass()
{
  float VFluid = pow(dBore / 2, 2) * M_PI * stroke;
  massFluid = VFluid * densityFluid;
  //cout << "Calculated fluid mass = " << massFluid << " g " << endl;
}


void hydraulic_cylinder::calculate_mass()
{
  calculate_material_mass();
  calculate_fluid_mass();
  //cout << "Connector mass = " << massConnector << " g" << endl;

  massOverall = massMaterial + massFluid + noConnectors * massConnector; // [g]
}


void hydraulic_cylinder::calculate_outer_volume()
{
  outerVolume = pow(dBore/2 + tWall, 2) * M_PI * (stroke + tLid * 2 + tPiston +
    tORing) + pow(dRod/2, 2) * (addRodLength + stroke); // [mm^3]
}

void hydraulic_cylinder::calculate_bore_diameter()
{
  // Include security factor and friction compensation
  // --> Multiply force with 2
  // --> Conversion from bar to N/mm^2 --> Factor 2
  dBore = 2 * sqrt(2 * rodForce * 10 / (M_PI * maxPressure));

  // avoid bore diameters below 3 mm
  if (dBore < 3)
  {
    dBore = 3;
    cout << "[hydraulic_cyinder] W: Lower limit of bore diameter (3mm) readched"
      << endl;
    }

  // Output disabled for study
  //cout << "Calculated bore diameter = " << dBore << " mm" << endl;
}

void hydraulic_cylinder::calculate_rod_diameter()
{
  dRod = sqrt(rodForce / 650) * 4; //[mm]
  // Scale rod according to our current configuration

  // avoid rod diameters below 2 mm
  if (dRod < 1.5)
  {
    dRod = 1.5;
    cout << "[hydraulic_cylinder] W: Lower limit of rod diameter (2mm) reached"
      << endl;
  }

  //cout << "Calculated rod diameter = " << dRod << " mm" << endl;
}

void hydraulic_cylinder::calculate_stroke()
{
  stroke = fluidVolume / (pow(dBore/2, 2) * M_PI);
}

void hydraulic_cylinder::calculate_wall_thickness()
{
  // Berechnung der required wall thickness erfolgt nach dem Kesselgesetz
  // Als Grundlage wird verwendet dass der Master Cylinder mit 14 mm bore diamet
  // er und 1 mm Wandstärke hält.
  tWall = 1 / 14 * dBore;

  // Limit the minimal wall thickness to 1 mm, since lower thickness leads to di
  // fficulties in manufacturing
  if (tWall < 1)
    tWall = 1;

}

void hydraulic_cylinder::calculate()
{
  if (statusStroke && statusRodForce)
  {
    // Case master cylinder: output stroke and force are given
    calculate_rod_diameter();
    calculate_bore_diameter();
  }
  else if (statusFluidVolume && statusRodForce && statusRodBoreRatio)
  {
    calculate_bore_diameter();
    calculate_stroke();
    dRod = dBore * rodBoreRatio;
  }
  else
  {
    cout << "[hydraulic_cylinder] E: Cannot Calculate cylinder based on this in"
      "put!" << endl;
  }
  // Known for sure at this point:
  // - dBore
  // - dRod
  // - stroke
  //cout << "Bore diameter = " << dBore << " mm" << endl;

  calculate_wall_thickness();

  calculate_mass();
  calculate_outer_volume();

  if (dBore < dRod)
    cout << "[hydraulic_cylinder] Rod diameter cannot be bigger than bore diame"
      "ter!" << endl;

}

float hydraulic_cylinder::get_rod_bore_ratio()
{
  rodBoreRatio = dRod / dBore;
}

float hydraulic_cylinder::get_fluid_volume()
{
  fluidVolume = pow(dBore/2, 2) * M_PI * stroke; // [mm^3]
  return fluidVolume;
}

float hydraulic_cylinder::get_extended_length()
{
  extendedLength = 2 * stroke + 2 * tLid + tORing + tPiston + addRodLength;
  return extendedLength;
}

float hydraulic_cylinder::get_outer_diameter()
{
  return (dBore + 2 * tWall);
}
