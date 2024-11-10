#include "configurationMenu.hpp"
#include "../../ctrl/ctrl.hpp"




ConfigurationMenuCtrl::ConfigurationMenuCtrl(CustomSection* customSection)
	: m_custom_section(customSection)
{
}

void ConfigurationMenuCtrl::process(std::string key, std::string value)
{
	 m_custom_section->setJsonConfiguration(m_button_to_json[key]);
}

void ConfigurationMenuCtrl::registerKey(std::string key)
{
	Ctrl::getInstance().registerBridge(key, std::make_shared<ConfigurationMenuCtrl>(*this));
	m_keys.push_back(key);
}

void ConfigurationMenu::setJsonConfiguration(std::string newPath)
{
	m_default = newPath;
	m_custom_section->setJsonConfiguration(m_default);
}
void ConfigurationMenu::_bind_methods()
{

}

void ConfigurationMenu::_ready()
{
	while (get_child_count())
	{
		//Remove everything except for timer
		auto ch = get_child(get_child_count()-1);
		remove_child(ch);
	}

	m_side_menu = memnew(PanelContainer);
	add_child(m_side_menu);
	m_scroll_container = memnew(ScrollContainer);
	m_scroll_container->set_v_size_flags(Control::SizeFlags::SIZE_EXPAND_FILL);
	m_scroll_container->set_h_size_flags(Control::SizeFlags::SIZE_EXPAND_FILL);
	m_side_menu->add_child(m_scroll_container);
	m_side_menu_column = memnew(VBoxContainer);
	m_side_menu_column->set_v_size_flags(Control::SizeFlags::SIZE_EXPAND_FILL);
	m_side_menu_column->set_h_size_flags(Control::SizeFlags::SIZE_EXPAND_FILL);
	m_scroll_container->add_child(m_side_menu_column);
	m_custom_section = memnew(CustomSection);
	m_custom_section->set_v_size_flags(Control::SizeFlags::SIZE_EXPAND_FILL);
	m_custom_section->set_h_size_flags(Control::SizeFlags::SIZE_EXPAND_FILL);
	m_custom_section->setJsonConfiguration(m_default);
	m_side_menu_column->add_child(m_custom_section);
	m_mc = std::make_shared<ConfigurationMenuCtrl>(m_custom_section);

	update_on_resize();
}

void ConfigurationMenu::_process(float delta) {	
	Viewport *viewport = get_viewport();
	Rect2 current_size = viewport->get_visible_rect();
	if (current_size != m_screen_size) {
		m_resize_in_process = true;
		m_screen_size = current_size;
	}else if (m_resize_in_process)
	{
		m_resize_in_process = false;
		update_on_resize();
	}
}

void ConfigurationMenu::update_on_resize()
{
	m_side_menu->set_size(Vector2(m_side_menu_width, m_screen_size.size.y));
	m_side_menu->set_position(Vector2(m_screen_size.size.x - m_side_menu_width, 0)); 
}

void ConfigurationMenu::_exit_tree() 
{
	if (m_custom_section)
	{
		m_custom_section->queue_free();
	}
	if (m_scroll_container)
	{
		m_scroll_container->queue_free();
	}
	if (m_side_menu_column)
	{
		m_side_menu_column->queue_free();
	}
	if (m_side_menu)
	{
		m_side_menu->queue_free();
	}
	queue_free();
	Control::_exit_tree();
}


