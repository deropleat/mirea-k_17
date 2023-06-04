#ifndef CL_APPLICATION
#define CL_APPLICATION

#include "cl_base.h"

class cl_application : public cl_base
{
public:
	cl_application(cl_base*);

	void build_tree_objects();

	void control_panel_message_handler(std::string);
	void send_message_to_screen(std::string&);
	void send_message_to_screen_print_tree(std::string&);
	void send_message_to_control_panel(std::string&);
	void send_message_to_printer(std::string&);
	void send_message_to_pc(std::string&);
	void send_tick_message(std::string&);

	int exec_app();
private:
	int ticks = 1;
};

#endif