#ifndef MODELOVERVIEWCAMERA_H
#define MODELOVERVIEWCAMERA_H
#include <godot_cpp/classes/node3d.hpp>
#include <godot_cpp/classes/input.hpp>
#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/classes/input_event.hpp>
#include <godot_cpp/variant/utility_functions.hpp>
#include <godot_cpp/classes/scene_tree.hpp>
#include <godot_cpp/classes/directional_light3d.hpp>
#include <godot_cpp/classes/camera3d.hpp>
#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/classes/input_event_key.hpp>
#include <godot_cpp/classes/input.hpp>
#include <godot_cpp/core/binder_common.hpp>

using namespace godot;

class ModelOverviewCamera : public Node3D
{
	GDCLASS(ModelOverviewCamera, Node3D);
	public:
		void _ready() override;
		void _process(double delta) override;
	protected:
		static void _bind_methods();
	private:
		Node3D* m_camera_target;
		Camera3D* m_camera;
		DirectionalLight3D* m_light;
		Vector3 old_rotation;
		Vector2 click_position;
};

#endif
