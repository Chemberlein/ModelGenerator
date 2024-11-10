#ifndef SKYBOX_H
#define SKYBOX_H

#include <godot_cpp/classes/environment.hpp>
#include <godot_cpp/classes/world_environment.hpp>
#include <godot_cpp/classes/sky.hpp>
#include <godot_cpp/classes/procedural_sky_material.hpp>
#include <godot_cpp/classes/sky.hpp>

#include <godot_cpp/classes/input.hpp>
#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/classes/input_event.hpp>
#include <godot_cpp/variant/utility_functions.hpp>
#include <godot_cpp/classes/scene_tree.hpp>

#include <string>
#include <memory>

using namespace godot;

class SkyBox : public WorldEnvironment
{
	GDCLASS(SkyBox, WorldEnvironment);

	public:
		SkyBox();
		void _ready() override;

	protected:
		static void _bind_methods();
	private:
		Ref<Environment> env;
	   	Ref<ProceduralSkyMaterial> procedural_sky; 
		Ref<Sky> procSky;
};
#endif
