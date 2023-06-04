#include "cl_base.h"
#include <queue>

cl_base::cl_base(cl_base* pp_head_object, std::string s_name)
{
	p_head_object = pp_head_object;
	name = s_name;

	if (pp_head_object != nullptr) p_head_object->subordinate_objects.push_back(this);
}

cl_base::~cl_base()
{
	for (int i = 0; i < subordinate_objects.size(); i++) delete subordinate_objects[i];
	for (int i = 0; i < connects.size(); i++) delete connects[i];
}

cl_base* cl_base::get_sub_object(std::string s_name)
{
	for (int i = 0; i < subordinate_objects.size(); i++) if (subordinate_objects[i]->get_name() == s_name) return subordinate_objects[i];
	return nullptr;
}

cl_base* cl_base::get_sub_object_branch(std::string s_name)
{
	cl_base* found = nullptr;
	std::queue<cl_base*> q;
	q.push(this);

	while (!q.empty()) 
	{
		cl_base* elem = q.front();
		q.pop();
		if (elem->name == s_name)
		{
			if (found) return nullptr;
			else found = elem;
		}
		for (int i = 0; i < elem->subordinate_objects.size(); i++) q.push(elem->subordinate_objects[i]);
	}

	return found;
}


cl_base* cl_base::get_sub_object_tree(std::string s_name)
{
	if (!p_head_object) return get_sub_object_branch(s_name);
	return p_head_object->get_sub_object_tree(s_name);
}

void cl_base::print_branch_ready(std::string tabs)
{
	std::cout << std::endl << tabs << name;
	if (state != 0) std::cout << " is ready";
	else std::cout << " is not ready";

	for (int i = 0; i < subordinate_objects.size(); i++) subordinate_objects[i]->print_branch_ready(tabs + "    ");
}

void cl_base::set_state(int new_state)
{
	if (p_head_object == nullptr || p_head_object->state != 0) state = new_state;
	else state = 0;

	if (state == 0) for (int i = 0; i < subordinate_objects.size(); i++) subordinate_objects[i]->set_state(new_state);
}

std::string cl_base::get_name()
{
	return name;
}

cl_base* cl_base::get_head_object()
{
	return p_head_object;
}

std::vector<cl_base*> cl_base::get_sub_objects()
{
	return subordinate_objects;
}

int cl_base::get_type_of_head(cl_base* new_p_head_object)
{
	if (!p_head_object) return 1;
	if (!new_p_head_object) return 2;
	if (new_p_head_object->get_sub_object(name)) return 3;
	if (subordinate_object_extends(new_p_head_object)) return 4;
	return 0;
}

bool cl_base::set_head_object(cl_base* new_p_head_object)
{
	if (get_type_of_head(new_p_head_object) != 0) return false;

	for (int i = 0; i < p_head_object->subordinate_objects.size(); ++i) if (p_head_object->subordinate_objects[i]->name == name)
	{
		p_head_object->subordinate_objects.erase(p_head_object->subordinate_objects.begin() + i);
		break;
	}

	p_head_object = new_p_head_object;
	new_p_head_object->subordinate_objects.push_back(this);
	return true;
}

bool cl_base::subordinate_object_extends(cl_base* object)
{
	if (object == this) return true;
	if (!p_head_object) return false;
	return p_head_object->subordinate_object_extends(object);
}

void cl_base::delete_by_name(std::string s_name)
{
	cl_base* del = get_sub_object_branch(s_name);
	if (!del) return;

	cl_base* del_head = del->p_head_object;
	for (int i = 0; i < del_head->subordinate_objects.size(); ++i) 
	{
		if (del_head->subordinate_objects[i]->name == s_name)
		{
			delete del_head->subordinate_objects[i];
			del_head->subordinate_objects.erase(del_head->subordinate_objects.begin() + i);
			return;
		}
	}
}

void cl_base::set_connect(TYPE_SIGNAL p_signal, cl_base* p_object, TYPE_HANDLER p_handler)
{
	o_sh* p_value;

	for (unsigned int i = 0; i < connects.size(); i++) if (connects[i]->p_signal == p_signal && connects[i]->p_cl_base == p_object && connects[i]->p_handler == p_handler) return;

	p_value = new o_sh();
	p_value->p_signal = p_signal;
	p_value->p_cl_base = p_object;
	p_value->p_handler = p_handler;
	connects.push_back(p_value);
}

void cl_base::send_object_signal(TYPE_SIGNAL p_signal, std::string& s_command)
{
	if (state == 0) return;

	TYPE_HANDLER p_handler;
	cl_base* p_object;

	(this->*p_signal)(s_command);

	for (unsigned int i = 0; i < connects.size(); i++) 
	{
		if (connects[i]->p_signal == p_signal) 
		{
			p_handler = connects[i]->p_handler;
			p_object = connects[i]->p_cl_base;
			if (p_object->state != 0) (p_object->*p_handler)(s_command);
		}
	}
}

void cl_base::delete_object_connection(TYPE_SIGNAL p_signal, cl_base* p_target_object, TYPE_HANDLER p_handler)
{
	for (int i = 0; i < connects.size(); i++) if ((connects.at(i)->p_signal == p_signal && connects.at(i)->p_cl_base == p_target_object && connects.at(i)->p_handler == p_handler)) connects.erase(connects.begin() + i);
}

int cl_base::get_state()
{
	return state;
}