#include <iostream>

#ifndef REQUIREMENTS_H
#define REQUIREMENTS_H

class requirements
{
private:
	float output_force; // [N]
	float output_speed; // [mm/s]
	float runtime; // [h]
	float no_strokes; // [1]
	float stroke; // [mm]
	float transmission_length; // [mm]
	float active_time; // [1] how much time is the system actively holding against a force
	float bending_angle; // [deg]

	float tMoving; // [s]
	float tHolding; // [s]

	bool status = 1;


public:
	void read_in_user_input();
	// Will return the status of the requirements. 1 means entered, 0 means not.
	bool get_status() const {return status;};

	// Get functions
	float get_output_force() const {return output_force;}; // [N]
	float get_transmission_length() const {return transmission_length;};
	float get_output_speed() const {return output_speed;}; // [mm/s]
	float get_runtime() const {return runtime;}; // [h]
	float get_runtime_sec() const {return runtime*60*60;}; // [s]
	float get_no_strokes() const {return no_strokes;};
	float get_stroke() const {return stroke;};
	float get_active_time() const {return active_time;};
	float get_bending_angle() const {return bending_angle;};

	// Calculated values
	float get_t_moving(); // [s]
	float get_t_holding(); // [s]

	// Set functions
	void set_output_force(const float& _output_force);
	void set_output_speed(const float& _output_speed);
	void set_runtime(const float& _runtime);
	void set_no_strokes(const float& _no_strokes);
	void set_stroke(const float& _stroke);
	void set_transmission_length(const float& _transmission_length);
	void set_active_time(const float& _active_time);
	void set_bending_angle(const float& _bending_angle);

};

#endif
