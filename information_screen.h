#ifndef INFORMATION_SCREEN
#define INFORMATION_SCREEN

#include "cl_base.h"

class information_screen : public cl_base
{
public:
	information_screen(cl_base*);

	void message_to_output_handler(std::string);
	void message_to_output_tree_handler(std::string);
};

#endif