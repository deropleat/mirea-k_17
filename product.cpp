#include "product.h"

product::product(cl_base* p_head_object, std::string s_name, std::string s_filament, int i_volume) : cl_base(p_head_object, s_name)
{
	filament = s_filament;
	volume = i_volume;
}

int product::get_volume()
{
	return volume;
}

std::string product::get_filament()
{
	return filament;
}

void product::reduce_volume(int value)
{
	volume -= value;
}