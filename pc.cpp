#include "pc.h"
#include "product.h"

pc::pc(cl_base* p_head_object) : cl_base(p_head_object, "PC") 
{
}

void pc::command_handler(std::string& command)
{
	if (command == "PC condition") 
	{
		std::string message;
		if (get_sub_objects().empty()) message = "PC condition turned off";
		else 
		{
			message += "PC condition turned on";
			for (int i = 0; i < get_sub_objects().size(); ++i) message += " " + get_sub_objects()[i]->get_name() + ";";
		}
		send_object_signal(SIGNAL_D(pc::send_message_to_info_screen), message);
		return;
	}

	int index = command.find(' ');
	int volume = std::stoi(command.substr(0, index));
	command = command.substr(index + 1);
	index = command.find(' ');
	std::string filament = command.substr(0, index);
	command = command.substr(index + 1);
	std::string name = command;
	if (get_sub_object_tree(name))
	{
		std::string message = "Failed to create product";
		send_object_signal(SIGNAL_D(pc::send_message_to_info_screen), message);
		return;
	}

	product* product_obj = new product(this, name, filament, volume);
	product_obj->set_state(1);
}

void pc::message_from_3d_printer_handler(std::string& command)
{
	command += std::to_string(get_sub_objects().size()) + "; PC:";
	
	for (int i = 0; i < get_sub_objects().size(); ++i) 
	{
		product* product_obj = (product*)get_sub_objects()[i];
		command += " " + product_obj->get_name() + ":" + std::to_string(product_obj->get_volume());
	}
	send_object_signal(SIGNAL_D(pc::send_message_to_info_screen), command);
}

void pc::message_from_3d_printer_product_handler(std::string& command)
{
	if (!get_sub_objects().empty())
	{
		product* nextProduct = (product*)get_sub_objects()[0];
		std::string message = nextProduct->get_filament() + " " + nextProduct->get_name();
		send_object_signal(SIGNAL_D(pc::send_message_to_printer_3d_product), message);
	}
}

void pc::send_message_to_info_screen(std::string&) 
{
}

void pc::send_message_to_printer_3d_product(std::string&) 
{
}