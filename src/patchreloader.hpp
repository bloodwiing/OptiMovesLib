#ifndef PATCHRELOADER_HPP
#define PATCHRELOADER_HPP

#include <Godot.hpp>
#include <Node.hpp>
#include <Variant.hpp>
#include <String.hpp>
#include <ResourceLoader.hpp>
#include <Script.hpp>
#include <functional>

class PatchReloader : public godot::Node {
    GODOT_CLASS(PatchReloader, Node);
public:
    PatchReloader() = default;

    void _init() {};

    static void _register_methods();

    void set_enabled_patch(const godot::String script_res, const godot::String find_patch_id, bool state);
};

#endif
