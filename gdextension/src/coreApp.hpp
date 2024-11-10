#ifndef COREAPP_H
#define COREAPP_H

#include <godot_cpp/classes/node3d.hpp>
#include <godot_cpp/classes/input.hpp>
#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/classes/input_event.hpp>
#include <godot_cpp/variant/utility_functions.hpp>
#include <godot_cpp/classes/scene_tree.hpp>
#include <godot_cpp/classes/canvas_layer.hpp>
#include "./core/elements/ui/sideMenu.hpp"
#include "./core/elements/objects/cModel.hpp"
#include "./core/elements/environment/skyBox.hpp"
#include "./core/elements/camera/modelOverviewCamera.hpp"
#include "./models/container/container.hpp"
#include "./models/laptopholder/laptopHolder.hpp"
#include "../../core/ctrl/ctrlImpl.hpp"

using namespace godot;

class CoreAppCtrl : public CtrlImplementation
{
	public:
		CoreAppCtrl();
		void process(std::string key, std::string value) override;
		void registerKey(std::string key) override;
		void setModel(Node3D* model);
	protected:
		std::vector<std::string> m_keys;
	private:
		void cleanModel();
		Node3D* m_model;
};

class CoreApp : public Node3D
{
	GDCLASS(CoreApp, Node3D);
	public:
		void _exit_tree() override;
		void _ready() override;

	protected:
		static void _bind_methods();
	private:
		CanvasLayer* m_ui;
		SideMenu* m_sideMenu;
		SkyBox*   m_skyBox;
		ModelOverviewCamera* m_camera;
		Node3D*   m_model;
		std::shared_ptr<CoreAppCtrl> m_mc;
};

#endif
