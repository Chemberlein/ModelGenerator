#include "register_types.h"

#include <gdextension_interface.h>

#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/core/defs.hpp>
#include <godot_cpp/godot.hpp>

#include "coreApp.hpp"
#include "core/elements/ui/sideMenu.hpp" 
#include "core/elements/ui/configurationMenu.hpp" 
#include "core/elements/ui/customSection.hpp" 
#include "core/elements/ui/accordionSection.hpp" 
#include "core/elements/ui/cLineEdit.hpp" 
#include "core/elements/ui/cSpinBox.hpp" 
#include "core/elements/ui/cButton.hpp"
#include "core/elements/ui/cDropDown.hpp"
#include "core/elements/ui/cCheckButton.hpp"
#include "core/elements/objects/cModel.hpp"
#include "core/elements/environment/skyBox.hpp"
#include "models/container/container.hpp"
#include "models/laptopholder/laptopHolder.hpp"
#include "core/elements/camera/modelOverviewCamera.hpp"

using namespace godot;

void initialize_test_module(ModuleInitializationLevel p_level) {
	if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) {
		return;
	}

	ClassDB::register_class<CustomSection>();
	ClassDB::register_class<AccordionSection>();
	ClassDB::register_class<CoreApp>();
	ClassDB::register_class<SideMenu>();
	ClassDB::register_class<ConfigurationMenu>();
	ClassDB::register_class<CLineEdit>();
	ClassDB::register_class<CSpinBox>();
	ClassDB::register_class<CButton>();
	ClassDB::register_class<CDropDown>();
	ClassDB::register_class<CCheckButton>();
	ClassDB::register_class<SkyBox>();
	ClassDB::register_class<ModelOverviewCamera>();	
	ClassDB::register_class<CustomContainerModel>();
	ClassDB::register_class<CustomContainer>();	
	ClassDB::register_class<CustomLaptopHolderModel>();
	ClassDB::register_class<CustomLaptopHolder>();	
	ClassDB::register_class<CModel>();	
}

void uninitialize_test_module(ModuleInitializationLevel p_level) {
	if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) {
		return;
	}
}

extern "C" {
// Initialization.
GDExtensionBool GDE_EXPORT test_library_init(GDExtensionInterfaceGetProcAddress p_get_proc_address, GDExtensionClassLibraryPtr p_library, GDExtensionInitialization *r_initialization) {
	godot::GDExtensionBinding::InitObject init_obj(p_get_proc_address, p_library, r_initialization);

	init_obj.register_initializer(initialize_test_module);
	init_obj.register_terminator(uninitialize_test_module);
	init_obj.set_minimum_library_initialization_level(MODULE_INITIALIZATION_LEVEL_SCENE);

	return init_obj.init();
}
}
