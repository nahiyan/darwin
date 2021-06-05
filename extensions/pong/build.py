import os
os.system(
    "cd godot-cpp; scons generate_bindings=yes; cd ../modules/neat; python build.py")
