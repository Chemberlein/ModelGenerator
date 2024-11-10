#include "container.hpp"
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
CustomContainerCtrl::CustomContainerCtrl()
{}

void CustomContainerCtrl::setModel(CustomContainerModel* model)
{
	m_model = model;
	registerKey("exportAsStl");
	registerKey("containerWidth");
	registerKey("containerHeight");
	registerKey("containerDepth");
	registerKey("containerWallWidth");
	registerKey("lidWallOverlap");
	registerKey("outsideCornerRadius");
	registerKey("innerCornerRadius");
}

void CustomContainerCtrl::registerKey(std::string key)
{
	Ctrl::getInstance().registerBridge(key, std::make_shared<CustomContainerCtrl>(*this));
	m_keys.push_back(key);
}

void CustomContainerCtrl::process(std::string key, std::string value)
{
	std::cout<<key << " " << value<<std::endl;
	if (key == "exportAsStl")
	{
		m_model->generateStlFile(Ctrl::getInstance().getSavePath());
	}
	else if (key == "containerWidth" && !value.empty())
	{
		m_model->setWidth(std::stof(value));
	}
	else if (key == "containerHeight" && !value.empty())
	{
		m_model->setHeight(std::stof(value));
	}
	else if (key == "containerDepth" && !value.empty())
	{
		m_model->setDepth(std::stof(value));
	}
	else if (key == "containerWallWidth" && !value.empty())
	{
		m_model->setWallWidth(std::stof(value));
	}
	else if (key == "lidWallOverlap" && !value.empty())
	{
		m_model->setLidWallOverlap(std::stof(value));
	}
	else if (key == "outsideCornerRadius" && !value.empty())
	{
		m_model->setOutsideCornerRadius(std::stof(value));
	}
	else if (key == "innerCornerRadius" && !value.empty())
	{
		m_model->setInnerCornerRadius(std::stof(value));
	}
}

void CustomContainer::_ready()
{
	while (get_child_count())
	{
		auto ch = get_child(get_child_count()-1);
		remove_child(ch);
	}
	m_model = memnew(CustomContainerModel);
	add_child(m_model);
	m_model->set_owner(get_tree()->get_edited_scene_root());

	m_ctrl = std::make_shared<CustomContainerCtrl>();
	m_ctrl->setModel(m_model);

	m_canvas_layer = memnew(CanvasLayer);
	add_child(m_canvas_layer);
	m_canvas_layer->set_owner(get_tree()->get_edited_scene_root());
	m_configuration_menu = memnew(ConfigurationMenu);
	m_canvas_layer->add_child(m_configuration_menu);
	m_configuration_menu->set_owner(get_tree()->get_edited_scene_root());
	m_configuration_menu->setJsonConfiguration("res://configuration/models/container.json");

}

void CustomContainer::generateStlFile(std::string path)
{
	m_model->generateStlFile(path);
}

void CustomContainer::_exit_tree()
{
	m_configuration_menu->queue_free();
}

void CustomContainer::_bind_methods()
{}

void CustomContainer::updateModel()
{
	m_model->updateModel();
}

void CustomContainerModel::clearModel()
{	

	while (get_child_count())
	{
		auto ch = get_child(get_child_count()-1);
		remove_child(ch);
	}
}

void CustomContainerModel::_ready ()
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

void CustomContainerModel::generateStlFile(std::string path) {
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
		auto basePath = path + "outputBase.stl";
		auto baseStl = FileAccess::open(basePath.c_str(), FileAccess::WRITE);
	
		Ref<SurfaceTool> surface_tool;
		surface_tool.instantiate();

		auto mesh = Object::cast_to<Mesh>(m_base->get_meshes()[1]);
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
	{
		auto lidPath = path + "outputLid.stl";
		auto lidStl = FileAccess::open(lidPath.c_str(), FileAccess::WRITE);

		Ref<SurfaceTool> surface_tool;
		surface_tool.instantiate();

		auto mesh = Object::cast_to<Mesh>(m_lid->get_meshes()[1]);
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
		lidStl->store_line("solid GodotMesh"); 

		std::unordered_map<Vector3, int, Vector3Hash> vertex_map;

		for (int surface_idx = 0; surface_idx < clean_mesh->get_surface_count(); ++surface_idx) {
			Array arrays = clean_mesh->surface_get_arrays(surface_idx);
			PackedVector3Array vertices = arrays[ArrayMesh::ARRAY_VERTEX];
			PackedVector3Array normals = arrays[ArrayMesh::ARRAY_NORMAL];

			for (int i = 0; i < vertices.size(); i += 3) {
				Vector3 normal = normals[i];
				Vector3 transformed_normal = Vector3(normal.x, normal.z, normal.y);
				lidStl->store_line(vformat("facet normal %f %f %f", transformed_normal.x, transformed_normal.y, transformed_normal.z));
				lidStl->store_line("outer loop");

				for (int j = 0; j < 3; ++j) {
					Vector3 vertex = vertices[i + j];
					Vector3 transformed_vertex = Vector3(vertex.x, vertex.z, vertex.y);

					// Deduplicate vertices
					if (vertex_map.find(transformed_vertex) == vertex_map.end()) {
						vertex_map[transformed_vertex] = vertex_map.size();
					}
					lidStl->store_line(vformat("vertex %f %f %f", transformed_vertex.x, transformed_vertex.y, transformed_vertex.z));
				}

				lidStl->store_line("endloop");
				lidStl->store_line("endfacet");
			}
		}

		lidStl->store_line("endsolid GodotMesh");
		lidStl->close();
	}

}

void CustomContainerModel::updateModel() {
	clearModel();

	m_base = memnew(CSGCombiner3D);
	m_lid = memnew(CSGCombiner3D);
	add_child(m_base);
	m_base->set_owner(get_tree()->get_edited_scene_root());
	m_base->set_operation(godot::CSGShape3D::Operation::OPERATION_UNION);
	add_child(m_lid);
	m_lid->set_owner(get_tree()->get_edited_scene_root());
	
	CSGCombiner3D* baseOutside = memnew(CSGCombiner3D);	
	baseOutside->set_operation(godot::CSGShape3D::Operation::OPERATION_UNION);
	m_base->add_child(baseOutside);
	baseOutside->set_owner(get_tree()->get_edited_scene_root());
	createContainerOutside(baseOutside);
	
	CSGCombiner3D* baseInside = memnew(CSGCombiner3D);	
	m_base->add_child(baseInside);
	baseInside->set_owner(get_tree()->get_edited_scene_root());
	createContainerInside(baseInside);
	m_base->set_position(Vector3(0,-m_height/2,0));

	CSGCombiner3D* lidOutside = memnew(CSGCombiner3D);	
	lidOutside->set_operation(godot::CSGShape3D::Operation::OPERATION_UNION);
	m_lid->add_child(lidOutside);
	lidOutside->set_owner(get_tree()->get_edited_scene_root());
	createContainerLidOutside(lidOutside);
	
	CSGCombiner3D* lidInside = memnew(CSGCombiner3D);	
	m_lid->add_child(lidInside);
	lidInside->set_owner(get_tree()->get_edited_scene_root());
	lidInside->set_position(Vector3(0,-m_wallWidth/2-1,0));
	createContainerLidInside(lidInside);
	m_lid->set_position(Vector3(0,m_height,0));
}

void CustomContainerModel::createContainerOutside(CSGCombiner3D* baseOutside)
{

	CSGBox3D* mainCube = memnew(CSGBox3D);
	baseOutside->add_child(mainCube);
	mainCube->set_owner(get_tree()->get_edited_scene_root());
	mainCube->set_operation(godot::CSGShape3D::Operation::OPERATION_UNION);
	mainCube->set_material(material);
	mainCube->set_size(Vector3(
				m_width,
				m_height,
				m_depth
	));

	std::vector<std::vector<float>> sideCubePositions =
	{
		{m_width/2,0,m_depth/2},
		{m_width/2,0,-m_depth/2},
		{-m_width/2,0,-m_depth/2},
		{-m_width/2,0,m_depth/2}
	};

	std::vector<std::vector<float>> sideCubeSizes =
	{
		{m_ocRadius*2,m_height,m_ocRadius*2},
		{m_ocRadius*2,m_height,m_ocRadius*2},
		{m_ocRadius*2,m_height,m_ocRadius*2},
		{m_ocRadius*2,m_height,m_ocRadius*2}
	};
	std::vector<std::vector<float>> sideCylinderPositions =
	{
		{m_width/2-m_ocRadius,0,m_depth/2-m_ocRadius},
		{m_width/2-m_ocRadius,0,-m_depth/2+m_ocRadius},
		{-m_width/2+m_ocRadius,0,m_depth/2-m_ocRadius},
		{-m_width/2+m_ocRadius,0,-m_depth/2+m_ocRadius}
	};

	CSGCombiner3D* corner = memnew(CSGCombiner3D);
	baseOutside->add_child(corner);
	corner->set_operation(godot::CSGShape3D::Operation::OPERATION_SUBTRACTION);

	for (int i = 0;i<4;i++)
	{
		CSGBox3D* cube = memnew(CSGBox3D);
		corner->add_child(cube);
		cube->set_operation(godot::CSGShape3D::Operation::OPERATION_UNION);
		cube->set_size(Vector3(sideCubeSizes[i][0],sideCubeSizes[i][1],sideCubeSizes[i][2]));
		cube->set_position(Vector3(sideCubePositions[i][0],sideCubePositions[i][1],sideCubePositions[i][2]));
	}

	for (int i = 0;i<4;i++)
	{
		CSGCylinder3D* cylinder = memnew(CSGCylinder3D);
		baseOutside->add_child(cylinder);
		cylinder->set_operation(godot::CSGShape3D::Operation::OPERATION_UNION);
		cylinder->set_owner(get_tree()->get_edited_scene_root());
		cylinder->set_material(material);
		cylinder->set_height(m_height);
		cylinder->set_sides(24);
		cylinder->set_radius(m_ocRadius);
		cylinder->set_position(Vector3(sideCylinderPositions[i][0],sideCylinderPositions[i][1],sideCylinderPositions[i][2]));
	}

}

void CustomContainerModel::createContainerLidOutside(CSGCombiner3D* lidOutside)
{
	auto width = m_width + m_wallWidth*2;
	auto depth = m_depth + m_wallWidth*2;
	auto height = m_lidWallOverlap + m_wallWidth;

	CSGBox3D* mainCube = memnew(CSGBox3D);
	lidOutside->add_child(mainCube);
	mainCube->set_owner(get_tree()->get_edited_scene_root());
	mainCube->set_operation(godot::CSGShape3D::Operation::OPERATION_UNION);
	mainCube->set_material(material);
	mainCube->set_size(
		Vector3(
				width,
				height,
				depth
		)
	);

	std::vector<std::vector<float>> sideCubePositions =
	{
		{width/2,0,depth/2},
		{width/2,0,-depth/2},
		{-width/2,0,-depth/2},
		{-width/2,0,depth/2}
	};

	std::vector<std::vector<float>> sideCubeSizes =
	{
		{m_ocRadius*2,height,m_ocRadius*2},
		{m_ocRadius*2,height,m_ocRadius*2},
		{m_ocRadius*2,height,m_ocRadius*2},
		{m_ocRadius*2,height,m_ocRadius*2}
	};
	std::vector<std::vector<float>> sideCylinderPositions =
	{
		{width/2-m_ocRadius,0,depth/2-m_ocRadius},
		{width/2-m_ocRadius,0,-depth/2+m_ocRadius},
		{-width/2+m_ocRadius,0,depth/2-m_ocRadius},
		{-width/2+m_ocRadius,0,-depth/2+m_ocRadius}
	};

	CSGCombiner3D* corner = memnew(CSGCombiner3D);
	lidOutside->add_child(corner);
	corner->set_operation(godot::CSGShape3D::Operation::OPERATION_SUBTRACTION);

	for (int i = 0;i<4;i++)
	{
		CSGBox3D* cube = memnew(CSGBox3D);
		corner->add_child(cube);
		cube->set_operation(godot::CSGShape3D::Operation::OPERATION_UNION);
		cube->set_size(Vector3(sideCubeSizes[i][0],sideCubeSizes[i][1],sideCubeSizes[i][2]));
		cube->set_position(Vector3(sideCubePositions[i][0],sideCubePositions[i][1],sideCubePositions[i][2]));
	}

	for (int i = 0;i<4;i++)
	{
		CSGCylinder3D* cylinder = memnew(CSGCylinder3D);
		lidOutside->add_child(cylinder);
		cylinder->set_operation(godot::CSGShape3D::Operation::OPERATION_UNION);
		cylinder->set_owner(get_tree()->get_edited_scene_root());
		cylinder->set_material(material);
		cylinder->set_height(height);
		cylinder->set_sides(24);
		cylinder->set_radius(m_ocRadius);
		cylinder->set_position(Vector3(sideCylinderPositions[i][0],sideCylinderPositions[i][1],sideCylinderPositions[i][2]));
	}

}

void CustomContainerModel::createContainerLidInside(CSGCombiner3D* lidInside)
{
	lidInside->set_operation(godot::CSGShape3D::Operation::OPERATION_SUBTRACTION);
	auto width = m_width;
	auto depth = m_depth;
	auto height = m_lidWallOverlap+1;

	CSGBox3D* mainCube = memnew(CSGBox3D);
	lidInside->add_child(mainCube);
	mainCube->set_owner(get_tree()->get_edited_scene_root());
	mainCube->set_operation(godot::CSGShape3D::Operation::OPERATION_UNION);
	mainCube->set_material(material);
	mainCube->set_size(
		Vector3(
				width,
				height,
				depth
		)
	);

	std::vector<std::vector<float>> sideCubePositions =
	{
		{width/2,0,depth/2},
		{width/2,0,-depth/2},
		{-width/2,0,-depth/2},
		{-width/2,0,depth/2}
	};

	std::vector<std::vector<float>> sideCubeSizes =
	{
		{m_ocRadius*2,height,m_ocRadius*2},
		{m_ocRadius*2,height,m_ocRadius*2},
		{m_ocRadius*2,height,m_ocRadius*2},
		{m_ocRadius*2,height,m_ocRadius*2}
	};
	std::vector<std::vector<float>> sideCylinderPositions =
	{
		{width/2-m_ocRadius,0,depth/2-m_ocRadius},
		{width/2-m_ocRadius,0,-depth/2+m_ocRadius},
		{-width/2+m_ocRadius,0,depth/2-m_ocRadius},
		{-width/2+m_ocRadius,0,-depth/2+m_ocRadius}
	};

	CSGCombiner3D* corner = memnew(CSGCombiner3D);
	lidInside->add_child(corner);
	corner->set_operation(godot::CSGShape3D::Operation::OPERATION_SUBTRACTION);

	for (int i = 0;i<4;i++)
	{
		CSGBox3D* cube = memnew(CSGBox3D);
		corner->add_child(cube);
		cube->set_operation(godot::CSGShape3D::Operation::OPERATION_UNION);
		cube->set_size(Vector3(sideCubeSizes[i][0],sideCubeSizes[i][1],sideCubeSizes[i][2]));
		cube->set_position(Vector3(sideCubePositions[i][0],sideCubePositions[i][1],sideCubePositions[i][2]));
	}

	for (int i = 0;i<4;i++)
	{
		CSGCylinder3D* cylinder = memnew(CSGCylinder3D);
		lidInside->add_child(cylinder);
		cylinder->set_operation(godot::CSGShape3D::Operation::OPERATION_UNION);
		cylinder->set_owner(get_tree()->get_edited_scene_root());
		cylinder->set_material(material);
		cylinder->set_height(height);
		cylinder->set_sides(24);
		cylinder->set_radius(m_ocRadius);
		cylinder->set_position(Vector3(sideCylinderPositions[i][0],sideCylinderPositions[i][1],sideCylinderPositions[i][2]));
	}

}

void CustomContainerModel::createContainerInside(CSGCombiner3D* baseInside)
{
	baseInside->set_operation(godot::CSGShape3D::Operation::OPERATION_SUBTRACTION);

	const auto insideWidth = m_width - m_wallWidth * 2;
	const auto insideHeight = m_height - m_wallWidth + 1;
	const auto insideDepth = m_depth - m_wallWidth * 2;
	std::vector<float> positionOffset = {0, 1, 0};
	baseInside->set_position(Vector3(positionOffset[0],positionOffset[1],positionOffset[2]));

	CSGBox3D* mainCube = memnew(CSGBox3D);
	baseInside->add_child(mainCube);
	mainCube->set_owner(get_tree()->get_edited_scene_root());
	mainCube->set_operation(godot::CSGShape3D::Operation::OPERATION_UNION);
	mainCube->set_material(material);
	mainCube->set_size(Vector3(
				insideWidth,
				insideHeight,
				insideDepth
	));
	
	std::vector<std::vector<float>> cornerSpherePositions =
	{
		{insideWidth/2-m_icRadius,-insideHeight/2+m_icRadius,insideDepth/2-m_icRadius},
		{insideWidth/2-m_icRadius,-insideHeight/2+m_icRadius,-insideDepth/2+m_icRadius},
		{-insideWidth/2+m_icRadius,-insideHeight/2+m_icRadius,-insideDepth/2+m_icRadius},
		{-insideWidth/2+m_icRadius,-insideHeight/2+m_icRadius,insideDepth/2-m_icRadius}
	};

	std::vector<std::vector<float>> sideCubePositions =
	{
		{0,-insideHeight/2,insideDepth/2},
		{-insideWidth/2,-insideHeight/2,0},
		{0,-insideHeight/2,-insideDepth/2},
		{insideWidth/2,-insideHeight/2,0},
		{insideWidth/2,0,insideDepth/2},
		{insideWidth/2,0,-insideDepth/2},
		{-insideWidth/2,0,-insideDepth/2},
		{-insideWidth/2,0,insideDepth/2}
	};

	std::vector<std::vector<float>> sideCubeSizes =
	{
		{insideWidth,m_icRadius*2,m_icRadius*2},
		{m_icRadius*2,m_icRadius*2,insideDepth},
		{insideWidth,m_icRadius*2,m_icRadius*2},
		{m_icRadius*2,m_icRadius*2,insideDepth},
		{m_icRadius*2,insideHeight,m_icRadius*2},
		{m_icRadius*2,insideHeight,m_icRadius*2},
		{m_icRadius*2,insideHeight,m_icRadius*2},
		{m_icRadius*2,insideHeight,m_icRadius*2}
	};
	std::vector<std::vector<float>> sideCylinderPositions =
	{
		{insideWidth/2-m_icRadius,-insideHeight/2+m_icRadius,0},
		{-insideWidth/2+m_icRadius,-insideHeight/2+m_icRadius,0},
		{0,-insideHeight/2+m_icRadius,-insideDepth/2+m_icRadius},
		{0,-insideHeight/2+m_icRadius,insideDepth/2-m_icRadius},
		{insideWidth/2-m_icRadius,m_icRadius/2,insideDepth/2-m_icRadius},
		{insideWidth/2-m_icRadius,m_icRadius/2,-insideDepth/2+m_icRadius},
		{-insideWidth/2+m_icRadius,m_icRadius/2,insideDepth/2-m_icRadius},
		{-insideWidth/2+m_icRadius,m_icRadius/2,-insideDepth/2+m_icRadius}
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
		sphere->set_radius(m_icRadius);
		sphere->set_position(Vector3(cornerSpherePositions[i][0],cornerSpherePositions[i][1],cornerSpherePositions[i][2]));
	}
	for (int i = 0;i<2;i++)
	{
		CSGCylinder3D* cylinder = memnew(CSGCylinder3D);
		baseInside->add_child(cylinder);
		cylinder->set_operation(godot::CSGShape3D::Operation::OPERATION_UNION);
		cylinder->set_owner(get_tree()->get_edited_scene_root());
		cylinder->set_material(material);
		cylinder->set_height(insideDepth-m_icRadius*2);
		cylinder->set_global_rotation_degrees(Vector3(90,0,0));
		cylinder->set_sides(24);
		cylinder->set_radius(m_icRadius);
		cylinder->set_position(Vector3(sideCylinderPositions[i][0],sideCylinderPositions[i][1],sideCylinderPositions[i][2]));
	}
	for (int i = 2;i<4;i++)
	{
		CSGCylinder3D* cylinder = memnew(CSGCylinder3D);
		baseInside->add_child(cylinder);
		cylinder->set_operation(godot::CSGShape3D::Operation::OPERATION_UNION);
		cylinder->set_owner(get_tree()->get_edited_scene_root());
		cylinder->set_material(material);
		cylinder->set_height(insideWidth-m_icRadius*2);
		cylinder->set_global_rotation_degrees(Vector3(90,90,0));
		cylinder->set_sides(24);
		cylinder->set_radius(m_icRadius);
		cylinder->set_position(Vector3(sideCylinderPositions[i][0],sideCylinderPositions[i][1],sideCylinderPositions[i][2]));
	}
	for (int i = 4;i<8;i++)
	{
		CSGCylinder3D* cylinder = memnew(CSGCylinder3D);
		baseInside->add_child(cylinder);
		cylinder->set_operation(godot::CSGShape3D::Operation::OPERATION_UNION);
		cylinder->set_owner(get_tree()->get_edited_scene_root());
		cylinder->set_material(material);
		cylinder->set_height(insideHeight-m_icRadius);
		cylinder->set_sides(24);
		cylinder->set_radius(m_icRadius);
		cylinder->set_position(Vector3(sideCylinderPositions[i][0],sideCylinderPositions[i][1],sideCylinderPositions[i][2]));
	}
}

void CustomContainerModel::_exit_tree()
{
	m_base->queue_free();
	m_lid->queue_free();
}

void CustomContainerModel::setWidth(float width)
{
	m_width = width;
	updateModel();
}

void CustomContainerModel::setHeight(float height)
{
	m_height = height;
	updateModel();
}

void CustomContainerModel::setDepth(float depth)
{
	m_depth = depth;
	updateModel();
}

void CustomContainerModel::setWallWidth(float wallWidth)
{
	m_wallWidth = wallWidth;
	updateModel();
}

void CustomContainerModel::setHaveLid(bool haveLid)
{
	m_haveLid = haveLid;
	updateModel();
}

void CustomContainerModel::setLidWallOverlap(float lidWallOverlap)
{
	m_lidWallOverlap = lidWallOverlap;
	updateModel();
}

void CustomContainerModel::setOutsideCornersType(float type)
{
	m_icType = type;
	updateModel();
}

void CustomContainerModel::setOutsideCornerRadius(float radius)
{
	m_ocRadius = radius;
	updateModel();
}

void CustomContainerModel::setInnerCornersType(float type)
{
	m_icType = type;
	updateModel();
}

void CustomContainerModel::setInnerCornerRadius(float radius)
{
	m_icRadius = radius;
	updateModel();
}

float CustomContainerModel::getWidth()
{
	return m_width;
}

float CustomContainerModel::getHeight()
{
	return m_height;
}

float CustomContainerModel::getDepth()
{
	return m_depth;
}

float CustomContainerModel::getWallWidth()
{
	return m_wallWidth;
}

bool CustomContainerModel::getHaveLid()
{
	return m_haveLid;
}

float CustomContainerModel::getLidWallOverlap()
{
	return m_lidWallOverlap;
}

float CustomContainerModel::getOutsideCornersType()
{
	return m_ocType;
}

float CustomContainerModel::getOutsideCornerRadius()
{
	return m_ocRadius;
}

float CustomContainerModel::getInnerCornersType()
{
	return m_icType;
}

float CustomContainerModel::getInnerCornerRadius()
{
	return m_icRadius;
}

void CustomContainerModel::_bind_methods()
{

}
