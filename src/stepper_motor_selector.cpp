/*
  file    stepper_motor_selector.cpp
  author  Urs Hofmann
  date    27. Nov 2017
  mail    urshofmann@gmx.net
*/

#include "stepper_motor_selector.h"

void stepper_motor_selector::set_req_rope_speed(const float& _reqRopeSpeed)
{
  reqRopeSpeed = _reqRopeSpeed; // [mm/s]
}

void stepper_motor_selector::set_req_rope_tension(const float& _reqRopeTension)
{
  reqRopeTension = _reqRopeTension; // [N]
}

void stepper_motor_selector::read_in_and_sort_database()
{
  vector<vector<float>> StepperMotorData =
    myFile.read_file("databases/stepper_motors.dat");

  stepper_motor stepperMotorTemp;

  StepperMotorDatabase.clear();

  unsigned int sizeDatabase = StepperMotorData.size();

  for(unsigned int iStepper = 0; iStepper < sizeDatabase; iStepper++)
  {
    stepperMotorTemp.set_holding_torque(StepperMotorData[iStepper][0]); // [Nm]
    stepperMotorTemp.set_current(StepperMotorData[iStepper][1]); // [A]
    stepperMotorTemp.set_voltage(StepperMotorData[iStepper][2]); // [V]
    stepperMotorTemp.set_mass(StepperMotorData[iStepper][3]); // [g]
    stepperMotorTemp.set_volume(StepperMotorData[iStepper][4]); // [cm^3]

    StepperMotorDatabase.push_back(stepperMotorTemp);
  }
}

// Exclude stepper motors which cannot provide sufficient power for the RAS

void stepper_motor_selector::exclude_weak_steppers()
{
  unsigned int iStepper = 0;

  while (iStepper < StepperMotorDatabase.size())
  {
    if (StepperMotorDatabase[iStepper].get_power()
      < (0.5 * reqRopeTension * reqRopeSpeed / 1000))
      // [N * m * 1 / s] ... [mm / s * N / 1000]
      // Divided by two since we are assuming a linear increase in force --> Ave
      // rage mechanical power needed is exactly half of maximum
    {
      StepperMotorDatabase.erase(StepperMotorDatabase.begin()+iStepper);
    }
    else
    {
      iStepper++;
    }
  }

  if (StepperMotorDatabase.size() == 0)
    cout << "[stepper_motor_selector] W: No matching stepper motors found in da"
      "tabase." << endl;
}

void stepper_motor_selector::select_best_matching_stepper()
{
  unsigned int idVol = 0;
  unsigned int idMass = 0;

  float minVol = 1000000; // [mm^3]
  float minMass = 1000000; // [g]

  for (unsigned int iStepper = 0; iStepper < StepperMotorDatabase.size();
    iStepper++)
  {
    if(StepperMotorDatabase[iStepper].get_mass() < minMass)
    {
      minMass = StepperMotorDatabase[iStepper].get_mass();
      idMass = iStepper;
    }
    if(StepperMotorDatabase[iStepper].get_volume() < minVol)
    {
      minVol = StepperMotorDatabase[iStepper].get_volume();
      idVol = iStepper;
    }
  }

  if(idVol == idMass)
  {
    selectedStepperMotor = StepperMotorDatabase[idVol];
  }
  else
  {
    unsigned int selection = 0;

    while(selection != 1 && selection != 2)
    {
      cout << "Optimize mass or volume at the output?" << endl;
      cout << " [1] Mass" << endl;
      cout << " [2] Volume" << endl;
      cin >> selection;
    }

    if(selection == 1)
      selectedStepperMotor = StepperMotorDatabase[idMass];

    if(selection == 2)
      selectedStepperMotor = StepperMotorDatabase[idVol];
  }
}

vector<stepper_motor> stepper_motor_selector::select_stepper_motor()
{
  // Read in and sort database
  read_in_and_sort_database();

  // Exclude steppers which are too weak
  exclude_weak_steppers();

  // Select best matching stepper
  // select_best_matching_stepper();

  return StepperMotorDatabase;
}
