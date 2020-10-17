#include <filesystem>
#include <core/Filesystem.h>

std::string Filesystem::current()
{
    return std::filesystem::current_path();
}