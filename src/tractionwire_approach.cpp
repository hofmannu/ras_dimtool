#include "tractionwire_approach.h"

using namespace std;

// Set requirements for remote actuation system as defined by user

void tractionwire_approach::set_requirements(const requirements&
    _myRequirements)
{
  myRequirements = _myRequirements;
  status_requirements = 1;
}

// Main calculation procedure

void tractionwire_approach::calculate()
{
  // Check if requirements were already set
  if (status_requirements == 0)
  {
    cout << "E: Cannot calculate tractionwire-approach w/o knowing requirements"
      << endl;
  }
  else
  {
    //cout << "   Start tractionwire calculation" << endl;
    // Traction wires / transmission system
    myTractionwire.set_length(myRequirements.get_transmission_length());
    myTractionwire.set_bending_angle(myRequirements.get_bending_angle());
    myTractionwire.calculate();

    myProperties.set_mass_transmissionsystem(2*myTractionwire.get_mass());
    myProperties.set_volume_transmissionsystem(2*myTractionwire.get_volume()); // [mm^3]

    // Output winch including bearings, gear and rack
    // Note: traction wire calculation is required before since we need the
    // effectivity to calculate the output winch
    myOutputwinch.set_output_force(myRequirements.get_output_force());
    myOutputwinch.set_rack_length(myRequirements.get_stroke());
    // Simplification rack length = stroke
    myOutputwinch.set_eff_trans_system(myTractionwire.get_efficiency());
    myOutputwinch.calculate();

    // Read components calculated by myOutputwinch
    myBearing = myOutputwinch.get_bearing();
    myRack = myOutputwinch.get_rack();

    myProperties.set_mass_output(
        myBearing.get_mass() * 2 +
        myRack.get_mass() +
        myOutputwinch.get_mass_winch() +
        myOutputwinch.get_mass_housing());

    myProperties.set_volume_output(myRack.get_volume()+
      myOutputwinch.get_volume()); // [mm^3]

    // Rope tension at input winch
    FRopeInput = myOutputwinch.get_rope_tension_at_input();
    vRopeInput = myRequirements.get_output_speed() *
      myOutputwinch.get_radius_winch() / myOutputwinch.get_radius_pinion();
      // [mm/s]

    // Select stepper motor based on database file
    myStepperMotorSelector.set_req_rope_tension(FRopeInput); // [N]
    myStepperMotorSelector.set_req_rope_speed(vRopeInput); // [mm/s]
    myStepperMotorDatabase = myStepperMotorSelector.select_stepper_motor();
    // Feeds back a vector containing multiple stepper motors which all fulfill
    // the requirements

    // Dimension input winch, housing, and battery for each stepper motor
    myInputWinchDatabase.clear();
    myHousingDatabase.clear();
    myBatteryDatabase.clear();

    vector<float> m;
    vector<float> V;

    for (int iStepper = 0; iStepper < myStepperMotorDatabase.size(); iStepper++)
    {
      // Calculate input winch for iStepper
      myInputWinch.set_effective_motor_torque(
          myStepperMotorDatabase[iStepper].get_torque());
      myInputWinch.set_rope_tension(FRopeInput);
      myInputWinch.calculate();
      myInputWinchDatabase.push_back(myInputWinch);

      // Calculate housing for iStepper
      myHousing.set_winch_diameter(
          myInputWinchDatabase[iStepper].get_outer_radius() * 2);
      myHousing.calculate();
      myHousingDatabase.push_back(myHousing);

      // Calculate battery
      float tMoving = myRequirements.get_t_moving();
      float tHolding = myRequirements.get_t_holding();

      float IMoving = myStepperMotorDatabase[iStepper].get_current();
      float IHolding = IMoving / 2;

      float energyTemp = (IMoving * tMoving + IHolding * tHolding) *
        myStepperMotorDatabase[iStepper].get_voltage(); // [Ws]
      energyTemp = sFactorBattery * energyTemp / 60 / 60; // [Wh]
      // Incl. security factor of 2 (Battery cannot be fully discharged)

      myBattery.set_energy(energyTemp);
      myBatteryDatabase.push_back(myBattery);

      m.push_back(myStepperMotorDatabase[iStepper].get_mass() +
        myInputWinchDatabase[iStepper].get_mass() +
        myHousingDatabase[iStepper].get_mass() +
        myBatteryDatabase[iStepper].get_mass());
      V.push_back(myStepperMotorDatabase[iStepper].get_volume() +
        myHousingDatabase[iStepper].get_volume() +
        myBatteryDatabase[iStepper].get_volume());
    }

    // Select lightest and smallest solution
    int idMMin = 0;
    int idVMin = 0;

    for (unsigned int iOptions = 0; iOptions < m.size(); iOptions++)
    {
      if(m[idMMin] > m[iOptions])
        idMMin = iOptions;

      if(V[idVMin] > V[iOptions])
        idVMin = iOptions;
    }

    // Define final ID to store the selection
    int finalId;

    if (idMMin == idVMin) // if lightest and smallest solution are the same
    {
      finalId = idMMin;
    }
    else // otherwise ask user so select for what to optimize
    {
      unsigned int selection = 0;
      if(doInteractive)
      {
        while(selection != 1 && selection != 2)
        {
          cout << "[tractionwire_approach] Would you like to optimize mass or v"
          "olume at the output?" << endl;
          cout << " [1] Mass" << endl;
          cout << " [2] Volume" << endl;
          cin >> selection;
        }

        if(selection == 1)
          finalId = idMMin;

        if(selection == 2)
          finalId = idVMin;
      }
      else // if not interactive, optimize for mass
      {
        finalId = idMMin;
      }
    }

    // Get selected combination
    myStepperMotor = myStepperMotorDatabase[finalId];
    myHousing = myHousingDatabase[finalId];
    myBattery = myBatteryDatabase[finalId];

    myProperties.set_mass_actuationunit(m[finalId]);
    myProperties.set_volume_actuationunit(V[finalId]);

  }
}

// Calculate output winch
void tractionwire_approach::calculate_output_winch()
{
  // Set length of rack
  myOutputwinch.set_rack_length(myRequirements.get_stroke()+30);

  // Set output force
  myOutputwinch.set_output_force(myRequirements.get_output_force());

  // Set efficiency of tractionwires
  myOutputwinch.set_eff_trans_system(myTractionwire.get_efficiency());

  myOutputwinch.calculate();

  myBearing = myOutputwinch.get_bearing();
}

void tractionwire_approach::print_output()
{
  // Open file to write results in and give back error if not open
  output.open("results/tractionwire_results.txt");
  if(output.is_open() == 0)
  {
    cout << "E: Could not open output file" << endl;
  }
  else
  {

  output << " Traction-wire based approach" << endl;

  // Print defined requirements to file
  output << endl << "[[Requirements]]" << endl;
	output << endl << "Output force = " << myRequirements.get_output_force()
    << " N"	<< endl;
	output << "Output speed = " << myRequirements.get_output_speed() << " mm/s"
    << endl;
	output << "Runtime = " << myRequirements.get_runtime() << " h" << endl;
	output << "Number of strokes = " << myRequirements.get_no_strokes() << endl;
	output << "Stroke = " << myRequirements.get_stroke() << " mm" << endl;
	output << "Transmission length = " << myRequirements.get_transmission_length()
		<< " mm" << endl;
	output << "Active time = " << myRequirements.get_active_time() << endl;
	output << "Bending angle = " << myRequirements.get_bending_angle() << endl;

  output << endl << "[[Output]]" << endl;

  output << endl << "[Bearings]" << endl;
  output << endl << "Outer radius = " << myBearing.get_outer_radius() << " mm"
    << endl;
  output << "Inner radius = " << myBearing.get_inner_radius() << " mm" << endl;
  output << "Height = " << myBearing.get_height() << " mm" << endl;
  output << "Mass (of both) = " << 2*myBearing.get_mass() << " g" << endl;

  output << endl << "[Rack]" << endl;
  output << endl << "Height = " << myRack.get_height() << " mm" << endl;
  output << "Width = " << myRack.get_width() << " mm" << endl;
  output << "Mass = " << myRack.get_mass() << " g" << endl;
  output << "Volume = " << myRack.get_volume()/1000 << " cm^3" << endl;

  output << endl << "[Output winch]" << endl;
  output << endl << "Mass = " << myOutputwinch.get_mass_winch() << " g" << endl;
  output << "Radius of output winch = " << myOutputwinch.get_radius_winch()/10
    << " cm" << endl;

  output << endl << "[Housing]" << endl;
  output << endl << "Mass = " << myOutputwinch.get_mass_housing() << " g"
    << endl;

  output << endl << "Mass output = " << myProperties.get_mass_output() << " g" << endl;
  output << "Volume output = " << myProperties.get_volume_output()/1000 << " cm^3" << endl;


  output << endl << "[[Transmission system]]" << endl;

  output << endl << "[Traction wires]" << endl;

  output << endl << "Mass = " << myProperties.get_mass_transmission_system() << " g" << endl;
  output << "Volume = " <<  myProperties.get_volume_transmission_system()/1000 << " cm^3" << endl;

  output << endl << "[[Actuation unit]]" << endl;

  output << endl << "[Stepper motor]" << endl;
  output << endl << "Mass = " << myStepperMotor.get_mass() << " g" << endl;
  output << "Volume = " << myStepperMotor.get_volume()/1000 << " cm^3" << endl;

  output << endl << "[Input winch (inkl. bearings)]" << endl << endl;

  output << "Radius of input winch = " << myInputWinch.get_winch_radius() << " mm" << endl;
  output << "Mass = " << myInputWinch.get_mass() << " g" << endl;

  output << endl << "[Housing (inkl. SEEs)]" << endl;
  output << endl << "Mass = " << myHousing.get_mass() << " g" << endl;
  output << "Volume = " << myHousing.get_volume()/1000 << " cm^3" << endl;

  output << endl << "[Battery]" << endl;
  output << endl << "Mass = " << myBattery.get_mass() << " g" << endl;
  output << "Volume = " << myBattery.get_volume() / 1000 << " cm^3" << endl;
  output << "Capacity = " << myBattery.get_energy() << " Wh" << endl << endl;

  output << "Overall mass of actuation unit = " << myProperties.get_mass_actuation_unit()
    << " g" << endl;
  output << "Volume of actuation unit = " << myProperties.get_volume_actuation_unit() / 1000
    << " cm^3" << endl;


  // Close output file
  output.close();
  }
}

void tractionwire_approach::set_do_interactive(const bool& _doInteractive)
{
  doInteractive = _doInteractive;

}
