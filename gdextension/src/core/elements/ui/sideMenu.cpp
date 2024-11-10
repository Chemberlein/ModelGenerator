#include "sideMenu.hpp"
#include "../../ctrl/ctrl.hpp"




SideMenuCtrl::SideMenuCtrl(CustomSection* customSection)
	: m_custom_section(customSection)
{
	m_button_to_json = {
		{"home", "res://configuration/menus/home.json"},
		{"models", "res://configuration/menus/models.json"},
//		{"settings", "res://configuration/menus/settings.json"},
		{"shortcuts", "res://configuration/menus/shortcuts.json"},
		{"about", "res://configuration/menus/about.json"}
};	
	m_custom_section->setJsonConfiguration(m_button_to_json["home"]);
}

void SideMenuCtrl::process(std::string key, std::string value)
{	
	if (key == "outputDirectoryPath")
	{
		Ctrl::getInstance().setSavePath(value);
	}
	else
	{
		m_custom_section->setJsonConfiguration(m_button_to_json[key]);
	}
}

void SideMenuCtrl::registerKey(std::string key)
{
	Ctrl::getInstance().registerBridge(key, std::make_shared<SideMenuCtrl>(*this));
	m_keys.push_back(key);
}

void SideMenu::_bind_methods()
{

}

void SideMenu::_ready()
{
	m_side_menu = memnew(PanelContainer);
	m_toggle_button = memnew(Button);
	m_toggle_button_closed_texture = ResourceLoader::get_singleton()->load("res://assets/icons/menuClosed.png");
	m_toggle_button_open_texture = ResourceLoader::get_singleton()->load("res://assets/icons/menuOpen.png");
	m_toggle_button->connect("pressed", callable_mp(this, &SideMenu::toggle_menu));
	
	m_side_panel_hidden_position = Vector2(-m_side_menu_width-5, 0);
	m_side_panel_visible_position = Vector2(0, 0);

	m_toggle_button_hidden_position = Vector2(m_toggle_button_margin, m_toggle_button_margin);
	m_toggle_button_visible_position = Vector2(m_side_menu_width + m_toggle_button_margin, m_toggle_button_margin);

	m_slide_timer = memnew(Timer);
	m_slide_timer->set_wait_time(0.016);
	m_slide_timer->set_one_shot(false);
	m_slide_timer->connect("timeout", callable_mp(this, &SideMenu::animate_slide));
	add_child(m_slide_timer);
	m_slide_timer->set_owner(get_tree()->get_edited_scene_root());
	m_side_menu_column = memnew(VBoxContainer);

	m_logo = memnew(TextureRect);
	m_logo_texture = ResourceLoader::get_singleton()->load("res://assets/icons/logoSmall.png");
	m_logo->set_texture(m_logo_texture);

	m_custom_section = memnew(CustomSection);
	m_custom_section->set_v_size_flags(Control::SizeFlags::SIZE_EXPAND_FILL);
	m_custom_section->set_h_size_flags(Control::SizeFlags::SIZE_EXPAND_FILL);
	
	m_mc = std::make_shared<SideMenuCtrl>(m_custom_section);

	m_top_button_row   = memnew(HBoxContainer);
	m_bottom_button_row = memnew(HBoxContainer);
	m_home_button = memnew(CButton);
	m_home_button->setText("Home");
	m_home_button->setTrigger("home");
	m_mc->registerKey("home");
	m_models_button = memnew(CButton);
	m_models_button->setText("Models");
	m_models_button->setTrigger("models");
	m_mc->registerKey("models");
//	m_settings_button = memnew(CButton);
//	m_settings_button->setText("Settings");
//	m_settings_button->setTrigger("settings");
//	m_mc->registerKey("settings");
	m_shortcuts_button = memnew(CButton);
	m_shortcuts_button->setText("Controls");
	m_shortcuts_button->setTrigger("shortcuts");
	m_mc->registerKey("shortcuts");
	m_about_button = memnew(CButton);
	m_about_button->setText("About");
	m_about_button->setTrigger("about");
	m_mc->registerKey("about");
m_mc->registerKey("outputDirectoryPath");	
	updateSideMenu();
}

void SideMenu::updateSideMenu()
{
	while (get_child_count()>1)
	{
		//Remove everything except for timer
		auto ch = get_child(get_child_count()-1);
		remove_child(ch);
	}

	m_side_menu->set_size(Vector2(m_side_menu_width, m_screen_size.size.y));
	m_side_menu->set_position(Vector2(-m_side_menu_width, 0)); 

	
	add_child(m_side_menu);
	m_side_menu->set_owner(get_tree()->get_edited_scene_root());
	
	updateSideMenuContent();
	updateToggleButtonTexture();

	m_toggle_button->set_position(Vector2(m_toggle_button_margin, m_toggle_button_margin));
	add_child(m_toggle_button);
	m_toggle_button->set_owner(get_tree()->get_edited_scene_root());
	
	Vector2 target_position = !m_menu_visible ? m_side_panel_hidden_position : m_side_panel_visible_position;
	Vector2 toggle_button_target_position = !m_menu_visible ? m_toggle_button_hidden_position : m_toggle_button_visible_position;
	m_side_menu->set_position(target_position);
	m_toggle_button->set_position(toggle_button_target_position);

}

void SideMenu::updateSideMenuContent()
{
	while (m_side_menu->get_child_count())
	{
		auto ch = m_side_menu->get_child(m_side_menu->get_child_count()-1);
		m_side_menu->remove_child(ch);
	}

	m_side_menu_column->set_v_size_flags(Control::SizeFlags::SIZE_EXPAND_FILL);
	m_side_menu_column->set_h_size_flags(Control::SizeFlags::SIZE_EXPAND_FILL);
	m_side_menu->add_child(m_side_menu_column);
	m_side_menu_column->set_owner(get_tree()->get_edited_scene_root());

	while (m_side_menu_column->get_child_count())
	{
		auto ch = m_side_menu_column->get_child(m_side_menu_column->get_child_count()-1);
		m_side_menu_column->remove_child(ch);
	}

  	m_side_menu_column->add_child(m_logo);
	m_logo->set_owner(get_tree()->get_edited_scene_root());

	while (m_top_button_row->get_child_count())
	{
		auto ch = m_top_button_row->get_child(m_top_button_row->get_child_count()-1);
		m_top_button_row->remove_child(ch);
	}

	m_top_button_row->set_alignment(BoxContainer::AlignmentMode::ALIGNMENT_CENTER);
	m_top_button_row->set_h_size_flags(Control::SizeFlags::SIZE_EXPAND_FILL);
	m_side_menu_column->add_child(m_top_button_row);
	m_top_button_row->set_owner(get_tree()->get_edited_scene_root());
	
	m_top_button_row->add_child(m_home_button);
	m_home_button->set_owner(get_tree()->get_edited_scene_root());

	m_top_button_row->add_child(m_models_button);
	m_models_button->set_owner(get_tree()->get_edited_scene_root());

	//m_top_button_row->add_child(m_settings_button);
	//m_settings_button->set_owner(get_tree()->get_edited_scene_root());


	m_side_menu_column->add_child(m_custom_section);
	m_custom_section->set_owner(get_tree()->get_edited_scene_root());

	while (m_bottom_button_row->get_child_count())
	{
		auto ch = m_bottom_button_row->get_child(m_bottom_button_row->get_child_count()-1);
		m_bottom_button_row->remove_child(ch);
	}

	m_bottom_button_row->set_alignment(BoxContainer::AlignmentMode::ALIGNMENT_END);
	m_bottom_button_row->set_h_size_flags(Control::SizeFlags::SIZE_EXPAND_FILL);
	m_side_menu_column->add_child(m_bottom_button_row);
	m_bottom_button_row->set_owner(get_tree()->get_edited_scene_root());
	
	m_bottom_button_row->add_child(m_shortcuts_button);
	m_shortcuts_button->set_owner(get_tree()->get_edited_scene_root());
	m_bottom_button_row->add_child(m_about_button);
	m_about_button->set_owner(get_tree()->get_edited_scene_root());
}

void SideMenu::updateToggleButtonTexture()
{
	if (m_menu_visible)
	{
		m_toggle_button->set_text("Close");
		m_toggle_button->set_button_icon(m_toggle_button_open_texture);
	}
	else
	{
		m_toggle_button->set_text("Menu");
		m_toggle_button->set_button_icon(m_toggle_button_closed_texture);
	}
}

void SideMenu::update_on_resize()
{
	m_side_panel_hidden_position = Vector2(-m_side_menu_width-10, 0);
	m_side_panel_visible_position = Vector2(0, 0);

	m_toggle_button_hidden_position = Vector2(m_toggle_button_margin, m_toggle_button_margin);
	m_toggle_button_visible_position = Vector2(m_side_menu_width + m_toggle_button_margin + 10, m_toggle_button_margin);
	
	m_side_menu->set_size(Vector2(m_side_menu_width, m_screen_size.size.y));
	m_side_menu->set_position(Vector2(m_menu_visible?m_side_panel_visible_position:m_side_panel_hidden_position)); 
	m_toggle_button->set_position(Vector2(m_menu_visible?m_toggle_button_visible_position:m_toggle_button_hidden_position)); 
}

void SideMenu::_process(float delta) {	
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

void SideMenu::toggle_menu()
{
	if (m_slide_timer->is_stopped())
	{
		m_slide_timer->start();
	}
}

void SideMenu::animate_slide()
{
	Vector2 target_position = m_menu_visible ? m_side_panel_hidden_position : m_side_panel_visible_position;
	m_side_panel_current_position = m_side_menu->get_position();

	Vector2 toggle_button_target_position = m_menu_visible ? m_toggle_button_hidden_position : m_toggle_button_visible_position;
	m_toggle_button_current_position = m_toggle_button->get_position();

	float distance_to_move = m_slide_speed * m_slide_timer->get_wait_time();
	Vector2 direction = (target_position - m_side_panel_current_position).normalized();
	Vector2 new_position = m_side_panel_current_position + direction * distance_to_move;
	Vector2 new_toggle_button_new_position = m_toggle_button_current_position + direction * distance_to_move;

	if (m_side_panel_current_position.distance_to(target_position) <= distance_to_move)
	{
		m_side_menu->set_position(target_position);
		m_toggle_button->set_position(toggle_button_target_position);
		m_slide_timer->stop();
		m_menu_visible = !m_menu_visible;
		updateToggleButtonTexture();
	}
	else
	{
		m_side_menu->set_position(new_position);
		m_toggle_button->set_position(new_toggle_button_new_position);
	}
}

void SideMenu::_exit_tree() 
{
	if (m_custom_section) {
		m_custom_section->queue_free();
	}
	if (m_about_button) {
		m_about_button->queue_free();
	}
	if (m_shortcuts_button) {
		m_shortcuts_button->queue_free();
	}
	//if (m_settings_button) {
	//	m_settings_button->queue_free();
//	}
	if (m_models_button) {
		m_models_button->queue_free();
	}
	if (m_home_button) {
		m_home_button->queue_free();
	}
	if (m_logo) {
		m_logo->queue_free();
	}
	if (m_bottom_button_row) {
		m_bottom_button_row->queue_free();
	}
	if (m_top_button_row) {
		m_top_button_row->queue_free();
	}
	if (m_side_menu_column) {
		m_side_menu_column->queue_free();
	}
	if (m_toggle_button) {
		m_toggle_button->queue_free();
	}
	if (m_slide_timer) {
		m_slide_timer->queue_free();
	}
	if (m_side_menu) {
		m_side_menu->queue_free();
	}
	m_toggle_button_open_texture.unref();
	m_toggle_button_closed_texture.unref();
	m_logo_texture.unref();

	queue_free();
	Control::_exit_tree();
}
