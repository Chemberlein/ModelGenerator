#ifndef CONFIGURATIONMENU_H
#define CONFIGURATIONMENU_H
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
#include <godot_cpp/classes/scroll_container.hpp>
#include <godot_cpp/classes/scene_tree.hpp>

#include "customSection.hpp"
#include "cButton.hpp"
#include "../../ctrl/ctrlImpl.hpp"

#include <string>
#include <map>
#include <memory>

using namespace godot;

class ConfigurationMenuCtrl : public CtrlImplementation
{
	public:
		ConfigurationMenuCtrl(CustomSection* customSection);
		void process(std::string key, std::string value) override;
		void registerKey(std::string key) override;
	protected:
		std::vector<std::string> m_keys;
		CustomSection* m_custom_section;
		std::map<std::string, std::string> m_button_to_json;
};

class ConfigurationMenu : public Control {
	GDCLASS(ConfigurationMenu, Control);
public:
	void _ready();
	void _process(float delta);
	void _exit_tree() override;
	void setJsonConfiguration(std::string newPath);
protected:
	static void _bind_methods();
private:
	bool m_resize_in_process=false;
	void update_on_resize();
	Rect2 m_screen_size;
	std::string m_default = "res://gdextension/models/demo.json";

	std::shared_ptr<ConfigurationMenuCtrl> m_mc;

	PanelContainer* m_side_menu = nullptr;

	VBoxContainer* m_side_menu_column;
	ScrollContainer* m_scroll_container;
	CustomSection* m_custom_section;
	float m_side_menu_width = 500;
};
#endif
