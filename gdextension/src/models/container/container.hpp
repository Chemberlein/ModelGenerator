#ifndef CUSTOMCONTAINER_H
#define CUSTOMCONTAINER_H

#include <godot_cpp/classes/csg_combiner3d.hpp>
#include <godot_cpp/classes/csg_mesh3d.hpp>
#include <godot_cpp/classes/csg_box3d.hpp>
#include <godot_cpp/classes/standard_material3d.hpp>

#include <godot_cpp/classes/input.hpp>
#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/classes/input_event.hpp>
#include <godot_cpp/variant/utility_functions.hpp>
#include <godot_cpp/classes/scene_tree.hpp>
#include "./core/elements/ui/configurationMenu.hpp"
#include <godot_cpp/variant/variant.hpp>
#include <godot_cpp/variant/utility_functions.hpp>
#include <godot_cpp/classes/file_access.hpp>
#include <godot_cpp/classes/array_mesh.hpp>
#include <godot_cpp/classes/csg_sphere3d.hpp>
#include <godot_cpp/classes/csg_cylinder3d.hpp>
#include <godot_cpp/classes/canvas_layer.hpp>
#include <godot_cpp/core/memory.hpp> // for memnew
#include "../../core/ctrl/ctrlImpl.hpp"

using namespace godot;

class CustomContainerModel : public Node3D {
	GDCLASS(CustomContainerModel, Node3D);

	public:
		void _ready () override;
		void generateStlFile(std::string path);
		void _exit_tree() override;

		void updateModel();

		void setWidth(float width);
		void setHeight(float height);
		void setDepth(float depth);
		void setWallWidth(float wallWidth);
		void setHaveLid(bool haveLid);
		void setLidWallOverlap(float lidWallOverlap);
		void setOutsideCornersType(float type);
		void setOutsideCornerRadius(float radius);
		void setInnerCornersType(float type);
		void setInnerCornerRadius(float radius);
	
		float getWidth();
		float getHeight();
		float getDepth();
		float getWallWidth();
		bool getHaveLid();
		float getLidWallOverlap();
		float getOutsideCornersType();
		float getOutsideCornerRadius();
		float getInnerCornersType();
		float getInnerCornerRadius();
	protected:
		static void _bind_methods();
	private:
		CSGCombiner3D* m_base;
		CSGCombiner3D* m_lid;

		void createContainerInside(CSGCombiner3D* baseInside);
		void createContainerLidInside(CSGCombiner3D* lidInside);
		void createContainerOutside(CSGCombiner3D* baseInside);
		void createContainerLidOutside(CSGCombiner3D* lidOutside);

		void clearModel();

		StandardMaterial3D* material;

		float m_width = 30;
		float m_height = 30;
		float m_depth = 30;
		float m_wallWidth = 2;
	   	float m_lidWallOverlap = 2;
	   	float m_ocType = 2;
		float m_ocRadius = 5;
		float m_icType = 3;
		float m_icRadius = 3;
		bool m_haveLid = true;

};

class CustomContainerCtrl : public CtrlImplementation
{
	public:
		CustomContainerCtrl();
		void process(std::string key, std::string value) override;
		void registerKey(std::string key) override;
		void setModel(CustomContainerModel* model);
	protected:
		std::vector<std::string> m_keys;
		CustomContainerModel* m_model;
};

class CustomContainer : public CSGCombiner3D
{
	GDCLASS(CustomContainer, CSGCombiner3D);
	public:
		void _ready () override;
		void generateStlFile(std::string path);
		void _exit_tree() override;

		void updateModel();
	protected:
		static void _bind_methods();
	private:
		std::shared_ptr<CustomContainerCtrl> m_ctrl;
		CustomContainerModel* m_model;
		CanvasLayer* m_canvas_layer;
		ConfigurationMenu* m_configuration_menu;
};


#endif

