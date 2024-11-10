#include "coreApp.hpp"
#include "../../ctrl/ctrl.hpp"


#include <godot_cpp/variant/variant.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

CoreAppCtrl::CoreAppCtrl()
{}

void CoreAppCtrl::setModel(Node3D* model)
{
	m_model = model;
	CustomContainer* cc = memnew(CustomContainer);
	m_model->add_child(cc);
	registerKey("containerSelected");
	registerKey("laptopholderSelected");
}

void CoreAppCtrl::registerKey(std::string key)
{
	Ctrl::getInstance().registerBridge(key, std::make_shared<CoreAppCtrl>(*this));
	m_keys.push_back(key);
}

void CoreAppCtrl::cleanModel()
{
	while (m_model->get_child_count())
	{
		auto ch = m_model->get_child(m_model->get_child_count()-1);
		m_model->remove_child(ch);
	}
}	
void CoreAppCtrl::process(std::string key, std::string value)
{
	if (key == "containerSelected")
	{
		cleanModel();
		CustomContainer* cc = memnew(CustomContainer);
		m_model->add_child(cc);
	}
	else if (key == "laptopholderSelected")
	{
		cleanModel();
		CustomLaptopHolder* lh = memnew(CustomLaptopHolder);
		m_model->add_child(lh);
	}
}

void CoreApp::_ready ()
{
	while (get_child_count())
	{
		auto ch = get_child(get_child_count()-1);
		remove_child(ch);
	}
	
	m_skyBox = memnew(SkyBox);
	add_child(m_skyBox);
	m_skyBox->set_owner(get_tree()->get_edited_scene_root());

	m_ui = memnew(CanvasLayer);
	add_child(m_ui);
	m_ui->set_owner(get_tree()->get_edited_scene_root());

	m_sideMenu = memnew(SideMenu);
	m_ui->add_child(m_sideMenu);
	m_sideMenu->set_owner(get_tree()->get_edited_scene_root());

	m_camera = memnew(ModelOverviewCamera);
	add_child(m_camera);
	m_camera->set_owner(get_tree()->get_edited_scene_root());

	m_model = memnew(Node3D);
	add_child(m_model);
	m_model->set_owner(get_tree()->get_edited_scene_root());
	m_mc = std::make_shared<CoreAppCtrl>();
	m_mc->setModel(m_model);
}

void CoreApp::_bind_methods() { 
}

void CoreApp::_exit_tree() 
{
	if (m_model)
	{
		m_model->queue_free();
	}
	if (m_skyBox) {
		m_skyBox->queue_free();
	}
	if (m_sideMenu) {
		m_sideMenu->queue_free();
	}
	if (m_ui) {
		m_ui->queue_free();
	}
	queue_free();
	Node3D::queue_free();
}
