#ifndef CUSTOMLAPTOPHOLDER_H
#define CUSTOMLAPTOPHOLDER_H

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

class CustomLaptopHolderModel : public Node3D {
	GDCLASS(CustomLaptopHolderModel, Node3D);

	public:
		void _ready () override;
		void generateStlFile(std::string path);

		void updateModel();

		void setSideWidth(float width);
		void setHeight(float height);
		void setLength(float length);
		void setLaptopWidth(float laptopWidth);
		void setCornerRadius(float radius);
		void setSpaceFromTheTable(float space);

	protected:
		static void _bind_methods();
	private:
		CSGCombiner3D* m_laptopHolder;

		void createLaptopHolderBody(CSGCombiner3D* baseInside);

		void clearModel();

		StandardMaterial3D* material;

		float m_width = 20;
		float m_height = 30;
		float m_length = 150;
		float m_laptopWidth = 20;
		float m_cornerRadius = 5;
		float m_spaceFromTable = 5;
};

class CustomLaptopHolderCtrl : public CtrlImplementation
{
	public:
		CustomLaptopHolderCtrl();
		void process(std::string key, std::string value) override;
		void registerKey(std::string key) override;
		void setModel(CustomLaptopHolderModel* model);
	protected:
		std::vector<std::string> m_keys;
		CustomLaptopHolderModel* m_model;
};

class CustomLaptopHolder : public CSGCombiner3D
{
	GDCLASS(CustomLaptopHolder, CSGCombiner3D);
	public:
		void _ready () override;
		void generateStlFile(std::string path);

		void updateModel();
	protected:
		static void _bind_methods();
	private:
		std::shared_ptr<CustomLaptopHolderCtrl> m_ctrl;
		CustomLaptopHolderModel* m_model;
		CanvasLayer* m_canvas_layer;
		ConfigurationMenu* m_configuration_menu;
};


#endif

