#ifndef BASESPINBOX_H
#define BASESPINBOX_H

#include <godot_cpp/classes/spin_box.hpp>

#include <godot_cpp/classes/input.hpp>
#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/classes/input_event.hpp>
#include <godot_cpp/variant/utility_functions.hpp>
#include <godot_cpp/classes/scene_tree.hpp>

#include <string>
#include <memory>

using namespace godot;

class CSpinBox : public SpinBox
{
	GDCLASS(CSpinBox, SpinBox);

	public:
		CSpinBox();
		
		void valueChanged(float v);

		void _ready() override;
		
		void setParameterName(const std::string& name);

		void setValue(const std::string& value);
	
		void _exit_tree() override;

	protected:
		static void _bind_methods();
	private:
		std::string m_parameterName;
		String m_currentValue;
};
#endif
