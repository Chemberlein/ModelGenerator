#ifndef CLINEEDIT_H
#define CLINEEDIT_H

#include <godot_cpp/classes/line_edit.hpp>

#include <godot_cpp/classes/input.hpp>
#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/classes/input_event.hpp>
#include <godot_cpp/variant/utility_functions.hpp>
#include <godot_cpp/classes/scene_tree.hpp>

#include <string>
#include <memory>

using namespace godot;

class CLineEdit : public LineEdit
{
	GDCLASS(CLineEdit, LineEdit);

	public:
		CLineEdit();
		
		void textChanged(String s);

		void textSubmitted(String s);

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
