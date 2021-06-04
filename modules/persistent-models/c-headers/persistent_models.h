#ifndef __PERSISTENT_MODELS_H__
#define __PERSISTENT_MODELS_H__

#include <cstdarg>
#include <cstdint>
#include <cstdlib>
#include <ostream>
#include <new>

struct PersistentModels;

struct PMModel {
  const char *definition;
  float fitness;
};

extern "C" {

PersistentModels *pm_load_definition(const char *definition);

uintptr_t pm_count(PersistentModels **m);

PMModel pm_get_model(PersistentModels **m, uintptr_t index);

void pm_free_string(char *s);

void pm_free_models(PersistentModels *m);

PersistentModels *pm_load_file(const char *file_path);

void pm_save_file(PersistentModels **m, const char *file_path);

void pm_add_to_stage(PersistentModels **m, const char *definition, float fitness);

void pm_commit(PersistentModels **m, uintptr_t amount);

} // extern "C"

#endif // __PERSISTENT_MODELS_H__