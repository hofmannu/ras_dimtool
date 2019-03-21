/*
File 			pneumatic_approach.h
Author 		Urs Hofmann
Date 			15. Nov 2017
*/


#include "requirements.h"
#include "file_handling.h"
#include <vector>
#include "battery.h"
#include "ras_properties.h"
#include "pneumatic_valve.h"
#include "pneumatic_tube.h"
#include "pneumatic_tube_selector.h"

struct pneumatic_cylinder
{
	float d_cylinder;
	float d_cylinderrod;
	float stroke;
	float mass;
	float volume;
};


/* Pneumatic airtank
 * Describes all the important properties which the air tank has.
 */

struct pneumatic_airtank
{
	float mass; // [g]
	float volume; // [mm^3]
	float storage_pressure; // [bar]
	float storage_volume; // [mm^3]
};


class pneumatic_approach
{
private:
	ofstream output;
	requirements myRequirements;
	file_handling myFile;
	vector<pneumatic_cylinder> myCylinderDatabase;
	vector<pneumatic_airtank> myAirtankDatabase;
	ras_properties myRas_properties;

	/* Working pressure of pneumatic cylinder
	 * The maximum value is 14 bar and is set in the beginning. However at lower forces it
	 * might be necessary to work with lower forces to not waste energy. Therefore, it
	 * might be changed in void recheck_working_pressure().
	 */

	float p_working = 14; // [bar]
	float mass_connector = 1; // [g]

	// Thresholds for exclusion
	float dmin_cylinder; // min bore diameter [mm]
	float min_e_storage; // bar * mm^3

	// Components
	pneumatic_cylinder myCylinder;
	pneumatic_tube_selector myTubeSelector;
	pneumatic_tube myTube;
	pneumatic_airtank myAirtank;
	pneumatic_valve myValve;
	battery myBattery;

	// Output calculation
	void calc_mincylinder_diameter();
	void select_cylinder();
	void recheck_working_pressure();

	// Transmission system calculation
	void calc_transmission_system();

	// Actuation unit calculation
	void calc_mintank_energy();
	void select_airtank();
	void calc_battery();
	void calc_mass_volume_act_unit();


public:
	void set_requirements(const requirements& _requirements);

	// Runs the whole calculation
	void calculate();
	void print_output();
	ras_properties get_properties() const {return myRas_properties;};

};
