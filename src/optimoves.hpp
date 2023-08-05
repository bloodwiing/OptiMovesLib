#ifndef OPTIMOVES_HPP
#define OPTIMOVES_HPP

#include <Godot.hpp>
#include <Node.hpp>
#include <Variant.hpp>
#include <Script.hpp>
#include <Array.hpp>

#include <unordered_map>
#include <list>

typedef std::list<godot::String> PropertyMap;

template<>
struct std::hash<godot::String>
{
    std::size_t operator()(const godot::String& k) const
    {
        return k.hash();
    }
};

class OptiMoves : public godot::Node {
    GODOT_CLASS(OptiMoves, Node);
public:
    OptiMoves() = default;

    void _init() {};

    static void _register_methods();

    godot::Variant duplicate_data(godot::Variant data);

    void copy_properties(godot::Variant a, godot::Variant b);
    void snapshot_properties(godot::Variant var);

    bool run_safe(Object* patch, Object* obj, const godot::String function, const godot::Array args = godot::Array());

private:
    std::unordered_map<godot::String, PropertyMap> _templates;

    PropertyMap* record_property_template(Object* obj);
    PropertyMap* get_property_template(Object* obj);
};

#endif
