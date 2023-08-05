cd godot-cpp
scons headers_dir="godot-headers/" platform=linux generate_bindings=yes custom_api_file="../api.json" bits=64 target=release use_static_cpp=yes generate_template_get_node=yes
cd ..
