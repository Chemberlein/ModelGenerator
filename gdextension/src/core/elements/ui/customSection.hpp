#ifndef CUSTOMSECTION_H
#define CUSTOMSECTION_H
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
#include <godot_cpp/classes/image.hpp>
#include <godot_cpp/classes/resource_loader.hpp>
#include <godot_cpp/classes/v_box_container.hpp>
#include <godot_cpp/classes/h_box_container.hpp>
#include <godot_cpp/classes/rich_text_label.hpp>
#include "godot_cpp/classes/file_access.hpp"
#include "godot_cpp/classes/json.hpp"
#include <godot_cpp/classes/scene_tree.hpp>
#include "cButton.hpp"
#include "cLineEdit.hpp"
#include "cSpinBox.hpp"
#include "cDropDown.hpp"
#include "cCheckButton.hpp"
#include <string>

class CustomSection : public VBoxContainer {
	GDCLASS(CustomSection, Control);
public:
	
	void _exit_tree() override;
	void _ready();
	void _process(float delta);
	void setJsonConfiguration(const std::string& jsonConfigurationPath);
protected:
	static void _bind_methods();
private:
	void freeVectors();
	void generateSection(const Dictionary& section);
	void generateButton(const Dictionary& button);
	void generateText(const Dictionary& text);
	void generateLabeledSpinBox(const Dictionary& labeledSpinBox);
	void generateLabeledInputBox(const Dictionary& labeledInputBox);
	void generateDropDown(const Dictionary& dropDown);
	void generateCheckBox(const Dictionary& checkBox);
	void generateModelCard(const Dictionary& card);

	void update_on_resize();
	
	Rect2 m_screen_size;
	void updateContent();
	Variant m_json_data;

	std::vector<CButton*> m_button_vector;
	std::vector<Label*>  m_label_vector;
	std::vector<RichTextLabel*>  m_rtl_vector;
	std::vector<CLineEdit*>  m_line_edit_vector;
	std::vector<CSpinBox*>  m_spin_box_vector;
	std::vector<CDropDown*>  m_drop_down_vector;
	std::vector<HBoxContainer*>  m_h_box_container_vector;
	std::vector<VBoxContainer*>  m_v_box_container_vector;
	std::vector<CCheckButton*>  m_check_box_vector;
	std::vector<Control*>  m_control_vector;
	std::vector<PanelContainer*>  m_panel_container_vector;
};
#endif
