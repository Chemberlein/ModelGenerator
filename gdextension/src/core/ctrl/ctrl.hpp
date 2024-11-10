#ifndef CTRL_H
#define CTRL_H

#include <memory>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include "ctrlImpl.hpp"
class Ctrl
{
private:
	std::map<std::string, std::shared_ptr<CtrlImplementation>> m_abstractNodes;
	Ctrl(){};
public:
	Ctrl(Ctrl const&)           = delete;
	void operator=(Ctrl const&) = delete;
	static Ctrl& getInstance()
	{
		static Ctrl instance;
		return instance;
	}

	void registerBridge(std::string key, std::shared_ptr<CtrlImplementation> entry);
	
	void send(std::string key, std::string value = "");

	void setSavePath(std::string newSavePath);
	std::string getSavePath();

private:
	std::string m_savePath = "user://";
};

#endif
