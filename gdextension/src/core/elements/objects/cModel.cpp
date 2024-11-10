#include "cModel.hpp"

#include <godot_cpp/variant/variant.hpp>
#include <godot_cpp/variant/utility_functions.hpp>
#include <godot_cpp/classes/file_access.hpp>
#include <godot_cpp/classes/array_mesh.hpp>
#include <godot_cpp/classes/csg_sphere3d.hpp>
#include <godot_cpp/classes/csg_cylinder3d.hpp>
#include <godot_cpp/core/memory.hpp> // for memnew

void CModel::_ready (){
	while (get_child_count())
	{
		auto ch = get_child(get_child_count()-1);
		remove_child(ch);
	}
	m_main = memnew(CSGCombiner3D);
	m_child = memnew(CustomContainer);
	add_child(m_child);
	add_child(m_main);

	m_configuration_menu = memnew(ConfigurationMenu);
	add_child(m_configuration_menu);
	m_configuration_menu->setJsonConfiguration("res://gdextension/src/models/container/ConfigurationOptions.json");
}

void CModel::_bind_methods() { 
}

void CModel::generateStlFile(std::string path="")
{
	m_child->generateStlFile(path);
}

void CModel::_exit_tree() 
{
	if (m_main) {
		m_main->queue_free();
	}
	if (m_child)
	{
		m_child->queue_free();	
	}
	if (m_configuration_menu) {
		m_configuration_menu->queue_free();
	}
	queue_free();
	Node3D::_exit_tree();
}
