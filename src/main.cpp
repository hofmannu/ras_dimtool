#include <iostream>
#include "requirements.h"
#include "pneumatic_approach.h"
#include "tractionwire_approach.h"
#include "hydraulic_approach.h"
#include <vector>
#include "ras_properties.h"
#include <thread>
#include <chrono>

typedef std::chrono::high_resolution_clock Clock;

requirements myRequirements;
pneumatic_approach myPneumaticApproach;
tractionwire_approach myTractionwireApproach;
hydraulic_approach myHydraulicApproach;

using namespace std;

// Compare three values and return the number with the LOWEST value
int compare3(const float& number0, const float& number1, const float& number2)
{
	int result;
	if ((number0 < number1) && (number0 < number2))
		result = 0;
	else if ((number1 < number0) && (number1 < number2))
		result = 1;
	else if ((number2 < number1) && (number2 < number0))
		result = 2;

	return result;
}

int main(int argc, char **argv)
{

	cout << " Welcome to RAS selection tool " << endl;
	int menu_selector = 99;
	int submenu_selector = 99;

	while(menu_selector > 0)
	{
		submenu_selector = 99;

		cout << " --- Main menu --- " << endl;
		cout << " [1] New calculation " << endl;
		cout << " [2] Database submenu " << endl;
		cout << " [3] Study " << endl;
		cout << " [0] Quit" << endl;
		cin >> menu_selector;

		if (menu_selector == 1)
		{
			while (submenu_selector > 0)
			{
				cout << " --- [1] New calculation --- " << endl;
				cout << " [1] Enter requirements " << endl;
				cout << " [2] Calculate pneumatic approach" << endl;
				cout << " [3] Calculate hydraulic approach" << endl;
				cout << " [4] Calculate traction wire based approach" << endl;
				cout << " [0] Back to main menu" << endl;
				cin >> submenu_selector;

				// Read in requirements
				if (submenu_selector == 1)
				{
					// Read in requirements and check for validity
					myRequirements.read_in_user_input();
				}

				if (myRequirements.get_status() == 1 && submenu_selector > 0)
				{
					// Start pneumatic simulation
					if (submenu_selector == 2)
					{
						myPneumaticApproach.set_requirements(myRequirements);
						myPneumaticApproach.calculate();
						myPneumaticApproach.print_output();
					}
					// Start hydraulic simulation
					if (submenu_selector == 3)
					{
						myHydraulicApproach.set_requirements(myRequirements);
						myHydraulicApproach.calculate();
						myHydraulicApproach.print_output();
					}
					// Start tractionwire simulation
					if (submenu_selector == 4)
					{
						myTractionwireApproach.set_requirements(myRequirements);
						myTractionwireApproach.calculate();
						myTractionwireApproach.print_output();
					}
				}
				else if (myRequirements.get_status() == 0 && submenu_selector > 0)
				{
					cout << "Requirements not known yet. Please enter." << endl;
				}


			}



		}

		if (menu_selector == 2)
			cout << "Not implemented yet" << endl;

		if (menu_selector == 3)
		{
			cout << "Simulation study" << endl;
			cout << " - Set requirements" << endl;
			myRequirements.set_output_speed(25);
			myRequirements.set_runtime(1);
			myRequirements.set_no_strokes(300);
			myRequirements.set_stroke(25);
			myRequirements.set_transmission_length(1000);
			myRequirements.set_active_time(0.2);
			myRequirements.set_bending_angle(0);
			myRequirements.set_output_force(150);

			cout << " - Run study " << endl;

			/*ofstream pneumOffstream;
			pneumOffstream.open("results/study_pneum.dat");
			pneumOffstream << "F s mo mt ma M vo vt va V" << endl;
			ofstream hydraOffstream;
			hydraOffstream.open("results/study_hydra.dat");
			hydraOffstream << "F s mo mt ma M vo vt va V" << endl;
			ofstream tractOffstream;
			tractOffstream.open("results/study_tract.dat");
			tractOffstream << "F s mo mt ma M vo vt va V" << endl;
			*/

			ofstream studyResults;
			studyResults.open("results/study_comp.dat");
			studyResults << "F s mo mt ma M vo vt va V" << endl;

			ofstream studyResultsShort;
			studyResultsShort.open("results/study_comp_short.dat");
			studyResultsShort << "F s mo mt ma M vo vt va V" << endl;

			vector<ras_properties> tempProp;

			float forceStart = 90;
			float forceStop = 300;
			float forceStep = 0.5;

			float strokeStart = 10;
			float strokeStop = 150;
			float strokeStep = 0.5;

			int tempResult;

			cout << "0 Percent";

			auto tStart = Clock::now();
			auto tNow = Clock::now();

			for (float force = forceStart; force <= forceStop; force = force + forceStep)
			{

				myRequirements.set_output_force(force);

				for (float stroke = strokeStart; stroke <= strokeStop; stroke = stroke + strokeStep)
				{

					myRequirements.set_stroke(stroke);

					// Initialize requirements
					myPneumaticApproach.set_requirements(myRequirements);
					myHydraulicApproach.set_requirements(myRequirements);
					myTractionwireApproach.set_requirements(myRequirements);

					// Calculate
					myPneumaticApproach.calculate();
					myHydraulicApproach.calculate();
					myTractionwireApproach.calculate();

					// Get results back
					tempProp.clear();
					tempProp.push_back(myPneumaticApproach.get_properties()); // 0
					tempProp.push_back(myHydraulicApproach.get_properties()); // 1
					tempProp.push_back(myTractionwireApproach.get_properties()); // 2

					// Print force and stroke to file
					studyResults << force << " " << stroke;
					studyResultsShort << force << " " << stroke << " ";;

					// mass output
					tempResult = compare3(tempProp[0].get_mass_output(),
						tempProp[1].get_mass_output(),
						tempProp[2].get_mass_output());
					studyResults << " (" << tempResult << " " << tempProp[tempResult].get_mass_output() << ")";
					studyResultsShort << tempResult << " ";

					// mass transmission system
					tempResult = compare3(tempProp[0].get_mass_transmission_system(),
						tempProp[1].get_mass_transmission_system(),
						tempProp[2].get_mass_transmission_system());
					studyResults << " (" << tempResult << " " << tempProp[tempResult].get_mass_transmission_system() << ")";
					studyResultsShort << tempResult << " ";

					// mass actuation unit
					tempResult = compare3(tempProp[0].get_mass_actuation_unit(),
						tempProp[1].get_mass_actuation_unit(),
						tempProp[2].get_mass_actuation_unit());
					studyResults << " (" << tempResult << " " << tempProp[tempResult].get_mass_actuation_unit() << ")";
					studyResultsShort << tempResult << " ";

					// mass overall
					tempResult = compare3(tempProp[0].get_mass_overall(),
						tempProp[1].get_mass_overall(),
						tempProp[2].get_mass_overall());
					studyResults << " (" << tempResult << " " << tempProp[tempResult].get_mass_overall() << ")";
					studyResultsShort << tempResult << " ";

					// volume output
					tempResult = compare3(tempProp[0].get_volume_output(),
						tempProp[1].get_volume_output(),
						tempProp[2].get_volume_output());
					studyResults << " (" << tempResult << " " << tempProp[tempResult].get_volume_output() << ")";
					studyResultsShort << tempResult << " ";

					// volume transmission system
					tempResult = compare3(tempProp[0].get_volume_transmission_system(),
						tempProp[1].get_volume_transmission_system(),
						tempProp[2].get_volume_transmission_system());
					studyResults << " (" << tempResult << " " << tempProp[tempResult].get_volume_transmission_system() << ")";
					studyResultsShort << tempResult << " ";

					// volume actuation unit
					tempResult = compare3(tempProp[0].get_volume_actuation_unit(),
						tempProp[1].get_volume_actuation_unit(),
						tempProp[2].get_volume_actuation_unit());
					studyResults << " (" << tempResult << " " << tempProp[tempResult].get_volume_actuation_unit() << ")";
					studyResultsShort << tempResult << " ";

					// volume overall
					tempResult = compare3(tempProp[0].get_volume_overall(),
						tempProp[1].get_volume_overall(),
						tempProp[2].get_volume_overall());
					studyResults << " (" << tempResult << " " << tempProp[tempResult].get_volume_overall() << ")";
					studyResultsShort << tempResult << " ";

					float progress = (force - forceStart) / (forceStop - forceStart) * 100;
					tNow = Clock::now();
					float elapsedTime = chrono::duration_cast<chrono::seconds>(tNow - tStart).count();
					float remainingTime = elapsedTime/progress*(100-progress);

					cout.precision(0);
					cout.setf( std::ios::fixed, std:: ios::floatfield );
					cout << "\r" << progress << " Percent, "
						<< elapsedTime << " s elapsed, "
						<< elapsedTime/progress*(100-progress) << " s remaining.";

					studyResults << endl;
					studyResultsShort << endl;
				}


			}
			studyResults.close();
			studyResultsShort.close();
		}
	}
	return 0;
}
