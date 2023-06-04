#ifndef PC
#define PC

#include "cl_base.h"

class pc : public cl_base
{
public:
	pc(cl_base*);

	void command_handler(std::string&);
	void send_message_to_info_screen(std::string&);
	void send_message_to_printer_3d_product(std::string&);
	void message_from_3d_printer_product_handler(std::string&);
	void message_from_3d_printer_handler(std::string&);
};

#endif