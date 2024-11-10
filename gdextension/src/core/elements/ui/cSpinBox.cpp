#include "cSpinBox.hpp"
#include "../../ctrl/ctrl.hpp"
CSpinBox::CSpinBox()
{}

void CSpinBox::_bind_methods(){
	ClassDB::bind_method(D_METHOD("valueChanged", "f"), &CSpinBox::valueChanged);
}

void  CSpinBox::valueChanged(float v)
{
	m_currentValue = std::to_string(v).c_str();
	Ctrl::getInstance().send(m_parameterName, std::to_string(v));
}

void CSpinBox::_ready()
{
	connect("value_changed", Callable(this, "valueChanged"));
}

void CSpinBox::setParameterName(const std::string& name)
{
	m_parameterName = name;
}

void CSpinBox::setValue(const std::string& value)
{
	m_currentValue = value.c_str();
}

void CSpinBox::_exit_tree()
{
	queue_free();
	SpinBox::_exit_tree();
}
