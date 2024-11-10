#ifndef SIDEMENU_H
#define SIDEMENU_H
#include <godot_cpp/classes/control.hpp>
#include <godot_cpp/classes/button.hpp>
#include <godot_cpp/classes/label.hpp>
#include <godot_cpp/classes/timer.hpp>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/variant/utility_functions.hpp>
#include <godot_cpp/classes/panel_container.hpp>
#include <godot_cpp/classes/viewport.hpp>
#include <godot_cpp/classes/texture_button.hpp>
#include <godot_cpp/classes/texture_rect.hpp>
#include <godot_cpp/classes/image_texture.hpp>
#include <godot_cpp/classes/resource_loader.hpp>
#include <godot_cpp/classes/v_box_container.hpp>
#include <godot_cpp/classes/h_box_container.hpp>
#include <godot_cpp/classes/scene_tree.hpp>

#include "customSection.hpp"
#include "cButton.hpp"
#include "../../ctrl/ctrlImpl.hpp"

#include <string>
#include <map>
#include <memory>

using namespace godot;

class SideMenuCtrl : public CtrlImplementation
{
	public:
		SideMenuCtrl(CustomSection* customSection);
		void process(std::string key, std::string value) override;
		void registerKey(std::string key) override;
	protected:
		std::vector<std::string> m_keys;
		CustomSection* m_custom_section;
		std::map<std::string, std::string> m_button_to_json;
};

class SideMenu : public Control {
	GDCLASS(SideMenu, Control);
public:
	void _ready();
	void _process(float delta);
	void toggle_menu();
	void animate_slide();
	void _exit_tree() override;
protected:
	static void _bind_methods();
private:
	std::shared_ptr<SideMenuCtrl> m_mc;
	bool m_resize_in_process=false;
	void update_on_resize();
	Rect2 m_screen_size;

	PanelContainer* m_side_menu = nullptr;
	Button*  m_toggle_button;
	Timer* m_slide_timer = nullptr;
	Ref<Texture2D> m_toggle_button_closed_texture;
	Ref<Texture2D> m_toggle_button_open_texture;
	Vector2 m_side_panel_hidden_position;
	Vector2 m_side_panel_visible_position;
	Vector2 m_side_panel_current_position;
	Vector2 m_toggle_button_hidden_position;
	Vector2 m_toggle_button_visible_position;
	Vector2 m_toggle_button_current_position;

	VBoxContainer* m_side_menu_column;
	HBoxContainer* m_top_button_row;
	HBoxContainer* m_bottom_button_row;
	TextureRect*   m_logo;
	Ref<Texture2D> m_logo_texture;
	CButton*        m_home_button;
	CButton*        m_models_button;
	CButton*        m_settings_button;
	CButton*        m_shortcuts_button;
	CButton*        m_about_button;
	CustomSection* m_custom_section;
	bool m_menu_visible = false;
	float m_slide_speed = 1000.0f;

	float m_side_menu_width = 500;
	float m_toggle_button_margin = 5;

	void updateSideMenu();
	void updateToggleButtonTexture();
	void updateSideMenuContent();
};
#endif
