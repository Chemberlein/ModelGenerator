#include "cLineEdit.hpp"
#include "../../ctrl/ctrl.hpp"
#include <iostream>
CLineEdit::CLineEdit()
{}

void CLineEdit::_bind_methods(){
	ClassDB::bind_method(D_METHOD("textChanged", "str"), &CLineEdit::textChanged);
	ClassDB::bind_method(D_METHOD("textSubmitted", "str"), &CLineEdit::textSubmitted);
}

void CLineEdit::textChanged(String s)
{
	m_currentValue = s;
	Ctrl::getInstance().send(m_parameterName, std::string(m_currentValue.utf8().get_data()));
}

void  CLineEdit::textSubmitted(String s)
{
	m_currentValue = s;
	Ctrl::getInstance().send(m_parameterName+"_submitted", std::string(m_currentValue.utf8().get_data()));
}

void CLineEdit::_ready()
{
	connect("text_changed", Callable(this, "textChanged"));
	connect("text_submitted", Callable(this, "textSubmitted"));
}

void CLineEdit::setParameterName(const std::string& name)
{
	m_parameterName = name;
}

void CLineEdit::setValue(const std::string& value)
{
	m_currentValue = value.c_str();
	set_text(m_currentValue);
}

void CLineEdit::_exit_tree()
{
	queue_free();
	LineEdit::_exit_tree();
}
