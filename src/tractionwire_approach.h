#include "requirements.h"
#include "tractionwire.h"
#include "output_winch.h"
#include "input_winch.h"
#include <iostream>
#include "rack.h"
#include "rack_selector.h"
#include "bearing.h"
#include "stepper_motor_selector.h"
#include "stepper_motor.h"
#include "tractionwire_housing_bk.h"
#include "battery.h"
#include "ras_properties.h"

#ifndef TRACTIONWIRE_APPROACH_H
#define TRACTIONWIRE_APPROACH_H

class tractionwire_approach
{
private:

  bool doInteractive = 0;

  // File to write all information about the resulting RAS
  ofstream output;

  ras_properties myProperties;

  // Requirements for the RAS defined by the user
  requirements myRequirements;
  bool status_requirements = 0;

  // Parts
  tractionwire myTractionwire; // two identical tractionwires

  // Output winch containing two bearings, gear, and rack
  output_winch myOutputwinch;

  // Input winch positioned within the backpack
  vector<input_winch> myInputWinchDatabase;
  input_winch myInputWinch;

  bearing myBearing;
  rack myRack;

  stepper_motor_selector myStepperMotorSelector;
  vector<stepper_motor> myStepperMotorDatabase;
  stepper_motor myStepperMotor;

  vector<tractionwire_housing_bk> myHousingDatabase;
  tractionwire_housing_bk myHousing;

  vector<battery> myBatteryDatabase;
  battery myBattery;

  float FRopeInput; // [N]
  float vRopeInput; // [mm / s]

  float sFactorBattery = 1.7;
    // Battery should never be fully discharged --> security factor

  void calculate_output_winch();

public:
    void set_requirements(const requirements& _myRequirements);
    void calculate();
    void print_output();
    void set_do_interactive(const bool& _doInteractive);

    ras_properties get_properties() const {return myProperties;};
};

#endif
