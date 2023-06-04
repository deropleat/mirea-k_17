#ifndef CONTROL_PANEL
#define CONTROL_PANEL

#include "cl_base.h"

class control_panel : public cl_base
{
public:
	control_panel(cl_base*);

	void input_command(std::string&);
	void input_setting(std::string&);
	void send_message_to_system(std::string&);
};

#endif