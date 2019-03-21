/*
  file    hydraulic_approach.h
  author  Urs Hofmann
  date    25. Dez 2017
  mail    urshofmann@gmx.net
*/

#include "requirements.h"
#include "hydraulic_cylinder.h"
#include "hydraulic_tube.h"
#include "linear_motor.h"
#include "linear_motor_selector.h"
#include "battery.h"
#include "hydraulic_housing_bk.h"
#include "force_gauge.h"
#include "force_gauge_selector.h"
#include <vector>
#include "ras_properties.h"

#ifndef HYDRAULIC_APPROACH_H
#define HYDRAULIC_APPROACH_H

using namespace std;

class hydraulic_approach
{
private:
  bool doInteractive = 0;

  float hoseComplianceCompFactor = 1.1; // Factor compensating for compliance in
    // hydraulic hoses

  float sFactorBattery = 1.7;
    // Battery should never be fully discharged --> security factor

  ofstream output;
  ras_properties myProperties;

  hydraulic_cylinder mySlaveCylinder;

  hydraulic_cylinder myMasterCylinder;
  vector<hydraulic_cylinder> myMasterCylinderDatabase;

  battery myBattery;
  vector<battery> myBatteryDatabase;

  hydraulic_housing_bk myHousing;
  vector<hydraulic_housing_bk> myHousingDatabase;

  force_gauge myForceGauge;
  vector<force_gauge> myForceGaugeDatabase;

  hydraulic_tube myHydraulicTube;

  linear_motor myLinearMotor;
  vector<linear_motor> myLinearMotorDatabase;
  linear_motor_selector myLinearMotorSelector;

  requirements myRequirements;

public:
  void set_requirements(const requirements& _myRequirements);
  void calculate();
  void print_output();
  ras_properties get_properties() const {return myProperties;};

};

#endif
