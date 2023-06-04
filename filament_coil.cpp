#include "filament_coil.h"
#include "printer_3d.h"

filament_coil::filament_coil(cl_base* p_head_object, std::string s_id, int i_temp, int i_max_volume) : cl_base(p_head_object, s_id)
{
	max_volume = i_max_volume;
	current_volume = i_max_volume;
	temp = i_temp;

	set_connect(SIGNAL_D(filament_coil::send_signal_to_printer), p_head_object, HANDLER_D(printer_3d::coil_message_handler));
}

void filament_coil::printer_3d_signal_handler(std::string filament)
{
	if (filament == get_name()) 
	{
		std::string answer = "Filament coil condition: " + filament + " " + std::to_string(current_volume);
		send_object_signal(SIGNAL_D(filament_coil::send_signal_to_printer), answer);
	}
}

int filament_coil::process(int processVolume)
{
	if (processVolume > current_volume) 
	{
		int processedVolume = current_volume;
		current_volume = 0;
		return processedVolume;
	}
	else current_volume -= processVolume;

	if (current_volume == 0) return -1;
	return processVolume;
}

int filament_coil::get_temperature()
{
	return temp;
}

void filament_coil::refill()
{
	if (refilling_ticks == 0) 
	{
		refilling_ticks = 2;
		return;
	}

	if (refilling_ticks == 1) current_volume = max_volume;
	refilling_ticks--;
}

void filament_coil::send_signal_to_printer(std::string& signal) 
{
}