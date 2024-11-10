#include "laptopHolder.hpp"
#include "../../ctrl/ctrl.hpp"

#include <godot_cpp/classes/array_mesh.hpp>
#include <godot_cpp/classes/csg_sphere3d.hpp>
#include <godot_cpp/classes/csg_cylinder3d.hpp>
#include <godot_cpp/classes/canvas_layer.hpp>
#include <godot_cpp/classes/csg_combiner3d.hpp>
#include <godot_cpp/classes/surface_tool.hpp>
#include <godot_cpp/classes/array_mesh.hpp>
#include <godot_cpp/classes/mesh_instance3d.hpp>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/classes/dir_access.hpp>
#include <unordered_map>
#include <string>
#include <fstream>
CustomLaptopHolderCtrl::CustomLaptopHolderCtrl()
{}

void CustomLaptopHolderCtrl::setModel(CustomLaptopHolderModel* model)
{
	m_model = model;
	registerKey("exportAsStl");
	registerKey("laptopHolderSideWidth");
	registerKey("laptopHolderHeight");
	registerKey("laptopHolderLength");
	registerKey("laptopHolderLaptopWidth");
	registerKey("laptopHolderCornerRadius");
}

void CustomLaptopHolderCtrl::registerKey(std::string key)
{
	Ctrl::getInstance().registerBridge(key, std::make_shared<CustomLaptopHolderCtrl>(*this));
	m_keys.push_back(key);
}

void CustomLaptopHolderCtrl::process(std::string key, std::string value)
{
	std::cout<<key << " " << value<<std::endl;
	if (key == "exportAsStl")
	{
		m_model->generateStlFile(Ctrl::getInstance().getSavePath());
	}
	float fvalue = std::strtof(value.c_str(),NULL);

	if (key == "laptopHolderSideWidth" && !value.empty())
	{
		m_model->setSideWidth(fvalue);
	}
	else if (key == "laptopHolderHeight" && !value.empty())
	{
		m_model->setHeight(fvalue);
	}
	else if (key == "laptopHolderLength" && !value.empty())
	{
		m_model->setLength(fvalue);
	}
	else if (key == "laptopHolderLaptopWidth" && !value.empty())
	{
		m_model->setLaptopWidth(fvalue);
	}
	else if (key == "laptopHolderCornerRadius" && !value.empty())
	{
		m_model->setCornerRadius(fvalue);
	}
	else if (key == "laptopHolderSpaceFromTable" && !value.empty())
	{
		m_model->setSpaceFromTheTable(fvalue);
	}
}

void CustomLaptopHolder::_ready()
{
	while (get_child_count())
	{
		auto ch = get_child(get_child_count()-1);
		remove_child(ch);
	}
	m_model = memnew(CustomLaptopHolderModel);
	add_child(m_model);
	m_model->set_owner(get_tree()->get_edited_scene_root());

	m_ctrl = std::make_shared<CustomLaptopHolderCtrl>();
	m_ctrl->setModel(m_model);

	m_canvas_layer = memnew(CanvasLayer);
	add_child(m_canvas_layer);
	m_canvas_layer->set_owner(get_tree()->get_edited_scene_root());
	m_configuration_menu = memnew(ConfigurationMenu);
	m_canvas_layer->add_child(m_configuration_menu);
	m_configuration_menu->set_owner(get_tree()->get_edited_scene_root());
	m_configuration_menu->setJsonConfiguration("res://configuration/models/laptopHolder.json");

}

void CustomLaptopHolder::generateStlFile(std::string path)
{
	m_model->generateStlFile(path);
}

void CustomLaptopHolder::_bind_methods()
{}

void CustomLaptopHolder::updateModel()
{
	m_model->updateModel();
}

void CustomLaptopHolderModel::_bind_methods()
{}
void CustomLaptopHolderModel::clearModel()
{	

	while (get_child_count())
	{
		auto ch = get_child(get_child_count()-1);
		remove_child(ch);
	}
}

void CustomLaptopHolderModel::_ready ()
{
	material = memnew(StandardMaterial3D);
	material->set_albedo(Color::html("#292829"));

	updateModel();

}
struct Vector3Hash {
	std::size_t operator()(const Vector3 &v) const {
		return std::hash<float>()(v.x) ^ std::hash<float>()(v.y) ^ std::hash<float>()(v.z);
	}
};

void CustomLaptopHolderModel::generateStlFile(std::string path) {
	std::cout<<path<<std::endl;
	if (path[path.size() - 1] != '/')
		path += "/";
	if (path != "user://")
	{
		auto dir = std::ifstream(path);
		if (!dir)
		{
			std::cout<<"Path is invalid"<<std::endl;
			return;
		}
	}
	{
		auto basePath = path + "outputLaptopHolder.stl";
		auto baseStl = FileAccess::open(basePath.c_str(), FileAccess::WRITE);
	
		Ref<SurfaceTool> surface_tool;
		surface_tool.instantiate();

		auto mesh = Object::cast_to<Mesh>(m_laptopHolder->get_meshes()[1]);
		surface_tool->begin(Mesh::PRIMITIVE_TRIANGLES);

		for (int surface_idx = 0; surface_idx < mesh->get_surface_count(); ++surface_idx) {
			Array arrays = mesh->surface_get_arrays(surface_idx);
			PackedVector3Array vertices = arrays[ArrayMesh::ARRAY_VERTEX];
			PackedVector3Array normals = arrays[ArrayMesh::ARRAY_NORMAL];

			for (int i = 0; i < vertices.size(); ++i) {
				surface_tool->set_normal(normals[i]);
				surface_tool->add_vertex(vertices[i]);
			}
		}

		Ref<ArrayMesh> clean_mesh = surface_tool->commit();
		baseStl->store_line("solid GodotMesh"); 

		std::unordered_map<Vector3, int, Vector3Hash> vertex_map;

		for (int surface_idx = 0; surface_idx < clean_mesh->get_surface_count(); ++surface_idx) {
			Array arrays = clean_mesh->surface_get_arrays(surface_idx);
			PackedVector3Array vertices = arrays[ArrayMesh::ARRAY_VERTEX];
			PackedVector3Array normals = arrays[ArrayMesh::ARRAY_NORMAL];

			for (int i = 0; i < vertices.size(); i += 3) {
				Vector3 normal = normals[i];
				Vector3 transformed_normal = Vector3(normal.x, normal.z, normal.y);
				baseStl->store_line(vformat("facet normal %f %f %f", transformed_normal.x, transformed_normal.y, transformed_normal.z));
				baseStl->store_line("outer loop");

				for (int j = 0; j < 3; ++j) {
					Vector3 vertex = vertices[i + j];
					Vector3 transformed_vertex = Vector3(vertex.x, vertex.z, vertex.y);

					// Deduplicate vertices
					if (vertex_map.find(transformed_vertex) == vertex_map.end()) {
						vertex_map[transformed_vertex] = vertex_map.size();
					}
					baseStl->store_line(vformat("vertex %f %f %f", transformed_vertex.x, transformed_vertex.y, transformed_vertex.z));
				}

				baseStl->store_line("endloop");
				baseStl->store_line("endfacet");
			}
		}

		baseStl->store_line("endsolid GodotMesh");
		baseStl->close();
	}
}

void CustomLaptopHolderModel::updateModel() {
	clearModel();

	m_laptopHolder = memnew(CSGCombiner3D);
	add_child(m_laptopHolder);
	m_laptopHolder->set_owner(get_tree()->get_edited_scene_root());
	m_laptopHolder->set_operation(godot::CSGShape3D::Operation::OPERATION_UNION);
	
	CSGCombiner3D* laptopHolderBody = memnew(CSGCombiner3D);	
	laptopHolderBody->set_operation(godot::CSGShape3D::Operation::OPERATION_UNION);
	m_laptopHolder->add_child(laptopHolderBody);
	laptopHolderBody->set_owner(get_tree()->get_edited_scene_root());
	createLaptopHolderBody(laptopHolderBody);
	
	CSGCombiner3D* laptopCut = memnew(CSGCombiner3D);	
	m_laptopHolder->add_child(laptopCut);
	laptopCut->set_owner(get_tree()->get_edited_scene_root());
	laptopCut->set_operation(godot::CSGShape3D::Operation::OPERATION_SUBTRACTION);
	
	CSGBox3D* cutCube = memnew(CSGBox3D);
	laptopCut->add_child(cutCube);
	cutCube->set_owner(get_tree()->get_edited_scene_root());
	cutCube->set_operation(godot::CSGShape3D::Operation::OPERATION_UNION);
	cutCube->set_material(material);
	cutCube->set_size(Vector3(
				m_laptopWidth,
				m_height,
				m_length+10
	));
	cutCube->set_position(Vector3(0,m_spaceFromTable,0));

}

void CustomLaptopHolderModel::createLaptopHolderBody(CSGCombiner3D* baseInside)
{

	CSGBox3D* mainCube = memnew(CSGBox3D);
	baseInside->add_child(mainCube);
	mainCube->set_owner(get_tree()->get_edited_scene_root());
	mainCube->set_operation(godot::CSGShape3D::Operation::OPERATION_UNION);
	mainCube->set_material(material);
	mainCube->set_size(Vector3(
				m_width*2+m_laptopWidth,
				m_height,
				m_length
	));
	
	std::vector<std::vector<float>> cornerSpherePositions =
	{
		{(m_width*2+m_laptopWidth)/2-m_cornerRadius,+m_height/2-m_cornerRadius,m_length/2-m_cornerRadius},
		{(m_width*2+m_laptopWidth)/2-m_cornerRadius,+m_height/2-m_cornerRadius,-m_length/2+m_cornerRadius},
		{-(m_width*2+m_laptopWidth)/2+m_cornerRadius,+m_height/2-m_cornerRadius,-m_length/2+m_cornerRadius},
		{-(m_width*2+m_laptopWidth)/2+m_cornerRadius,+m_height/2-m_cornerRadius,m_length/2-m_cornerRadius}
	};

	std::vector<std::vector<float>> sideCubePositions =
	{
		{0,+m_height/2,m_length/2},
		{-(m_width*2+m_laptopWidth)/2,+m_height/2,0},
		{0,+m_height/2,-m_length/2},
		{(m_width*2+m_laptopWidth)/2,+m_height/2,0},
		{(m_width*2+m_laptopWidth)/2,0,m_length/2},
		{(m_width*2+m_laptopWidth)/2,0,-m_length/2},
		{-(m_width*2+m_laptopWidth)/2,0,-m_length/2},
		{-(m_width*2+m_laptopWidth)/2,0,m_length/2}
	};

	std::vector<std::vector<float>> sideCubeSizes =
	{
		{(m_width*2+m_laptopWidth),m_cornerRadius*2,m_cornerRadius*2},
		{m_cornerRadius*2,m_cornerRadius*2,m_length},
		{(m_width*2+m_laptopWidth),m_cornerRadius*2,m_cornerRadius*2},
		{m_cornerRadius*2,m_cornerRadius*2,m_length},
		{m_cornerRadius*2,m_height,m_cornerRadius*2},
		{m_cornerRadius*2,m_height,m_cornerRadius*2},
		{m_cornerRadius*2,m_height,m_cornerRadius*2},
		{m_cornerRadius*2,m_height,m_cornerRadius*2}
	};
	std::vector<std::vector<float>> sideCylinderPositions =
	{
		{(m_width*2+m_laptopWidth)/2-m_cornerRadius,m_height/2-m_cornerRadius,0},
		{-(m_width*2+m_laptopWidth)/2+m_cornerRadius,m_height/2-m_cornerRadius,0},
		{0,m_height/2-m_cornerRadius,-m_length/2+m_cornerRadius},
		{0,m_height/2-m_cornerRadius,m_length/2-m_cornerRadius},
		{(m_width*2+m_laptopWidth)/2-m_cornerRadius,-m_cornerRadius/2,m_length/2-m_cornerRadius},
		{(m_width*2+m_laptopWidth)/2-m_cornerRadius,-m_cornerRadius/2,-m_length/2+m_cornerRadius},
		{-(m_width*2+m_laptopWidth)/2+m_cornerRadius,-m_cornerRadius/2,m_length/2-m_cornerRadius},
		{-(m_width*2+m_laptopWidth)/2+m_cornerRadius,-m_cornerRadius/2,-m_length/2+m_cornerRadius}
	};

	CSGCombiner3D* corner = memnew(CSGCombiner3D);
	baseInside->add_child(corner);
	corner->set_operation(godot::CSGShape3D::Operation::OPERATION_SUBTRACTION);

	for (int i = 0;i<8;i++)
	{
		CSGBox3D* cube = memnew(CSGBox3D);
		corner->add_child(cube);
		cube->set_operation(godot::CSGShape3D::Operation::OPERATION_UNION);
		cube->set_size(Vector3(sideCubeSizes[i][0],sideCubeSizes[i][1],sideCubeSizes[i][2]));
		cube->set_position(Vector3(sideCubePositions[i][0],sideCubePositions[i][1],sideCubePositions[i][2]));
	}
	for (int i = 0;i<4;i++)
	{
		CSGSphere3D* sphere = memnew(CSGSphere3D);
		baseInside->add_child(sphere);
		sphere->set_operation(godot::CSGShape3D::Operation::OPERATION_UNION);
		sphere->set_owner(get_tree()->get_edited_scene_root());
		sphere->set_material(material);
		sphere->set_radial_segments(24);
		sphere->set_rings(24);
		sphere->set_radius(m_cornerRadius);
		sphere->set_position(Vector3(cornerSpherePositions[i][0],cornerSpherePositions[i][1],cornerSpherePositions[i][2]));
	}
	for (int i = 0;i<2;i++)
	{
		CSGCylinder3D* cylinder = memnew(CSGCylinder3D);
		baseInside->add_child(cylinder);
		cylinder->set_operation(godot::CSGShape3D::Operation::OPERATION_UNION);
		cylinder->set_owner(get_tree()->get_edited_scene_root());
		cylinder->set_material(material);
		cylinder->set_height(m_length-m_cornerRadius*2);
		cylinder->set_global_rotation_degrees(Vector3(90,0,0));
		cylinder->set_sides(24);
		cylinder->set_radius(m_cornerRadius);
		cylinder->set_position(Vector3(sideCylinderPositions[i][0],sideCylinderPositions[i][1],sideCylinderPositions[i][2]));
	}
	for (int i = 2;i<4;i++)
	{
		CSGCylinder3D* cylinder = memnew(CSGCylinder3D);
		baseInside->add_child(cylinder);
		cylinder->set_operation(godot::CSGShape3D::Operation::OPERATION_UNION);
		cylinder->set_owner(get_tree()->get_edited_scene_root());
		cylinder->set_material(material);
		cylinder->set_height((m_width*2+m_laptopWidth)-m_cornerRadius*2);
		cylinder->set_global_rotation_degrees(Vector3(90,90,0));
		cylinder->set_sides(24);
		cylinder->set_radius(m_cornerRadius);
		cylinder->set_position(Vector3(sideCylinderPositions[i][0],sideCylinderPositions[i][1],sideCylinderPositions[i][2]));
	}
	for (int i = 4;i<8;i++)
	{
		CSGCylinder3D* cylinder = memnew(CSGCylinder3D);
		baseInside->add_child(cylinder);
		cylinder->set_operation(godot::CSGShape3D::Operation::OPERATION_UNION);
		cylinder->set_owner(get_tree()->get_edited_scene_root());
		cylinder->set_material(material);
		cylinder->set_height(m_height-m_cornerRadius);
		cylinder->set_sides(24);
		cylinder->set_radius(m_cornerRadius);
		cylinder->set_position(Vector3(sideCylinderPositions[i][0],sideCylinderPositions[i][1],sideCylinderPositions[i][2]));
	}
}

void CustomLaptopHolderModel::setSideWidth(float width)
{
	m_width = width;
	updateModel();
}

void CustomLaptopHolderModel::setHeight(float height)
{
	m_height = height;
	updateModel();
}

void CustomLaptopHolderModel::setLength(float length)
{
	m_length = length;
	updateModel();
}

void CustomLaptopHolderModel::setLaptopWidth(float laptopWidth)
{
	m_laptopWidth = laptopWidth;
	updateModel();
}

void CustomLaptopHolderModel::setCornerRadius(float radius)
{
	if (radius*2>m_height)
		m_cornerRadius=m_height/2;
	else
		m_cornerRadius = radius;
	updateModel();
}

void CustomLaptopHolderModel::setSpaceFromTheTable(float space)
{
	m_spaceFromTable = space;
	updateModel();
}
