/*
File 			pneumatic_approach.h
Author 		Urs Hofmann
Date 			15. Nov 2017
*/

#include "pneumatic_approach.h"
#include <math.h>

using namespace std;

void pneumatic_approach::set_requirements(const requirements& _requirements)
{
	myRequirements = _requirements;
}

void pneumatic_approach::calculate()
{

	// Output calculation
	calc_mincylinder_diameter();
	select_cylinder();
	recheck_working_pressure();

	// Transmission system calculation
	calc_transmission_system();

	// Actuation unit calculation
	calc_mintank_energy();
	select_airtank();
	calc_battery();
	calc_mass_volume_act_unit();
}

// Calculate required bore diameter of pneumatic cylinder as a function of working
// pressure [bar] and required output force [N]
void pneumatic_approach::calc_mincylinder_diameter()
{
	float friction_comp_factor = 1.2;
	dmin_cylinder = sqrt(myRequirements.get_output_force() * friction_comp_factor * 4
		/ (p_working * M_PI) * 10);
	// sqrt([N] / [bar]) = sqrt([N] / [N] * [m^2] * 10^-5] = sqrt([mm]2 *10
	//cout << " Minimum bore diameter of pneumatic cylinder: " << dmin_cylinder
	//	<< " mm" << endl;
}

void pneumatic_approach::select_cylinder()
{

	// Read from cylinder database
	vector<vector<float>> cylinder_data =
		myFile.read_file("databases/pneumatic_cylinders.dat");
	pneumatic_cylinder cyl_temp;

	// Clear database
	myCylinderDatabase.clear();

	unsigned int size_database = cylinder_data.size();
	//cout << " Number of cylinders in database: " << size_database << endl;

	for(unsigned int i = 0; i < size_database; i++)
	{
		cyl_temp.d_cylinder = cylinder_data[i][0];
		cyl_temp.d_cylinderrod = cylinder_data[i][1];
		cyl_temp.stroke = cylinder_data[i][2];
		cyl_temp.mass = cylinder_data[i][3];
		cyl_temp.volume = cylinder_data[i][4];

		myCylinderDatabase.push_back(cyl_temp);
	}


	// Exclude cylinders with too small bore diameter or too small stroke
	unsigned int id = 0;
	while(id < myCylinderDatabase.size())
	{
		if(myCylinderDatabase[id].d_cylinder < dmin_cylinder)
		{
			myCylinderDatabase.erase(myCylinderDatabase.begin()+id);
		}
		else if (myCylinderDatabase[id].stroke < myRequirements.get_stroke())
		{
			myCylinderDatabase.erase(myCylinderDatabase.begin()+id);
		}
		else
		{
			id++;
		}
	}

	if(myCylinderDatabase.size() == 0)
		cout << " W: No matching pneum. cylinder found in database!" << endl;
	else
	{
		// In remaining cylinders: find lowest weight and volume (?)
		int id_lvol;
		float lvol = 1000000;
		int id_lmass;
		float lmass = 1000000;

		for (unsigned int i = 0; i < myCylinderDatabase.size(); i++)
		{
			if(myCylinderDatabase[i].mass < lmass)
			{
				lmass = myCylinderDatabase[i].mass;
				id_lmass = i;
			}
			if(myCylinderDatabase[i].volume < lvol)
			{
				lvol = myCylinderDatabase[i].volume;
				id_lvol = i;
			}
		}

		if (id_lvol == id_lmass)
		{
			//cout << "Cylinder with lowest mass and volume is the same" << endl;
			myCylinder = myCylinderDatabase[id_lvol];
		}
		else
		{
			int selection = 0;

			while(selection != 1 && selection != 2)
			{
				cout << "Would you like to optimize mass or volume at the output?" << endl;
				cout << " [1] Mass" << endl;
				cout << " [2] Volume" << endl;
				cin >> selection;
			}

			if(selection == 1)
				myCylinder = myCylinderDatabase[id_lmass];

			if(selection == 2)
				myCylinder = myCylinderDatabase[id_lvol];
		}



	}

	myRas_properties.set_mass_output(myCylinder.mass + 2 * mass_connector);
	myRas_properties.set_volume_output(myCylinder.volume);

}

void pneumatic_approach::recheck_working_pressure()
{
	float area = pow(myCylinder.d_cylinder, 2) / 4 * M_PI;
	p_working = myRequirements.get_output_force() * 1.5 / area * 10; // [N] / [mm^2] / 10 = [bar]
	//cout << " Work. pressure [bar]:\t" << p_working << endl;
	// 1.5 is security factor compensating for friction
}

void pneumatic_approach::calc_transmission_system()
{
	//cout << " Start calculating the transmission system..." << endl;
	myTube = myTubeSelector.select_pneumatic_tube(myRequirements, p_working,
		myCylinder.d_cylinder);
	myTube.set_length(myRequirements.get_transmission_length());
	myTube.calculate_vol_and_mass();
	myRas_properties.set_mass_transmissionsystem(myTube.get_mass()*2);
	myRas_properties.set_volume_transmissionsystem(myTube.get_outer_volume()*2);
}

void pneumatic_approach::calc_mintank_energy()
{
	float area_push = pow(myCylinder.d_cylinder, 2) * M_PI / 4; // [mm^2]
	float area_pull = area_push - pow(myCylinder.d_cylinderrod , 2) * M_PI / 4; // [mm^2]
	float volume_cycle_cyl = (area_pull + area_push) * myCylinder.stroke; // [mm^3]
	float overall_stroke_volume = volume_cycle_cyl + myTube.get_inner_volume() * 2; // [mm^3]

	min_e_storage = p_working * overall_stroke_volume * myRequirements.get_no_strokes();
	// [bar] * [mm^3]
}

void pneumatic_approach::select_airtank()
{
	vector<vector<float>> airtank_data = myFile.read_file("databases/pneumatic_airtank.dat");

	pneumatic_airtank tank_temp;

	myAirtankDatabase.clear();

	unsigned int size_database = airtank_data.size();
	//cout << " Number of airtanks in database: " << size_database << endl;

	for (unsigned int i = 0; i < size_database; i++)
	{
		tank_temp.storage_pressure = airtank_data[i][0]; // [bar]
		tank_temp.storage_volume = airtank_data[i][1]; // [mm^3]
		tank_temp.volume = airtank_data[i][2]; // [mm^3]
		tank_temp.mass = airtank_data[i][3]; // [g]

		myAirtankDatabase.push_back(tank_temp);
	}

	// Exclude airtanks not offering sufficient energy for number of strokes
	unsigned int id;
	while(id < myAirtankDatabase.size())
	{
		float energy = myAirtankDatabase[id].storage_pressure *
			myAirtankDatabase[id].storage_volume;
		if(energy < min_e_storage)
			myAirtankDatabase.erase(myAirtankDatabase.begin()+id);
		else
			id++;
	}

	if (myAirtankDatabase.size() == 0)
		cout << " W: No air tank found fulfilling the energy requirements!" << endl;
	else
	{
		int id_lvol;
		int id_lmass;
		float lvol = 9999999999999999;
		float lmass = 99999999999999;

		// FInd lowest mass and volume and corresponding ID
		for(unsigned int i = 0; i < myAirtankDatabase.size(); i++)
		{
			if (myAirtankDatabase[i].mass < lmass)
			{
				lmass = myAirtankDatabase[i].mass;
				id_lmass = i;
			}
			if (myAirtankDatabase[i].volume < lvol)
			{
				lvol = myAirtankDatabase[i].volume;
				id_lvol = i;
			}
		}

		if (id_lvol == id_lmass)
		{
			//cout << " Air tank with lowest mass and volume are the same" << endl;
			myAirtank = myAirtankDatabase[id_lvol];
		}
		else
		{
			int selection = 0;

			while(selection != 1 && selection != 2)
			{
				cout << "Would you like to optimize mass or volume at the output?" << endl;
				cout << " [1] Mass" << endl;
				cout << " [2] Volume" << endl;
				cin >> selection;
			}

			if(selection == 1)
				myAirtank = myAirtankDatabase[id_lmass];

			if(selection == 2)
				myAirtank = myAirtankDatabase[id_lvol];
		}



	}
}

void pneumatic_approach::calc_battery()
{
	myValve.set_current(1); // [A]
	myValve.set_voltage(12); // [V]
	float req_energy = myValve.get_current() * myValve.get_voltage() *
	myRequirements.get_runtime() * myRequirements.get_active_time();
	// [Wh]
	// cout << "Required energy: " << req_energy << endl;
	myBattery.set_energy(req_energy);


}

void pneumatic_approach::calc_mass_volume_act_unit()
{

	// weight and volume of valve are fixed in this version of the program
	myValve.set_mass(298 / 2); //[g]
	myValve.set_volume(87000 / 2); //[mm^3]

	float reg2_mass = 134; // [g]
	float reg2_vol = 51500; // [mm^3]

	myRas_properties.set_mass_actuationunit(myValve.get_mass() * 2 + myBattery.get_mass()
		+ myAirtank.mass + reg2_mass);
	myRas_properties.set_volume_actuationunit(myValve.get_volume() * 2 +
		myBattery.get_volume() + myAirtank.volume + reg2_vol);
}

void pneumatic_approach::print_output()
{

	// Open output file
	output.open("results/pneumatic_results.txt");
	output << " Pneumatic approach" << endl;

	output << endl << "[[Requirements]]" << endl;
	output << endl << "Output force = " << myRequirements.get_output_force() << " N"
		<< endl;
	output << "Output speed = " << myRequirements.get_output_speed() << " mm/s" << endl;
	output << "Runtime = " << myRequirements.get_runtime() << " h" << endl;
	output << "Number of strokes = " << myRequirements.get_no_strokes() << endl;
	output << "Stroke = " << myRequirements.get_stroke() << " mm" << endl;
	output << "Transmission length = " << myRequirements.get_transmission_length()
		<< " mm" << endl;
	output << "Active time = " << myRequirements.get_active_time() << endl;
	output << "Bending angle = " << myRequirements.get_bending_angle() << endl;

	output << endl << "[[Output]]" << endl;

	output << endl << "[Pneumatic cylinder]" << endl << endl;

	output << "Bore diameter = " << myCylinder.d_cylinder << " mm" << endl;
	output << "Rod diameter = " << myCylinder.d_cylinderrod << " mm" << endl;
	output << "Stroke = " << myCylinder.stroke << " mm" << endl;

	output << endl << "Note: Overall mass includes 2 g for pneumatic connectors";
	output << endl << "Overall mass output = " << myRas_properties.get_mass_output() <<
		" g" << endl;
	output << "Overall volume output = " << myRas_properties.get_volume_output() / 1000
		<< " cm^3" << endl;

	output << endl << "[[Transmission system]]" << endl;

	output << endl << "[Pneumatic tubes]" << endl << endl;
	output << "Inner diameter = " << myTube.get_inner_diameter() << " mm" << endl;
	output << "Outer diameter = " << myTube.get_outer_diameter() << " mm" << endl;

	output << endl << "Overall mass transmission system = " <<
		myRas_properties.get_mass_transmission_system() << " g" << endl;
	output << "Overall volume transmission system = " <<
		myRas_properties.get_volume_transmission_system() / 1000 << " cm^3" << endl;

	output << endl << "[[Actuation unit]]" << endl;

	output << endl << "[Air tank]" << endl;
	output << endl << "Storage pressure = " << myAirtank.storage_pressure << " bar"
		<< endl;
	output << "Storage volume = " << myAirtank.storage_volume / 1000000 <<" L"<< endl;
	output << "Mass = " << myAirtank.mass << " g" << endl;
	output << "Volume = " << myAirtank.volume / 1000 << " cm^3" << endl;

	output << endl << "[Battery]" << endl;
	output << endl << "Battery capacity = " << myBattery.get_energy() << " Wh" << endl;
	output << "Mass = " << myBattery.get_mass() << " g" << endl;
	output << "Volume = " << myBattery.get_volume() / 10000 << " cm^3" << endl;

	output << endl << "[Valves]" << endl;
	output << endl << "Mass = " << myValve.get_mass()*2 << " g" << endl;
	output << "Volume = " << myValve.get_volume()*2 / 1000 << " cm^3" << endl;

	output << endl << "[Regulator]" << endl;
	output << endl << "Mass = " << "134 g" << endl;
	output << "Volume = " << "51.5 cm^3" << endl << endl;

	output << "Overall mass actuation unit = " << myRas_properties.get_mass_actuation_unit() <<
		" g" << endl;
	output << "Overall volume actuation unit = " << myRas_properties.get_volume_actuation_unit() / 1000
		<< " cm^3" << endl;

	output.close();

}
