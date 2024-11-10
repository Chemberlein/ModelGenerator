#include "cButton.hpp"
#include "../../ctrl/ctrl.hpp"
CButton::CButton()
{}

void CButton::_bind_methods(){
	ClassDB::bind_method(D_METHOD("pressed"), &CButton::pressed);
}
void CButton::pressed()
{
	Ctrl::getInstance().send(m_trigger);
}

void CButton::_ready()
{
	connect("pressed", Callable(this, "pressed"));
}

void CButton::setTrigger(const std::string& trigger)
{
	m_trigger = trigger;
}

void CButton::setText(const std::string& text)
{
	m_text = text.c_str();
	set_text(m_text);
}
