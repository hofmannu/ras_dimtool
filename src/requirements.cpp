#include "requirements.h"

using namespace std;

void requirements::read_in_user_input()
{
	cout << " Requirements:" << endl;
	cout << " - Required output force [N] \t\t";
	cin >> output_force;
	cout << " - Required output speed [mm/s] \t";
	cin >> output_speed;
	cout << " - Required runtime [h] \t\t";
	cin >> runtime;
	cout << " - Required number of load cycles [1]\t";
	cin >> no_strokes;
	cout << " - Required stroke [mm]\t\t\t";
	cin >> stroke;
	cout << " - Transmission length [mm]\t\t";
	cin >> transmission_length;
	cout << " - Active time [0...1]\t\t\t";
	cin >> active_time;
	cout << " - Bending angle [deg]\t\t\t";
	cin >> bending_angle;
	cout << endl;

	// Set status to successfully defined for requirements
	status = 1;
}

float requirements::get_t_holding()
{
	tMoving = stroke / output_speed * no_strokes * 2; // [mm] / [mm/s] = [s]
	tHolding = active_time * runtime * 60 * 60 - tMoving;
	return tHolding; // [s]
}

float requirements::get_t_moving()
{
	tMoving = stroke / output_speed * no_strokes * 2; // [mm] / [mm/s] = [s]
	return tMoving; // [s]
}

void requirements::set_output_force(const float& _output_force)
{
	output_force = _output_force;
}

void requirements::set_output_speed(const float& _output_speed)
{
	output_speed = _output_speed;
}

void requirements::set_runtime(const float& _runtime)
{
	runtime = _runtime;
}

void requirements::set_no_strokes(const float& _no_strokes)
{
	no_strokes = _no_strokes;
}

void requirements::set_stroke(const float& _stroke)
{
	stroke = _stroke;
}

void requirements::set_active_time(const float& _active_time)
{
	active_time = _active_time;
}

void requirements::set_bending_angle(const float& _bending_angle)
{
	bending_angle = _bending_angle;
}

void requirements::set_transmission_length(const float& _transmission_length)
{
	transmission_length = _transmission_length;
}
