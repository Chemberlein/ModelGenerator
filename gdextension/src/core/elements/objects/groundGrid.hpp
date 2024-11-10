#ifndef BUILDPLATE_H
#define BUILDPLATE_H
#include <godot_cpp/classes/mesh_instance3d.hpp>
#include <godot_cpp/classes/box_mesh.hpp>
#include <godot_cpp/classes/node3d.hpp>
#include <godot_cpp/classes/standard_material3d.hpp>

using namespace godot;

class BuildPlate : public Node3D {
    GDCLASS(BuildPlate, Node3D);

protected:
    static void _bind_methods(){};

public:

    void _ready();

    void generate_grid_in_cells();

    void create_line(Vector3 start, Vector3 end);

    void set_depth(float d);
private:


    float m_spacing = 16.0;
    float m_line_thickness = 0.32;
    float m_x_start = -128.0;
    float m_z_start = -128.0;
    float m_x_end = 128.0;
    float m_z_end = 128.0;
    float m_depth = -10;


    StandardMaterial3D* material;
};
#endif