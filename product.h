#ifndef PRODUCT
#define PRODUCT

#include "cl_base.h"

class product : public cl_base
{
public:
	product(cl_base*, std::string, std::string, int);

	int get_volume();
	std::string get_filament();
	void reduce_volume(int);
private:
	int volume;
	std::string filament;
};

#endif