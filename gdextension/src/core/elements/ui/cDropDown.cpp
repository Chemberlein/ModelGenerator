#include "cDropDown.hpp"
#include "../../ctrl/ctrl.hpp"
CDropDown::CDropDown()
{}

void CDropDown::_bind_methods(){
	ClassDB::bind_method(D_METHOD("item_selected", "int"), &CDropDown::item_selected);
}

void CDropDown::item_selected(int arg)
{
	Ctrl::getInstance().send(m_trigger, m_items[arg]);
}

void CDropDown::addItem(const std::string& item)
{
	add_item(item.c_str(), m_items.size());
	m_items.push_back(item);
}

void CDropDown::_ready()
{
	connect("item_selected", Callable(this, "item_selected"));
}

void CDropDown::setTrigger(const std::string& trigger)
{
	m_trigger = trigger;
}


