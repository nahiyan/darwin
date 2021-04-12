#include <iostream>
#include <memory>
#include <vector>
#include <algorithm>
#include <fstream>
#include "main.h"
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/filereadstream.h"

using namespace std;
using namespace rapidjson;

struct ModelI
{
    string definition;
    float fitness;
};

struct Models
{
    vector<ModelI> list;
    vector<ModelI> stage;
};

shared_ptr<Models> models;

extern "C"
{
    bool pm_load_document(Document &document)
    {
        if (document.IsObject() && document.HasMember("models") && document["models"].IsArray())
        {
            models.reset(new Models());

            auto json_models = document["models"].GetArray();
            for (int i = 0; i < json_models.Size(); i++)
            {
                if (!json_models[i].IsObject())
                    return false;

                auto json_model = json_models[i].GetObject();

                if (!(json_model.HasMember("definition") && json_model["definition"].IsString() && json_model.HasMember("fitness") && (json_model["fitness"].IsFloat() || json_model["fitness"].IsInt())))
                    return false;

                auto json_model_definition = json_model["definition"].GetString();
                auto json_model_fitness = json_model["fitness"].GetFloat();

                models->list.push_back(ModelI{json_model_definition, json_model_fitness});
            }
            return true;
        }

        return false;
    }

    bool pm_load_definition(const char *definition)
    {
        Document document;
        document.Parse(definition);

        return pm_load_document(document);
    }

    int pm_count()
    {
        return models->list.size();
    }

    Range pm_fitness_range()
    {
        Range range{-1, 0};

        for (auto model : models->list)
        {
            if (range.start == -1)
                range.start = model.fitness;

            if (model.fitness < range.start)
                range.start = model.fitness;
            else if (model.fitness > range.end)
                range.end = model.fitness;
        }

        return range;
    }

    Model pm_get_model(int index)
    {
        auto model_i = models->list[index];
        return Model{model_i.definition.c_str(), model_i.fitness};
    }

    void pm_add_stage(const char *definition, float fitness)
    {
        models->stage.push_back(ModelI{definition, fitness});
    }

    void pm_commit(int max_size)
    {
        for (auto model : models->stage)
            models->list.push_back(model);

        models->stage.clear();

        auto sorter = [&](ModelI model_a, ModelI model_b) -> bool {
            return model_a.fitness > model_b.fitness;
        };

        sort(models->list.begin(), models->list.end(), sorter);

        while (models->list.size() > max_size)
            models->list.pop_back();
    }

    bool pm_unload()
    {
        models.reset();

        return !models;
    }

    const char *pm_to_json()
    {
        StringBuffer buffer;
        Writer<StringBuffer> writer(buffer);

        writer.StartObject();
        writer.Key("models");
        writer.StartArray();
        for (auto model : models->list)
        {
            writer.StartObject();
            writer.Key("definition");
            writer.String(model.definition.c_str());
            writer.Key("fitness");
            writer.Double(model.fitness);
            writer.EndObject();
        }
        writer.EndArray();
        writer.EndObject();

        char *s = new char[buffer.GetLength()];
        strcpy(s, buffer.GetString());

        return (const char *)s;
    }

    bool pm_save_file(const char *file_path)
    {
        try
        {
            ofstream file;
            file.open(file_path);
            shared_ptr<const char *> json = make_shared<const char *>(pm_to_json());
            file << *json;
            file.close();

            return true;
        }
        catch (...)
        {
            return false;
        }
    }

    bool pm_load_file(const char *file_path)
    {
        FILE *fp = fopen(file_path, "rb"); // non-Windows use "r"

        if (fp == NULL)
            return false;

        char readBuffer[65536];
        FileReadStream is(fp, readBuffer, sizeof(readBuffer));

        Document document;
        document.ParseStream(is);

        fclose(fp);
        return pm_load_document(document);
    }

    void pm_blank()
    {
        models.reset(new Models());
    }
}