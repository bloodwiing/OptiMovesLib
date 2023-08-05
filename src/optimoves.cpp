#include "optimoves.hpp"

using namespace godot;

void OptiMoves::_register_methods() {
    register_method("duplicate_data", &OptiMoves::duplicate_data);

    register_method("copy_properties", &OptiMoves::copy_properties);
    register_method("snapshot_properties", &OptiMoves::snapshot_properties);

    register_method("run_safe", &OptiMoves::run_safe);
}

Variant OptiMoves::duplicate_data(Variant data) {
    switch (data.get_type()) {
    case Variant::DICTIONARY:
        return Dictionary(data);
    case Variant::ARRAY:
        return Array(data);
    default:
        return data;
    }
    /*
    var d = null
    if data:
        if data is Dictionary or data is Array:
            d = data.duplicate()
        else :
            d = data
    return d
    */
}

void plain_copy_properties(Object* a, Object* b) {
    const auto properties = ((Script*)a->get_script())->get_script_property_list();

    const int properties_count = properties.size();
    for (int index = 0; index < properties_count; ++index) {
        const Dictionary& variable = properties[index];

        switch ((int)variable["type"]) {
        case Variant::NIL:
        case Variant::OBJECT:
        case Variant::ARRAY:
        case Variant::DICTIONARY:
            break;
        default:
            const Variant& name = variable["name"];
            b->set(name, a->get(name));
            break;
        }
    }
    /*
    var properties = get_script().get_script_property_list()
    for variable in properties:
        var value = get(variable.name)
        if not (value is Object or value is Array or value is Dictionary):
            state.set(variable.name, value)
    */
}

void diff_copy_properties(Object* a, Object* b, PropertyMap* map) {
    Variant snapshot_variant = a->get("_OptiMoves_Property_Snapshot");

    if (snapshot_variant.get_type() != Variant::ARRAY)
        return;

    Array snapshot = snapshot_variant;

    if (snapshot.size() == 0)
        return;

    int i = 0;
    Variant data;
    for (const auto& prop : *map) {
        if ((data = a->get(prop)) != snapshot[i++]) {
            b->set(prop, data);
        }
    }
}

void OptiMoves::copy_properties(godot::Variant a, godot::Variant b) {
    if (a.get_type() != Variant::OBJECT && b.get_type() != Variant::OBJECT)
        return;

    Object* a_obj = a;
    Object* b_obj = b;

    PropertyMap* map = get_property_template(a);

    if (map == nullptr) {
        plain_copy_properties(a, b);
    }
    else {
        diff_copy_properties(a, b, map);
    }
}

PropertyMap* OptiMoves::get_property_template(Object* obj) {
    const String& path = ((Script*)obj->get_script())->get_path();

    auto result = _templates.find(path);
    if (result != _templates.cend())
        return &((*result).second);
    return nullptr;
}

void record_check_variable_type(Object* obj, const Dictionary& variable, PropertyMap& props) {
    const String& name = variable["name"];
    const Variant& variant = obj->get(name);

    if (name == String("_OptiMoves_Property_Snapshot"))
        return;

    switch (variant.get_type()) {
    case Variant::NIL:
    case Variant::OBJECT:
    case Variant::ARRAY:
    case Variant::DICTIONARY:
        return;
    }

    props.push_back(name);
}

PropertyMap* OptiMoves::record_property_template(Object* obj) {
    PropertyMap* result;

    if ((result = get_property_template(obj)) != nullptr)
        return result;

    const auto properties = ((Script*)obj->get_script())->get_script_property_list();

    PropertyMap props;
    
    const int properties_count = properties.size();
    for (int index = 0; index < properties_count; ++index) {
        const Dictionary& variable = properties[index];

        record_check_variable_type(obj, variable, props);
    }

    const String& path = ((Script*)obj->get_script())->get_path();
    _templates[path] = props;

    return &_templates[path];
}

void OptiMoves::snapshot_properties(godot::Variant var) {
    if (var.get_type() != Variant::OBJECT)
        return;

    Object* obj = var;

    if (obj->get("_OptiMoves_Property_Snapshot").get_type() != Variant::NIL) {
        return;
    }

    PropertyMap* map = record_property_template(obj);

    Array snapshot;

    snapshot.resize((int)map->size());

    int i = 0;
    for (const auto& prop : *map) {
        snapshot[i++] = obj->get(prop);
    }

    obj->set("_OptiMoves_Property_Snapshot", snapshot);
}

bool OptiMoves::run_safe(Object* patch, Object* obj, const godot::String function, const godot::Array args /* = godot::Array() */) {
    try {
        obj->callv(function, args);
        return true;
    }
    catch (std::exception& e) {
        Godot::print(String("OptimsiedMoves: PATCH ERROR ") + e.what());
        patch->call("set_support_state", 0x14, "");
        patch->call("set_enabled_option", false);
        return false;
    }
}
