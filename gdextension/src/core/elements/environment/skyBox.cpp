#include "skyBox.hpp"

SkyBox::SkyBox()
{}

void SkyBox::_ready()
{
	env = Ref<Environment>(memnew(Environment));	
	env->set_background(Environment::BG_SKY);

	procedural_sky = Ref<ProceduralSkyMaterial>(memnew(ProceduralSkyMaterial));
	procedural_sky->set_sky_top_color(Color::html("#5A798A"));
	procedural_sky->set_sky_horizon_color(Color::html("#FFFAFF"));

	procedural_sky->set_ground_bottom_color(Color::html("#5A798A"));
	procedural_sky->set_ground_horizon_color(Color::html("#FFFAFF"));
	
	procSky = Ref<Sky>(memnew(Sky));
	procSky->set_material(procedural_sky);

	env->set_sky(procSky);
	set_environment(env);
}
void SkyBox::_bind_methods()
{

}

