/*
  file    linear_motor_selector.h
  author  Urs Hofmann
  date    25. Dez 2017
  mail    urshofmann@gmx.net
*/

#include "linear_motor_selector.h"

void linear_motor_selector::set_req_force(const float& _reqForce)
{
  reqForce = _reqForce;
}

void linear_motor_selector::set_req_speed(const float& _reqSpeed)
{
  reqSpeed = _reqSpeed;
}

void linear_motor_selector::read_in_and_sort_database()
{
  vector<vector<float>> LinearMotorData =
    myFile.read_file("databases/linear_motors.dat");

  LinearMotorDatabase.clear();

  unsigned int sizeDatabase = LinearMotorData.size();
  //cout << " Reading in " << sizeDatabase << " motors from database" << endl;

  for(unsigned int iMotor = 0; iMotor < sizeDatabase; iMotor++)
  {
    linearMotorTemp.set_force(LinearMotorData[iMotor][0]);
    linearMotorTemp.set_stroke(LinearMotorData[iMotor][1]);
    linearMotorTemp.set_speed(LinearMotorData[iMotor][2]);
    linearMotorTemp.set_mass(LinearMotorData[iMotor][3]);
    linearMotorTemp.set_volume(LinearMotorData[iMotor][4]);
    linearMotorTemp.set_voltage(LinearMotorData[iMotor][5]);
    linearMotorTemp.set_current(LinearMotorData[iMotor][6]);

    LinearMotorDatabase.push_back(linearMotorTemp);
  }
}

void linear_motor_selector::exclude_weak_motors()
{
  unsigned int iMotor = 0;

  while (iMotor < LinearMotorDatabase.size())
  {
    if (LinearMotorDatabase[iMotor].get_power() <
      (0.5 * reqSpeed * reqForce / 1000))
    {
      LinearMotorDatabase.erase(LinearMotorDatabase.begin()+iMotor);
    }
    else
    {
      iMotor++;
    }
  }

  if (LinearMotorDatabase.size() == 0)
  {
    cout << "[linear_motor_selector] W: No matching motors found in database"
       << endl;
  }
}

vector<linear_motor> linear_motor_selector::get_linear_motors()
{
  read_in_and_sort_database();
  exclude_weak_motors();
  return LinearMotorDatabase;
}
