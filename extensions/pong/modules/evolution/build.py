import os
import sys
if len(sys.argv) == 2 and (sys.argv[1] == "windows-x86_64" or sys.argv[1] == "windows"):
    os.system("cmake . -DCMAKE_BUILD_TYPE=Release -Bbuild-windows-x86_64 -GNinja -DCMAKE_TOOLCHAIN_FILE=../../../../toolchains/windows-x86_64/CMakeLists.txt; cmake --build build-windows-x86_64")
else:
    os.system(
        "cmake . -DCMAKE_BUILD_TYPE=Release -Bbuild -GNinja; cmake --build build")
