#include "cl_application.h"
#include "printer_3d.h"
#include "control_panel.h"
#include "pc.h"
#include "information_screen.h"
#include "queue"

cl_application::cl_application(cl_base* p_head_object) : cl_base(p_head_object) 
{
}

void cl_application::build_tree_objects()
{
	set_state(1);

	printer_3d* p_printer_3d = new printer_3d(this, 0, 0, 0);
	control_panel* p_control_panel = new control_panel(this);
	pc* p_pc = new pc(this);
	information_screen* p_information_screen = new information_screen(this);

	p_printer_3d->set_state(5);
	p_control_panel->set_state(1);
	p_pc->set_state(1);
	p_information_screen->set_state(1);

	p_printer_3d->set_connect(SIGNAL_D(printer_3d::send_message_to_info_screen), p_information_screen, HANDLER_D(information_screen::message_to_output_handler));
	p_pc->set_connect(SIGNAL_D(pc::send_message_to_info_screen), p_information_screen, HANDLER_D(information_screen::message_to_output_handler));
	p_control_panel->set_connect(SIGNAL_D(control_panel::input_setting), p_printer_3d, HANDLER_D(printer_3d::settings_handler));
	
	std::string cmd;
	while (p_printer_3d->get_state() != 1) p_control_panel->send_object_signal(SIGNAL_D(control_panel::input_setting), cmd);
	p_control_panel->delete_object_connection(SIGNAL_D(control_panel::input_setting), p_printer_3d, HANDLER_D(printer_3d::settings_handler));

	set_connect(SIGNAL_D(cl_application::send_message_to_printer), p_printer_3d, HANDLER_D(printer_3d::command_handler));
	set_connect(SIGNAL_D(cl_application::send_message_to_screen), p_information_screen, HANDLER_D(information_screen::message_to_output_handler));
	set_connect(SIGNAL_D(cl_application::send_message_to_screen_print_tree), p_information_screen, HANDLER_D(information_screen::message_to_output_tree_handler));
	set_connect(SIGNAL_D(cl_application::send_message_to_control_panel), p_control_panel, HANDLER_D(control_panel::input_command));
	set_connect(SIGNAL_D(cl_application::send_message_to_pc), p_pc, HANDLER_D(pc::command_handler));
	set_connect(SIGNAL_D(cl_application::send_tick_message), p_printer_3d, HANDLER_D(printer_3d::tick_handler));
	
	p_printer_3d->set_connect(SIGNAL_D(printer_3d::send_message_to_pc_callback), p_pc, HANDLER_D(pc::message_from_3d_printer_handler));
	p_printer_3d->set_connect(SIGNAL_D(printer_3d::send_message_to_pc_get_product), p_pc, HANDLER_D(pc::message_from_3d_printer_product_handler));
	p_control_panel->set_connect(SIGNAL_D(control_panel::send_message_to_system), this, HANDLER_D(cl_application::control_panel_message_handler));
	p_pc->set_connect(SIGNAL_D(pc::send_message_to_printer_3d_product), p_printer_3d, HANDLER_D(printer_3d::message_from_pc_product_handler));

	std::string message = "Ready to work";
	send_object_signal(SIGNAL_D(cl_application::send_message_to_screen), message);
}

int cl_application::exec_app()
{
	std::string cmd;
	while (get_state() != 0) 
	{
		send_object_signal(SIGNAL_D(cl_application::send_message_to_control_panel), cmd);
		send_object_signal(SIGNAL_D(cl_application::send_tick_message), cmd);
		ticks++;
	}

	return 0;
}

void cl_application::control_panel_message_handler(std::string s_text)
{
	if (s_text == "Turn off the system")
	{
		send_object_signal(SIGNAL_D(cl_application::send_message_to_screen), s_text);
		set_state(0);
	}
	else if (s_text.find("Filament coil condition") != -1) send_object_signal(SIGNAL_D(cl_application::send_message_to_printer), s_text);
	else if (s_text == "PC condition") send_object_signal(SIGNAL_D(cl_application::send_message_to_pc), s_text);
	else if (s_text.find("PC") == 0) 
	{
		std::string message = s_text.replace(0, 3, "");
		send_object_signal(SIGNAL_D(cl_application::send_message_to_pc), message);
	}
	else if (s_text == "System status") 
	{
		std::string message = "3D printer tact: " + std::to_string(ticks) + ";";
		send_object_signal(SIGNAL_D(cl_application::send_message_to_printer), message);
	}
	else if (s_text == "SHOWTREE") 
	{
		std::string message;
		send_object_signal(SIGNAL_D(cl_application::send_message_to_screen_print_tree), message);
		exit(0);
	}
}


void cl_application::send_message_to_control_panel(std::string& s_text)
{
}

void cl_application::send_message_to_printer(std::string& s_text)
{
}

void cl_application::send_message_to_pc(std::string& s_text)
{
}

void cl_application::send_tick_message(std::string& s_text)
{
}

void cl_application::send_message_to_screen_print_tree(std::string& p_signal)
{
}

void cl_application::send_message_to_screen(std::string& p_signal)
{
}