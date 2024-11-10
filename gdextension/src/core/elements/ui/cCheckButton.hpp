#ifndef CCHECKBUTTON_H 
#define CCHECKBUTTON_H

#include <godot_cpp/classes/check_button.hpp>

#include <godot_cpp/classes/input.hpp>
#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/classes/input_event.hpp>
#include <godot_cpp/variant/utility_functions.hpp>
#include <godot_cpp/classes/scene_tree.hpp>

#include <string>
#include <memory>

using namespace godot;

class CCheckButton : public CheckButton
{
	GDCLASS(CCheckButton, CheckButton);

	public:
		CCheckButton();

		void button_pressed();

		void _ready() override;
		
		void setTrigger(const std::string& trigger);

	protected:
		static void _bind_methods();
	private:
		std::string m_trigger;
};
#endif
