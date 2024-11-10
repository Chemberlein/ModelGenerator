#ifndef ABSTRACTNODE_H
#define ABSTRACTNODE_H
#include <string>
#include <vector>
#include <stdexcept>
#include <godot_cpp/classes/object.hpp>

class CtrlImplementation
{
	public:
		virtual
		void process(std::string key, std::string value);

		virtual
		void registerKey(std::string key);

	protected:
		static void _bind_methods(){};
		std::vector<std::string> m_keys;
};
#endif
