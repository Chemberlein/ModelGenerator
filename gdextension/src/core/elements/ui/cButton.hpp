#ifndef CBUTTON_H
#define CBUTTON_H

#include <godot_cpp/classes/button.hpp>

#include <godot_cpp/classes/input.hpp>
#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/classes/input_event.hpp>
#include <godot_cpp/variant/utility_functions.hpp>
#include <godot_cpp/classes/scene_tree.hpp>

#include <string>
#include <memory>

using namespace godot;

class CButton : public Button
{
	GDCLASS(CButton, Button);

	public:
		CButton();

		void pressed();

		void _ready() override;
		
		void setTrigger(const std::string& trigger);

		void setText(const std::string& text);

	protected:
		static void _bind_methods();
	private:
		std::string m_trigger;
		String m_text;
};
#endif
