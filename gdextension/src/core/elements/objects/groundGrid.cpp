#include "BuildPlate.hpp"


void BuildPlate::_ready() {
	material = memnew(StandardMaterial3D);
	material->set_albedo(Color::html("#495867")); // Set material color to red
	generate_grid_in_cells();
}

void BuildPlate::generate_grid_in_cells() {
	while (get_child_count())
	{
		auto ch = get_child(get_child_count()-1);
		remove_child(ch);
	}

    int rows = (m_z_end - m_z_start)/m_spacing;
    int columns = (m_x_end - m_x_start)/m_spacing;
	for (int i = 0; i <= rows; i++) {
		create_line(Vector3(m_x_start + i * m_spacing, m_depth - m_line_thickness, 0), Vector3(m_line_thickness, m_line_thickness, m_z_end - m_z_start));
	}

	for (int j = 0; j <= columns; j++) {
		create_line(Vector3(0, m_depth - m_line_thickness,m_z_start + j * m_spacing), Vector3(m_x_end - m_x_start, m_line_thickness, m_line_thickness));
	}
}

void BuildPlate::set_depth(float d)
{
	m_depth = d;
	generate_grid_in_cells();
}
void BuildPlate::create_line(Vector3 position, Vector3 size) {
	// Создание MeshInstance3D для каждой линии
	MeshInstance3D *line_instance = memnew(MeshInstance3D);
	// Задаем BoxMesh для линии
	Ref<BoxMesh> line_mesh;
	line_mesh.instantiate();
	line_mesh->set_size(size);  // Устанавливаем размеры линии
	line_instance->set_mesh(line_mesh);

	// Устанавливаем позицию линии
	Transform3D line_transform;
	line_transform.origin = position;
	line_instance->set_transform(line_transform);

	// Добавляем линию как дочерний элемент
	add_child(line_instance);
	if (line_instance->get_mesh().is_valid()) {
	    line_instance->set_surface_override_material(0, material); // Set material to surface 0
	}
}