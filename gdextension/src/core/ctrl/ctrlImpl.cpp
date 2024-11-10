#include "ctrlImpl.hpp"
#include <iostream>
void CtrlImplementation::process(std::string key, std::string value)
{
	std::cout<<"process method should be overrided"<<std::endl;
}

void CtrlImplementation::registerKey(std::string key)
{
	std::cout<<"register method should be overrided"<<std::endl;
}
