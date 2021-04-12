#ifndef __MAIN_H__
#define __MAIN_H__

#include <string>

extern "C"
{
    struct Range
    {
        float start;
        float end;
    };

    struct Model
    {
        const char *definition;
        float fitness;
    };

    bool pm_load_definition(const char *);
    bool pm_load_file(const char *);
    bool pm_save_file(const char *);
    bool pm_unload();
    void pm_blank();

    const char *pm_to_json();

    int pm_count();
    Range pm_fitness_range();
    Model pm_get_model(int index);

    void pm_add_stage(const char *, float);
    void pm_commit(int);
}

#endif // __MAIN_H__