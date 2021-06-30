import os
os.system(
    "cd godot-cpp; scons generate_bindings=yes -j16; cd ../modules/neat; python build.py")
