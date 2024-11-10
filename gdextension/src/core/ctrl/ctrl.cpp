#include "ctrl.hpp"
#include <iostream>
void Ctrl::registerBridge(std::string key, std::shared_ptr<CtrlImplementation> entry)
{
	m_abstractNodes[key] = entry;
	std::cout<<"Key registerd "<<key<<std::endl;
}

void Ctrl::send(std::string key, std::string value)
{
	if (m_abstractNodes.find(key) != m_abstractNodes.end())
	{
		m_abstractNodes[key]->process(key, value);
	}
	else
	{
		std::cout<<"Parameter " << key << " not registered.";
	}
}

void Ctrl::setSavePath(std::string newSavePath)
{
	std::cout<<"New path set "<<newSavePath<<std::endl;
	m_savePath = newSavePath;
}

std::string Ctrl::getSavePath()
{
	return m_savePath;
}
