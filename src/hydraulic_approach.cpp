/*
  file    hydraulic_approach.cpp
  author  Urs Hofmann
  date    27. Dez 2017
  mail    urshofmann@gmx.net
*/

#include "hydraulic_approach.h"
#include <math.h>

using namespace std;


void hydraulic_approach::set_requirements(const requirements& _myRequirements)
{
  myRequirements = _myRequirements;
}

void hydraulic_approach::calculate()
{
  // Calculate output
  mySlaveCylinder.set_rod_force(myRequirements.get_output_force());
  mySlaveCylinder.set_stroke(myRequirements.get_stroke());
  mySlaveCylinder.calculate();
  myProperties.set_mass_output(mySlaveCylinder.get_mass());
  myProperties.set_volume_output(mySlaveCylinder.get_outer_volume());

  // Calculate transmission system
  myHydraulicTube.set_length(myRequirements.get_transmission_length());
  float tubeFlow = myRequirements.get_output_speed() * M_PI *
    pow(mySlaveCylinder.get_bore_diameter()/2, 2); // [mm^3 / s]
  myHydraulicTube.set_flow_rate(tubeFlow);
  myHydraulicTube.calculate();
  myProperties.set_mass_transmissionsystem(myHydraulicTube.get_mass()*2);
  myProperties.set_volume_transmissionsystem(myHydraulicTube.get_outer_volume()*2);

  // Calculate actuation unit

  // Read in database of linear motors
  // Include security factor of tqo for friction compensation
  // Exclude weak linear motors
  myLinearMotorSelector.set_req_force(myRequirements.get_output_force() * 2);
  myLinearMotorSelector.set_req_speed(myRequirements.get_output_speed());
  myLinearMotorDatabase = myLinearMotorSelector.get_linear_motors();

  myMasterCylinderDatabase.clear();
  myBatteryDatabase.clear();
  myHousingDatabase.clear();

  // Vector used to store overall mass and volume of actuation system versions
  vector<float> m;
  vector<float> V;

  // for each linear motor we will calculate a actuation unit
  for (unsigned int iMotor = 0; iMotor < myLinearMotorDatabase.size(); iMotor++)
  {
    // calculate master cylinder
    hydraulic_cylinder tempMasterCylinder;
    tempMasterCylinder.set_fluid_volume(mySlaveCylinder.get_fluid_volume()
      * hoseComplianceCompFactor);
    // Set fluid volume for master cylinder including a security factor to compe
    // nsate for compliance within hoses
    tempMasterCylinder.set_rod_bore_ratio(mySlaveCylinder.get_rod_bore_ratio());
    tempMasterCylinder.set_rod_force(myLinearMotorDatabase[iMotor].get_force());
    // Set motor force to master cylinder
    tempMasterCylinder.calculate();
    myMasterCylinderDatabase.push_back(tempMasterCylinder);

    // caluclate battery
    float tMoving = myRequirements.get_t_moving();
    float tHolding = myRequirements.get_t_holding();

    float IMoving = myLinearMotorDatabase[iMotor].get_current();
    float IHolding = IMoving / 2;

    float energyTemp =  (IMoving * tMoving + IHolding * tHolding) *
      myLinearMotorDatabase[iMotor].get_voltage(); // [As * V] = [Ws]
    energyTemp = sFactorBattery * energyTemp / 60 / 60; // [Wh]
    // Incl. security factor of 2 (Battery cannot be fully discharged)
    myBattery.set_energy(energyTemp);
    myBatteryDatabase.push_back(myBattery);

    // Select force gauge
    force_gauge_selector myForceGaugeSelector;
    myForceGaugeSelector.set_req_force_level(
      myLinearMotorDatabase[iMotor].get_force());
    myForceGauge = myForceGaugeSelector.select_force_gauge();
    myForceGaugeDatabase.push_back(myForceGauge);

    // Calculate housing
    float length =
      myMasterCylinderDatabase[iMotor].get_extended_length() +
      myForceGaugeDatabase[iMotor].get_length();
    myHousing.set_length(length);
    myHousing.set_width(myMasterCylinderDatabase[iMotor].get_outer_diameter()+2);
    myHousing.calculate();
    myHousingDatabase.push_back(myHousing);

    // Sum up mass and volume and push them
    m.push_back(
      myForceGaugeDatabase[iMotor].get_mass() +
      myHousingDatabase[iMotor].get_mass() +
      myBatteryDatabase[iMotor].get_mass() +
      myMasterCylinderDatabase[iMotor].get_mass() +
      myLinearMotorDatabase[iMotor].get_mass());

    V.push_back(
      myLinearMotorDatabase[iMotor].get_volume() +
      myHousingDatabase[iMotor].get_volume() + // [cm^3]
      myBatteryDatabase[iMotor].get_volume()/1000);
  }

  // Select lightest and smallest solution
  int idMMin = 0;
  int idVMin = 0;
  int finalId;

  // Search for lightest and smallest solutions
  for (unsigned int iOptions = 0; iOptions < m.size(); iOptions++)
  {
    if(m[idMMin] > m[iOptions])
      idMMin = iOptions;

    if(V[idVMin] > V[iOptions])
      idVMin = iOptions;
  }

  if (idMMin == idVMin)
  {
    //cout << "[hydraulic_approach] Lightest and smallest stepper soltuions are t"
    //  "he same!" << endl;
    finalId = idMMin;
  }
  else
  {
    unsigned int selection = 0;
    if(doInteractive)
    {
      while(selection != 1 && selection != 2)
      {
        cout << "[hydraulic_approach] Would you like to optimize mass or volume a"
        "t the actuation unit?" <<
          endl;
        cout << " [1] Mass" << endl;
        cout << " [2] Volume" << endl;
        cin >> selection;
      }

      if(selection == 1)
        finalId = idMMin;

      if(selection == 2)
        finalId = idVMin;
    }
    else
      finalId = idMMin;
  }

  myBattery = myBatteryDatabase[finalId];
  myHousing = myHousingDatabase[finalId];
  myLinearMotor = myLinearMotorDatabase[finalId];
  myForceGauge = myForceGaugeDatabase[finalId];
  myMasterCylinder = myMasterCylinderDatabase[finalId];

  myProperties.set_mass_actuationunit(m[finalId]);
  myProperties.set_volume_actuationunit(V[finalId]);
}

void hydraulic_approach::print_output()
{
  output.open("results/hydraulic_approach.txt");
  output << " Hydraulic approach" << endl;

  // Print defined requirements to file
  output << endl << "[[Requirements]]" << endl;
	output << endl << "Output force = " << myRequirements.get_output_force() <<
    " N" << endl;
	output << "Output speed = " << myRequirements.get_output_speed() << " mm/s" <<
    endl;
	output << "Runtime = " << myRequirements.get_runtime() << " h" << endl;
	output << "Number of strokes = " << myRequirements.get_no_strokes() << endl;
	output << "Stroke = " << myRequirements.get_stroke() << " mm" << endl;
	output << "Transmission length = " << myRequirements.get_transmission_length()
		<< " mm" << endl;
	output << "Active time = " << myRequirements.get_active_time() << endl;
	output << "Bending angle = " << myRequirements.get_bending_angle() << endl;

  output << endl << "[[Output]]" << endl << endl;

  output << "Bore diameter = " << mySlaveCylinder.get_bore_diameter() << " mm"
    << endl;
  output << "Mass output = " << myProperties.get_mass_output() << " g" << endl;
  output << "Volume output = " << myProperties.get_volume_output()/1000 << " cm^3" << endl;

  output << endl << "[[Transmission system]]" << endl << endl;

  output << "Inner radius = " << myHydraulicTube.get_inner_radius() << " mm" <<
    endl;
  output << "Outer radius = " << myHydraulicTube.get_outer_radius() << " mm" <<
    endl;
  output << "Mass transmission system = " << myProperties.get_mass_transmission_system()
    << " g " << endl;
  output << "Volume transmission system = " << myProperties.get_volume_transmission_system()/1000
    << " cm^3" << endl;

  output << endl << "[[Actuation unit]]" << endl;

  output << endl << "[Linear motor] " << endl;
  output << "Mass = " << myLinearMotor.get_mass() << " g" << endl;
  output << "Volume = " << myLinearMotor.get_volume()/1000 << " cm^3" << endl;
  output << "Max. force = " << myLinearMotor.get_force() << " N" << endl;

  output << endl << "[Force gauge]" << endl;
  output << "Mass = " << myForceGauge.get_mass() << " g" << endl;
  output << "Max. force = " << myForceGauge.get_max_force() << " g" << endl;

  output << endl << "[Battery]" << endl;
  output << "Mass = " << myBattery.get_mass() << " g" << endl;
  output << "Volume = " << myBattery.get_volume()/1000 << " cm^3" << endl;
  output << "Energy = " << myBattery.get_energy() << " Wh" << endl;

  output << endl << "[Master cylinder]" << endl;
  output << "Mass = " << myMasterCylinder.get_mass() << " g" << endl;
  output << "Volume = " << myMasterCylinder.get_outer_volume()/1000 << " cm^3" <<
    endl;
  output << "Bore diameter = " << myMasterCylinder.get_bore_diameter() << " mm"
    << endl << endl;

  output << "Overall mass actuation unit = " << myProperties.get_mass_actuation_unit() << " g" << endl;
  output << "Overall volume actuation unit = " << myProperties.get_volume_actuation_unit()/1000 << " cm^3" << endl;


  output.close();
}
