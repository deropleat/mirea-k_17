#ifndef CL_BASE
#define CL_BASE

#include <vector>
#include <iostream>
#include <string>

class cl_base
{
public:
	cl_base(cl_base*, std::string = "Base_object");
	~cl_base();

	cl_base* get_head_object();
	cl_base* get_sub_object(std::string);
	cl_base* get_sub_object_tree(std::string);
	cl_base* get_sub_object_branch(std::string);
	std::vector<cl_base*> get_sub_objects();

	std::string get_name();
	int get_state();
	void set_state(int);
	bool set_head_object(cl_base*);

	void delete_by_name(std::string);
	void print_branch_ready(std::string = "");

	typedef void (cl_base::* TYPE_SIGNAL)(std::string&);
	typedef void (cl_base::* TYPE_HANDLER)(std::string);

	struct o_sh
	{
		TYPE_SIGNAL p_signal;
		cl_base* p_cl_base;
		TYPE_HANDLER p_handler;
	};

	void set_connect(TYPE_SIGNAL, cl_base*, TYPE_HANDLER);
	void send_object_signal(TYPE_SIGNAL, std::string&);
	void delete_object_connection(TYPE_SIGNAL, cl_base*, TYPE_HANDLER);

	std::vector<o_sh*> connects;
private:
	int get_type_of_head(cl_base*);
	bool subordinate_object_extends(cl_base* obj);
	
	int state = 0;
	std::string name;
	cl_base* p_head_object;
	std::vector<cl_base*> subordinate_objects;
};

#define SIGNAL_D(signal_f) (TYPE_SIGNAL)(&signal_f)
#define HANDLER_D(handler_f) (TYPE_HANDLER)(&handler_f)

#endif