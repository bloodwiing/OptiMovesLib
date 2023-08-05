#include "optimoves.hpp"
#include "patchreloader.hpp"
#include "OS.hpp"

extern "C" void GDN_EXPORT gdnative_init(godot_gdnative_init_options * o) {
    godot::Godot::gdnative_init(o);
}

extern "C" void GDN_EXPORT gdnative_singleton() {
    
}

extern "C" void GDN_EXPORT gdnative_terminate(godot_gdnative_terminate_options *o) {
    godot::Godot::gdnative_terminate(o);
}

extern "C" void GDN_EXPORT nativescript_init(void *handle) {
    godot::Godot::nativescript_init(handle);

    godot::register_class<OptiMoves>();
    godot::register_class<PatchReloader>();
}
