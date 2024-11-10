
#include "cCheckButton.hpp"
#include "../../ctrl/ctrl.hpp"
CCheckButton::CCheckButton()
{}

void CCheckButton::_bind_methods(){
	ClassDB::bind_method(D_METHOD("button_pressed"), &CCheckButton::button_pressed);
}
void CCheckButton::button_pressed()
{
	Ctrl::getInstance().send(m_trigger);
}

void CCheckButton::_ready()
{
	connect("pressed", Callable(this, "button_pressed"));
}

void CCheckButton::setTrigger(const std::string& trigger)
{
	m_trigger = trigger;
}

