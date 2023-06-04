#include "printer_3d.h"
#include "filament_coil.h"
#include "product.h"

printer_3d::printer_3d(cl_base* p_head_object, int i_speed, int i_temperature_speed, int i_current_temperature) : cl_base(p_head_object, "3D Printer")
{
	speed = i_speed;
	temperature_speed = i_temperature_speed;
	current_temperature = i_current_temperature;
}

void printer_3d::settings_handler(std::string command)
{
	if (is_configured) 
	{
		if (coils_count <= get_sub_objects().size()) 
		{
			set_state(1);
			return;
		}

		int index = command.find(' ');
		std::string filamentId = command.substr(0, index);
		command = command.substr(index + 1);
		index = command.find(' ');
		int filamentTemp = stoi(command.substr(0, index));
		command = command.substr(index + 1);
		index = command.find(' ');
		int filamentMaxVolume = stoi(command.substr(0, index));

		if (get_sub_object_tree(filamentId) != nullptr)
		{
			std::string message = "Failed to create filament coil";
			send_object_signal(SIGNAL_D(printer_3d::send_message_to_info_screen), message);
			return;
		}

		filament_coil* filamentCoil = new filament_coil(this, filamentId, filamentTemp, filamentMaxVolume);
		filamentCoil->set_state(1);

		set_connect(SIGNAL_D(printer_3d::send_message_to_filament), filamentCoil, HANDLER_D(filament_coil::printer_3d_signal_handler));
		return;
	}

	is_configured = true;
	int index = command.find(' ');
	coils_count = stoi(command.substr(0, index));
	command = command.substr(index + 1);
	index = command.find(' ');
	speed = stoi(command.substr(0, index));
	command = command.substr(index + 1);
	index = command.find(' ');
	temperature_speed = stoi(command.substr(0, index));
	command = command.substr(index + 1);
	index = command.find(' ');
	current_temperature = stoi(command.substr(0, index));
	command = command.substr(index + 1);
}

void printer_3d::command_handler(std::string& command)
{
	std::string toFind = "Filament coil condition ";
	int index = command.find(toFind);

	if (index != -1) 
	{
		std::string filament = command.replace(index, toFind.length(), "");
		send_object_signal(SIGNAL_D(printer_3d::send_message_to_filament), filament);
		return;
	}

	command += " temp: " + std::to_string(current_temperature) + "; status: " + std::to_string(get_state()) + "; print product: ";
	cl_base *current_product_obj = get_sub_object_branch(current_product);
	if (!current_product_obj) command += "0";
	else command += std::to_string(((product *) current_product_obj)->get_volume());

	command += "; queue products: ";
	send_object_signal(SIGNAL_D(printer_3d::send_message_to_pc_callback), command);
}

void printer_3d::tick_handler(std::string command)
{
	if (current_product.empty()) 
	{
		std::string message;
		send_object_signal(SIGNAL_D(printer_3d::send_message_to_pc_get_product), message);
		return;
	}
	process_current_product();
}

void printer_3d::message_from_pc_product_handler(std::string& command)
{
	int index = command.find(' ');
	std::string filament = command.substr(0, index);
	command = command.substr(index + 1);
	std::string product_name = command;
	
	filament_coil* filamentCoil = (filament_coil*)get_sub_object(filament);
	if (current_temperature > filamentCoil->get_temperature()) 
	{
		int delta = current_temperature - filamentCoil->get_temperature();
		if (delta > temperature_speed) delta %= temperature_speed;
		if (delta == 0) delta = temperature_speed;
		current_temperature -= delta;
		set_state(3);
	}
	else if (current_temperature < filamentCoil->get_temperature()) 
	{
		int delta = filamentCoil->get_temperature() - current_temperature;
		if (delta > temperature_speed) delta %= temperature_speed;
		if (delta == 0) delta = temperature_speed;
		current_temperature += delta;
		set_state(3);
	}
	else
	{
		product* product_obj = (product*)get_sub_object_tree(product_name);
		product_obj->set_head_object(this);
		current_product = product_name;
		set_state(2);
	}
}

void printer_3d::coil_message_handler(std::string message)
{
	send_object_signal(SIGNAL_D(printer_3d::send_message_to_info_screen), message);
}

void printer_3d::process_current_product()
{
	product* productObj = (product*)get_sub_object_branch(current_product);
	filament_coil* filamentCoil = (filament_coil*)get_sub_object_branch(productObj->get_filament());
	int process_volume = 0;

	if (productObj->get_volume() > speed) process_volume = speed;
	else process_volume = productObj->get_volume();

	int real_process_volume = filamentCoil->process(process_volume);
	if (real_process_volume != -1) productObj->reduce_volume(real_process_volume);
	else productObj->reduce_volume(process_volume);

	if (productObj->get_volume() <= 0) 
	{
		delete_by_name(current_product);
		current_product = "";
		set_state(1);
	}
	else 
	{
		if (real_process_volume != process_volume) 
		{
			set_state(4);
			filamentCoil->refill();
		}
		else set_state(2);
	}

}

void printer_3d::send_message_to_filament(std::string& message) 
{
}

void printer_3d::send_message_to_info_screen(std::string& message) 
{
}

void printer_3d::send_message_to_pc_callback(std::string& message) 
{
}

void printer_3d::send_message_to_pc_get_product(std::string& message) 
{
}