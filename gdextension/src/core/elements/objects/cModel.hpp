#ifndef CMODEL_H
#define CMODEL_H

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
#include "models/container/container.hpp"
using namespace godot;

class CModel : public CSGCombiner3D{
	GDCLASS(CModel, CSGCombiner3D);
	public:
		void _ready () override;
		void generateStlFile(std::string path);
		void _exit_tree() override;
	protected:
		static void _bind_methods();
	private:
		ConfigurationMenu* m_configuration_menu;
		CSGCombiner3D* m_main;
		CustomContainer* m_child;
};

#endif

