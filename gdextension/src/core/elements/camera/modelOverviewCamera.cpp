#include "modelOverviewCamera.hpp"
#include <godot_cpp/classes/viewport.hpp>
#include <godot_cpp/classes/input_map.hpp>
void ModelOverviewCamera::_ready()
{
	while (get_child_count())
	{
		auto ch = get_child(get_child_count()-1);
		remove_child(ch);
	}

	m_camera_target = memnew(Node3D);
	add_child(m_camera_target);
	m_camera_target->set_owner(get_tree()->get_edited_scene_root());

	m_camera = memnew(Camera3D);
	m_camera_target->add_child(m_camera);
	m_camera->set_owner(get_tree()->get_edited_scene_root());

	m_light = memnew(DirectionalLight3D);
	m_camera_target->add_child(m_light);
	m_light->set_owner(get_tree()->get_edited_scene_root());

	m_camera->set_global_position({0,0,200});

	m_light->set_global_position({0,0,200});
	
	set_rotation_degrees(Vector3(-32, 0, 0));
}

void ModelOverviewCamera::_bind_methods()
{

}

void ModelOverviewCamera::_process(double delta)
{
	if (InputMap::get_singleton()->has_action("cam_move")) {
		if (Input::get_singleton()->is_action_just_pressed("cam_move"))
		{
			old_rotation = get_rotation_degrees();
			click_position = get_viewport()->get_mouse_position();
		}
		if (Input::get_singleton()->is_action_pressed("cam_move"))
		{
			auto rot_x = godot::Math::clamp(old_rotation.x - (get_viewport()->get_mouse_position().y - click_position.y), real_t(-90.0), real_t(90.0));
			auto rot_y = old_rotation.y - (get_viewport()->get_mouse_position().x - click_position.x);
			rot_x = std::remainder(rot_x, 360.0);
			rot_y = std::remainder(rot_y, 360.0);
			set_rotation_degrees(Vector3(rot_x, rot_y, 0));
		}	
		if (Input::get_singleton()->is_action_pressed("cam_move") && Input::get_singleton()->is_action_just_pressed("cam_forward"))
		{
			auto pos = m_camera_target->get_position();
			pos.z = pos.z + 8;
			m_camera_target->set_position(pos);
		}
		if (Input::get_singleton()->is_action_pressed("cam_move") && Input::get_singleton()->is_action_just_pressed("cam_backward"))
		{
			auto pos = m_camera_target->get_position();
			pos.z = pos.z - 8;
			m_camera_target->set_position(pos);
		}
	}
}
