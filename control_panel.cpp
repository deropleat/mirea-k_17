#include "control_panel.h"

control_panel::control_panel(cl_base* p_head_object) : cl_base(p_head_object, "Control Panel") {}

void control_panel::input_setting(std::string& command)
{
	std::getline(std::cin, command);
}

void control_panel::input_command(std::string& command)
{
	std::getline(std::cin, command);
	send_object_signal(SIGNAL_D(control_panel::send_message_to_system), command);
}

void control_panel::send_message_to_system(std::string& command) 
{
}