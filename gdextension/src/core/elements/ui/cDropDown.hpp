#ifndef CDROPDOWN_H
#define CDROPDOWN_H

#include <godot_cpp/classes/option_button.hpp>

#include <godot_cpp/classes/input.hpp>
#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/classes/input_event.hpp>
#include <godot_cpp/variant/utility_functions.hpp>
#include <godot_cpp/classes/scene_tree.hpp>

#include <string>
#include <memory>
#include <iostream>

using namespace godot;

class CDropDown : public OptionButton
{
	GDCLASS(CDropDown, OptionButton);

	public:
		CDropDown();

		void item_selected(int arg);

		void _ready() override;
		
		void setTrigger(const std::string& trigger);
		
		void addItem(const std::string& item);
	protected:
		static void _bind_methods();
	private:
		std::vector<std::string> m_items;
		std::string m_trigger;
};
#endif
