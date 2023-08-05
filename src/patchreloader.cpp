#include "patchreloader.hpp"

using namespace godot;

void PatchReloader::_register_methods() {
    register_method("set_enabled_patch", &PatchReloader::set_enabled_patch);
}

void replace_script_source(Script* script, String from, String to) {
    String script_source = script->get_source_code();
    script_source = script_source.replace(from, to);
    script->set_source_code(script_source);
    script->reload(true);
}

void callback_enable_patch_functions(Script* script) {
    replace_script_source(script, "func __OptimisedMoves_", "func ");
}

void callback_disable_patch_functions(Script* script) {
    replace_script_source(script, "func ", "func __OptimisedMoves_");
}

bool check_if_patch(Script* script, const String& find_patch_id, std::function<void(Script*)> update) {
    if (!script->has_source_code())
        return false;

    const Variant result = script->call("__get_patch_info");
    Godot::print(result.get_type() == Variant::OBJECT ? "TRUE" : "FALSE");
    if (result.get_type() != Variant::OBJECT)
        return false;

    Object* patch_info = Object::___get_from_variant(result);
    Godot::print(patch_info->get_class());

    //const String& script_source = script->get_source_code();
    //Godot::print(script_source);
    //
    //const String patch_id_pattern = "var __patch_id := \"";
    //const size_t patch_id_pattern_size = 19;
    //
    //size_t patch_id_index;
    //if ((patch_id_index = script_source.find(patch_id_pattern)) == -1)
    //    return false;
    //patch_id_index += patch_id_pattern_size;
    //
    //Godot::print(std::to_string(patch_id_index).c_str());
    //const size_t patch_id_end = script_source.find("\"", patch_id_index);
    //Godot::print(std::to_string(patch_id_end).c_str());
    //const String patch_id = script_source.substr(patch_id_index, patch_id_end - patch_id_index);
    //Godot::print(patch_id);
    //
    //Godot::print(patch_id == find_patch_id ? "TRUE" : "FALSE");
    //if (patch_id != find_patch_id)
    //    return false;

    Godot::print(patch_info->get("id") == find_patch_id ? "TRUE" : "FALSE");
    if (patch_info->get("id") != find_patch_id)
        return false;

    Godot::print("MATCH!");
    update(script);
    return true;
}

void find_patch(const String& script_res, const String& find_patch_id, std::function<void(Script*)> update) {
    Script* script;

    ResourceLoader* const loader = ResourceLoader::get_singleton();

    Ref<Script> reference = loader->load(script_res);

    Godot::print(reference.is_valid() ? "TRUE" : "FALSE");
    while (reference.is_valid()) {
        Godot::print(reference.is_valid() ? "TRUE" : "FALSE");
        script = (Script*)reference.ptr();
        Godot::print(script->get_name());
        Godot::print(script->get_path());

        if (check_if_patch(script, find_patch_id, update))
            break;

        reference = script->get_base_script();
    }
}

void PatchReloader::set_enabled_patch(const String script_res, const String find_patch_id, bool state) {
    Godot::print("BEGIN SEARCH");
    find_patch(script_res, find_patch_id, state ? callback_enable_patch_functions : callback_disable_patch_functions);
}
