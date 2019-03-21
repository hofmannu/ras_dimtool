ODIR = obj
_OBJS = force_gauge.o force_gauge_selector.o hydraulic_housing_bk.o linear_motor.o linear_motor_selector.o hydraulic_tube.o hydraulic_cylinder.o tractionwire_housing_bk.o input_winch.o stepper_motor.o stepper_motor_selector.o hydraulic_approach.o battery.o bearing.o bearing_selector.o file_handling.o output_winch.o pneumatic_approach.o pneumatic_tube.o pneumatic_tube_selector.o pneumatic_valve.o rack.o rack_selector.o ras_properties.o requirements.o tractionwire_approach.o tractionwire.o
OBJS = $(patsubst %,$(ODIR)/%,$(_OBJS))
CC = g++ -std=c++11
CFLAGS = -Wall -std=c++11 -rpath -rpath-link
LLAGS = -Wall -Wextra -pedantic -O2 -std=c++11 -rpath -rpath-link
LIBS =

all: main

obj/battery.o :
	$(CC) $(LFLAGS) -c src/battery.cpp -o obj/battery.o

obj/bearing.o :
	$(CC) $(LFLAGS) -c src/bearing.cpp -o obj/bearing.o

obj/bearing_selector.o : obj/file_handling.o obj/bearing.o
	$(CC) $(LFLAGS) -c src/bearing_selector.cpp -o obj/bearing_selector.o

obj/file_handling.o :
	$(CC) $(LFLAGS) -c src/file_handling.cpp -o obj/file_handling.o

obj/output_winch.o : obj/bearing.o obj/bearing_selector.o obj/rack_selector.o
	$(CC) $(LFLAGS) -c src/output_winch.cpp -o obj/output_winch.o

obj/input_winch.o : obj/bearing.o obj/bearing_selector.o
	$(CC) $(LFLAGS) -c src/input_winch.cpp -o obj/input_winch.o

obj/pneumatic_approach.o : obj/pneumatic_tube.o obj/pneumatic_tube_selector.o obj/pneumatic_valve.o obj/file_handling.o
	$(CC) $(LFLAGS) -c src/pneumatic_approach.cpp -o obj/pneumatic_approach.o

obj/pneumatic_tube.o :
	$(CC) $(LFLAGS) -c src/pneumatic_tube.cpp -o obj/pneumatic_tube.o

obj/pneumatic_tube_selector.o : obj/file_handling.o obj/pneumatic_tube.o
	$(CC) $(LFLAGS) -c src/pneumatic_tube_selector.cpp -o obj/pneumatic_tube_selector.o

obj/pneumatic_valve.o :
	$(CC) $(LFLAGS) -c src/pneumatic_valve.cpp -o obj/pneumatic_valve.o

obj/rack.o :
	$(CC) $(LFLAGS) -c src/rack.cpp -o obj/rack.o

obj/rack_selector.o : obj/rack.o
	$(CC) $(LFLAGS) -c src/rack_selector.cpp -o obj/rack_selector.o

obj/stepper_motor.o :
	$(CC) $(LFLAGS) -c src/stepper_motor.cpp -o obj/stepper_motor.o

obj/stepper_motor_selector.o : obj/stepper_motor.o
	$(CC) $(LFLAGS) -c src/stepper_motor_selector.cpp -o obj/stepper_motor_selector.o

obj/ras_properties.o :
	$(CC) $(LFLAGS) -c src/ras_properties.cpp -o obj/ras_properties.o

obj/requirements.o :
	$(CC) $(LFLAGS) -c src/requirements.cpp -o obj/requirements.o

obj/tractionwire_approach.o : obj/battery.o obj/tractionwire_housing_bk.o obj/tractionwire.o obj/output_winch.o obj/stepper_motor.o obj/stepper_motor_selector.o obj/input_winch.o
	$(CC) $(LFLAGS) -c src/tractionwire_approach.cpp -o obj/tractionwire_approach.o

obj/tractionwire.o :
	$(CC) $(LFLAGS) -c src/tractionwire.cpp -o obj/tractionwire.o

obj/tractionwire_housing_bk.o :
	$(CC) $(LFLAGS) -c src/tractionwire_housing_bk.cpp -o obj/tractionwire_housing_bk.o

obj/hydraulic_approach.o : obj/hydraulic_housing_bk.o obj/force_gauge.o obj/force_gauge_selector.o obj/linear_motor.o obj/linear_motor_selector.o obj/requirements.o obj/hydraulic_cylinder.o obj/hydraulic_tube.o
	$(CC) $(LFLAGS) -c src/hydraulic_approach.cpp -o obj/hydraulic_approach.o

obj/hydraulic_cylinder.o :
	$(CC) $(LFLAGS) -c src/hydraulic_cylinder.cpp -o obj/hydraulic_cylinder.o

obj/hydraulic_tube.o :
	$(CC) $(LFLAGS) -c src/hydraulic_tube.cpp -o obj/hydraulic_tube.o

obj/linear_motor.o :
	$(CC) $(LFLAGS) -c src/linear_motor.cpp -o obj/linear_motor.o

obj/linear_motor_selector.o : obj/linear_motor.o
	$(CC) $(LFLAGS) -c src/linear_motor_selector.cpp -o obj/linear_motor_selector.o

obj/hydraulic_housing_bk.o :
	$(CC) $(LFLAGS) -c src/hydraulic_housing_bk.cpp -o obj/hydraulic_housing_bk.o

obj/force_gauge.o :
	$(CC) $(LFLAGS) -c src/force_gauge.cpp -o obj/force_gauge.o

obj/force_gauge_selector.o : obj/force_gauge.o
	$(CC) $(LFLAGS) -c src/force_gauge_selector.cpp -o obj/force_gauge_selector.o

main : $(OBJS)
	$(CC) $(LFLAGS) $(OBJS) src/main.cpp $(LIBS) -o bin/main

.PHONY: obj/tractionwire_housing_bk.o obj/tractionwire_approach.o obj/hydraulic_approach.o obj/output_winch.o obj/ras_properties.o obj/pneumatic_approach.o
