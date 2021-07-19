import os
os.system(
    "cd godot-cpp; scons generate_bindings=yes target=release -j16; cd ../modules/evolution; python build.py")
