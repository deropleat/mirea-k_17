#ifndef FILAMENT_COIL
#define FILAMENT_COIL

#include "cl_base.h"

class filament_coil : public cl_base
{
public:
	filament_coil(cl_base*, std::string, int, int);

	void refill();
	int get_temperature();
	int process(int);

	void send_signal_to_printer(std::string&);
	void printer_3d_signal_handler(std::string);
private:
	int temp, max_volume, current_volume, refilling_ticks = 0;
};

#endif