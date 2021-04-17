#include <iostream>
#include "../persistent_models.h"

using namespace std;

int main()
{
    PersistentModels *pm = pm_load_definition("{\"models\":[{\"definition\": \"one\", \"fitness\": 100}, {\"definition\": \"one\", \"fitness\": 100}, {\"definition\": \"one\", \"fitness\": 1000}]}");
    cout << pm_count(&pm) << "\n";
    PMModel model = pm_get_model(&pm, 0);
    cout << model.definition << " " << model.fitness << "\n";
    pm_free_string((char *)model.definition);
    pm_free_models(pm);

    return 0;
}