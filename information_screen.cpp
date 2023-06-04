#include "information_screen.h"

information_screen::information_screen(cl_base* p_head_object) : cl_base(p_head_object, "Information Screen") 
{
}

void information_screen::message_to_output_handler(std::string message)
{
	std::cout << message << std::endl;
}

void information_screen::message_to_output_tree_handler(std::string message)
{
	std::cout << "Object tree";
	get_head_object()->print_branch_ready();
}