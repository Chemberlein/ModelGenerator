#include "customSection.hpp"
#include <iostream>

void CustomSection::_bind_methods()
{

}

void CustomSection::_ready()
{

	set_v_size_flags(Control::SizeFlags::SIZE_EXPAND_FILL);
	set_h_size_flags(Control::SizeFlags::SIZE_EXPAND_FILL);


	updateContent();
}

void CustomSection::setJsonConfiguration(const std::string& jsonConfigurationPath)
{
	// Open the file with FileAccess
    Ref<FileAccess> file = FileAccess::open(jsonConfigurationPath.c_str(), FileAccess::ModeFlags::READ);

    // Check if file opened successfully
    if (!file.is_valid()) {
        return;
    }

    // Read the entire content of the file as a string
    String json_string = file->get_as_text();
    m_json_data = JSON::parse_string(json_string);

	updateContent();
}

void CustomSection::updateContent()
{
	while (get_child_count())
	{
		auto ch = get_child(get_child_count()-1);
		remove_child(ch);
		ch->queue_free();
	}
	freeVectors();
	set_v_size_flags(Control::SizeFlags::SIZE_EXPAND_FILL);
	set_h_size_flags(Control::SizeFlags::SIZE_EXPAND_FILL);


    Dictionary json_dict = m_json_data;
	auto listOfSection = json_dict.keys();
	for (int i = 0; i < listOfSection.size(); i++)
	{
		String name = listOfSection[i];
		Dictionary value = json_dict[name];
		String type = value["type"];
		if (type == "group")
		{
			generateSection(value);
		}
	}
}

void CustomSection::generateSection(const Dictionary& section)
{
	
	Control* newSepL = memnew(Control);
	Control* newSepR = memnew(Control);
	newSepL->set_h_size_flags(Control::SizeFlags::SIZE_EXPAND_FILL);	
	newSepR->set_h_size_flags(Control::SizeFlags::SIZE_EXPAND_FILL);	
	HBoxContainer* hbx = memnew(HBoxContainer);
	hbx->set_h_size_flags(Control::SizeFlags::SIZE_EXPAND_FILL);
	add_child(hbx);
	
	Label* title = memnew(Label);
	title->set_text(section["title"]);
	hbx->add_child(newSepL);
	hbx->add_child(title);
	hbx->add_child(newSepR);
	m_h_box_container_vector.push_back(hbx);
	m_label_vector.push_back(title);
	m_control_vector.push_back(newSepL);
	m_control_vector.push_back(newSepR);
	Array listOfItems = section["items"];
	for (int i = 0; i<listOfItems.size(); i++)
	{
		Dictionary item = listOfItems[i];
		String type = item["type"];
		if (type == "button")
		{
			generateButton(item);
		}
		else if ( type == "text" )
		{
			generateText(item);
		}
		else if ( type == "labeledInputBox" )
		{
			generateLabeledInputBox(item);
		}
		else if ( type == "labeledSpinBox" )
		{
			generateLabeledSpinBox(item);
		}
		else if ( type == "dropDown" )
		{
			generateDropDown(item);
		}
		else if ( type == "labeledCheckBox" )
		{
			generateCheckBox(item);
		}
		else if ( type == "modelCard" )
		{
			generateModelCard(item);
		}
	}
}

void CustomSection::generateModelCard(const Dictionary& card)
{
	PanelContainer* container = memnew(PanelContainer);
	VBoxContainer* vbox = memnew(VBoxContainer);
	Label* lbl = memnew(Label);
	RichTextLabel* rtl = memnew(RichTextLabel);
	CButton* btn = memnew(CButton);

	String trigger = card["trigger"];
	String title = card["title"];
	String description = card["description"];
	lbl->set_text(title);
	rtl->set_text(description);
	rtl->set_fit_content(true);

	btn->setText("Edit");
	btn->setTrigger(std::string(trigger.utf8()));
	vbox->add_child(lbl);	
	vbox->add_child(rtl);	
	vbox->add_child(btn);
	vbox->set_h_size_flags(Control::SizeFlags::SIZE_EXPAND_FILL);	
	container->add_child(vbox);
	add_child(container);
	m_rtl_vector.push_back(rtl);
	m_label_vector.push_back(lbl);
	m_button_vector.push_back(btn);
	m_v_box_container_vector.push_back(vbox);
	m_panel_container_vector.push_back(container);
}


void CustomSection::generateDropDown(const Dictionary& dropDown)
{
	Control* newSep = memnew(Control);
	newSep->set_v_size_flags(Control::SizeFlags::SIZE_EXPAND_FILL);
	newSep->set_h_size_flags(Control::SizeFlags::SIZE_EXPAND_FILL);	
	HBoxContainer* hbx = memnew(HBoxContainer);
	Label*         lbl = memnew(Label);
	CDropDown*     drd = memnew(CDropDown);
	String text = dropDown["text"];
	String trigger = dropDown["trigger"];
	hbx->set_h_size_flags(Control::SizeFlags::SIZE_EXPAND_FILL);
	add_child(hbx);
	lbl->set_text(text);
	hbx->add_child(lbl);
	hbx->add_child(newSep);
	drd->setTrigger(std::string(trigger.utf8()));
	
	Array listOfItems = dropDown["list"];
	for (int i = 0; i < listOfItems.size(); i++)
	{
		String item = listOfItems[i];
		drd->addItem(std::string(item.utf8()));
	}
	
	hbx->add_child(drd);
	m_label_vector.push_back(lbl);
	m_drop_down_vector.push_back(drd);
	m_h_box_container_vector.push_back(hbx);
	m_control_vector.push_back(newSep);
}

void CustomSection::generateButton(const Dictionary& button)
{
	CButton* btn = memnew(CButton);
	String text = button["text"];
	String trigger = button["trigger"];
	btn->setText(std::string(text.utf8()));
	btn->setTrigger(std::string(trigger.utf8()));
	btn->set_h_size_flags(Control::SizeFlags::SIZE_EXPAND_FILL);
	add_child(btn);
	m_button_vector.push_back(btn);
}

void CustomSection::generateCheckBox(const Dictionary& checkBox)
{
	Control* newSep = memnew(Control);
	newSep->set_v_size_flags(Control::SizeFlags::SIZE_EXPAND_FILL);
	newSep->set_h_size_flags(Control::SizeFlags::SIZE_EXPAND_FILL);	

	HBoxContainer* hbx = memnew(HBoxContainer);
	Label*         lbl = memnew(Label);
	CCheckButton*     led = memnew(CCheckButton);
	String text = checkBox["text"];
	String trigger = checkBox["trigger"];
	hbx->set_h_size_flags(Control::SizeFlags::SIZE_EXPAND_FILL);
	add_child(hbx);
	lbl->set_text(text);
	hbx->add_child(lbl);
	hbx->add_child(newSep);
	led->setTrigger(std::string(trigger.utf8()));
	hbx->add_child(led);
	m_label_vector.push_back(lbl);
	m_check_box_vector.push_back(led);
	m_h_box_container_vector.push_back(hbx);
	m_control_vector.push_back(newSep);
}

void CustomSection::generateText(const Dictionary& text)
{
	RichTextLabel* rtl = memnew(RichTextLabel);
	String textt = text["text"];
	rtl->set_text(textt);
	rtl->set_fit_content(true);
	add_child(rtl);
	m_rtl_vector.push_back(rtl);
}

void CustomSection::generateLabeledInputBox(const Dictionary& labeledSpinBox)
{
	Control* newSep = memnew(Control);
	newSep->set_v_size_flags(Control::SizeFlags::SIZE_EXPAND_FILL);
	newSep->set_h_size_flags(Control::SizeFlags::SIZE_EXPAND_FILL);	

	HBoxContainer* hbx = memnew(HBoxContainer);
	Label*         lbl = memnew(Label);
	CLineEdit*     led = memnew(CLineEdit);
	String text    = labeledSpinBox["text"];
	String trigger = labeledSpinBox["trigger"];

	hbx->set_h_size_flags(Control::SizeFlags::SIZE_EXPAND_FILL);
	add_child(hbx);
	lbl->set_text(text);
	hbx->add_child(lbl);
	hbx->add_child(newSep);
	led->setParameterName(std::string(trigger.utf8()));
	if (labeledSpinBox.has("value"))
	{
		String value = labeledSpinBox["value"];
		led->setValue(std::string(value.utf8()));
	}
	hbx->add_child(led);
	m_label_vector.push_back(lbl);
	m_line_edit_vector.push_back(led);
	m_h_box_container_vector.push_back(hbx);
	m_control_vector.push_back(newSep);
}

void CustomSection::generateLabeledSpinBox(const Dictionary& labeledInputBox)
{
	Control* newSep = memnew(Control);
	newSep->set_v_size_flags(Control::SizeFlags::SIZE_EXPAND_FILL);
	newSep->set_h_size_flags(Control::SizeFlags::SIZE_EXPAND_FILL);	

	HBoxContainer* hbx = memnew(HBoxContainer);
	Label*         lbl = memnew(Label);
	CSpinBox*     led = memnew(CSpinBox);
	String text = labeledInputBox["text"];
	String trigger = labeledInputBox["trigger"];
	hbx->set_h_size_flags(Control::SizeFlags::SIZE_EXPAND_FILL);
	add_child(hbx);
	lbl->set_text(text);
	hbx->add_child(lbl);
	hbx->add_child(newSep);
	led->setParameterName(std::string(trigger.utf8()));
	hbx->add_child(led);
	m_label_vector.push_back(lbl);
	m_spin_box_vector.push_back(led);
	m_h_box_container_vector.push_back(hbx);
	m_control_vector.push_back(newSep);
}

void CustomSection::update_on_resize()
{
//	updateContent();
}

void CustomSection::_process(float delta) {	
	Viewport *viewport = get_viewport();
	Rect2 current_size = viewport->get_visible_rect();
	if (current_size != m_screen_size) {
		m_screen_size = current_size;
		update_on_resize();
	}
}

void CustomSection::_exit_tree() 
{
	freeVectors();
	queue_free();
	VBoxContainer::queue_free();
	VBoxContainer::_exit_tree();
}

void CustomSection::freeVectors()
{
	while(!m_control_vector.empty())
	{
		const auto ptr = m_control_vector.back();
		ptr->queue_free();
		memdelete(ptr);
		m_control_vector.pop_back();
	}
	while(!m_drop_down_vector.empty())
	{
		const auto ptr = m_drop_down_vector.back();
		ptr->queue_free();
		m_drop_down_vector.pop_back();
	}
	while(!m_check_box_vector.empty())
	{
		const auto ptr = m_check_box_vector.back();
		ptr->queue_free();
		m_check_box_vector.pop_back();
	}
	while(!m_button_vector.empty())
	{
		const auto ptr = m_button_vector.back();
		ptr->queue_free();
		m_button_vector.pop_back();
	}
	while(!m_label_vector.empty())
	{
		const auto ptr = m_label_vector.back();
		ptr->queue_free();
		m_label_vector.pop_back();
	}
	while(!m_rtl_vector.empty())
	{
		const auto ptr = m_rtl_vector.back();
		ptr->queue_free();
		m_rtl_vector.pop_back();
	}
	while(!m_line_edit_vector.empty())
	{
		const auto ptr = m_line_edit_vector.back();
		ptr->queue_free();
		m_line_edit_vector.pop_back();
	}
	while(!m_spin_box_vector.empty())
	{
		const auto ptr = m_spin_box_vector.back();
		ptr->queue_free();
		m_spin_box_vector.pop_back();
	}
	while(!m_h_box_container_vector.empty())
	{
		const auto ptr = m_h_box_container_vector.back();
		ptr->queue_free();
		m_h_box_container_vector.pop_back();
	}
	while(!m_v_box_container_vector.empty())
	{
		const auto ptr = m_v_box_container_vector.back();
		ptr->queue_free();
		m_v_box_container_vector.pop_back();
	}
	while(!m_panel_container_vector.empty())
	{
		const auto ptr = m_panel_container_vector.back();
		ptr->queue_free();
		m_panel_container_vector.pop_back();
	}

}
