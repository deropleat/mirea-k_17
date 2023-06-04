#ifndef PRINTER_3D
#define PRINTER_3D

#include "cl_base.h"

class printer_3d : public cl_base
{
public:
	printer_3d(cl_base*, int, int, int);

	void tick_handler(std::string);
	void settings_handler(std::string);
	void command_handler(std::string&);
	void process_current_product();
	void coil_message_handler(std::string);
	void send_message_to_filament(std::string&);
	void send_message_to_info_screen(std::string&);
	void send_message_to_pc_callback(std::string&);
	void send_message_to_pc_get_product(std::string&);
	void message_from_pc_product_handler(std::string&);
private:
	bool is_configured = false;
	std::string current_product = "";
	int speed, temperature_speed, current_temperature, coils_count = 0;
};

#endif