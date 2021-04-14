#include <iostream>
#include <fstream>
#include "gtest/gtest.h"
#include "persistent-models.h"

#define DEFINITION_1 "{\"models\":[{\"definition\":\"one\",\"fitness\":300},{\"definition\":\"two\",\"fitness\":100},{\"definition\":\"three\",\"fitness\":1000}]}"
#define FILE_PATH_1 "test/data/definition1.json"
#define FILE_PATH_2 "test/data/definition2.json"

using namespace std;

TEST(LoadTest, LoadsSuccessfully)
{
    ASSERT_EQ(true, pm_load_definition(DEFINITION_1));
    ASSERT_EQ(3, pm_count());
    auto range = pm_fitness_range();
    ASSERT_FLOAT_EQ(100, range.start);
    ASSERT_FLOAT_EQ(1000, range.end);
    ASSERT_STREQ("one", pm_get_model(0).definition);
    ASSERT_STREQ("two", pm_get_model(1).definition);
    ASSERT_FLOAT_EQ(1000, pm_get_model(2).fitness);
}

TEST(LoadTest, UnloadsSuccessfully)
{
    ASSERT_EQ(true, pm_unload());
}

TEST(CommitTest, CommitsProperly)
{
    pm_load_definition(DEFINITION_1);
    pm_add_stage("Four", 500);
    pm_add_stage("Five", 10.5);
    pm_add_stage("Six", 900);
    pm_add_stage("Seven", 2000.5);
    pm_commit(3);

    ASSERT_EQ(3, pm_count());
    auto range = pm_fitness_range();
    ASSERT_FLOAT_EQ(900, range.start);
    ASSERT_FLOAT_EQ(2000.5, range.end);
    ASSERT_FLOAT_EQ(2000.5, pm_get_model(0).fitness);
    ASSERT_STREQ("Six", pm_get_model(2).definition);
}

TEST(JsonSerialization, SerializesJsonProperly)
{
    pm_load_definition(DEFINITION_1);
    shared_ptr<const char *> json = make_shared<const char *>(pm_to_json());
    ASSERT_STREQ("{\"models\":[{\"definition\":\"one\",\"fitness\":300.0},{\"definition\":\"two\",\"fitness\":100.0},{\"definition\":\"three\",\"fitness\":1000.0}]}", *json);
}

TEST(FileBasedOperations, LoadsFromFile)
{
    ASSERT_EQ(true, pm_load_file(FILE_PATH_1));
    ASSERT_EQ(3, pm_count());
    auto range = pm_fitness_range();
    ASSERT_FLOAT_EQ(100, range.start);
    ASSERT_FLOAT_EQ(1000, range.end);
    ASSERT_STREQ("one", pm_get_model(0).definition);
    ASSERT_STREQ("two", pm_get_model(1).definition);
    ASSERT_FLOAT_EQ(1000, pm_get_model(2).fitness);
}

TEST(FileBasedOperations, SaveToFile)
{
    pm_blank();
    pm_add_stage("Hey", 3.14);
    pm_commit(10);
    pm_save_file(FILE_PATH_2);

    try
    {
        {
            ifstream file(FILE_PATH_2);
            char line[200];
            file.getline(line, 200);
            ASSERT_STREQ("{\"models\":[{\"definition\":\"Hey\",\"fitness\":3.140000104904175}]}", line);
            file.close();
        }
        ofstream file;
        file.open(FILE_PATH_2, std::ofstream::out | std::ofstream::trunc);
        file.close();
    }
    catch (...)
    {
    }
}

// TEST(GenerateTest, GeneratesProperString)
// {
//     pm_load_definition(DEFINITION1);

// }

// class QueryTest : public ::testing::Test
// {
// public:
//     void SetUp() override
//     {
//         Connect(DB_PATH);
//     }

//     void TearDown() override
//     {
//         Disconnect();
//     }
// };

// class DataBasedTest : public QueryTest
// {
// protected:
//     void SetUp() override
//     {
//         QueryTest::SetUp();
//         ResetTables();
//     }

//     void TearDown() override
//     {
//         ResetTables();
//         QueryTest::TearDown();
//     }
// };

// TEST(VersionTest, ReturnsCorrectVersion)
// {
//     ASSERT_EQ(GetSqliteVersion(), SQLITE_VERSION_INT);
// }

// TEST(ConnectionTest, ConnectsProperly)
// {
//     ASSERT_EQ(SQLITE_OK, Connect(DB_PATH));
// }

// TEST(ConnectionTest, DisconnectsProperly)
// {
//     ASSERT_EQ(SQLITE_OK, Disconnect());
// }

// TEST_F(DataBasedTest, ConnectsAndSetsUp)
// {
//     ASSERT_EQ(SQLITE_CONSTRAINT, ConnectAndSetup(DB_PATH));
// }

// TEST_F(QueryTest, ResetsTables)
// {
//     ASSERT_EQ(SQLITE_OK, ResetTables());
// }

// TEST_F(DataBasedTest, FetchesCorrectExtensionId)
// {
//     ASSERT_EQ(2, GetExtensionId("neat-pack"));
//     ASSERT_EQ(0, GetExtensionId("non-existent-extension"));
// }

// TEST_F(DataBasedTest, InsertsModel)
// {
//     ASSERT_EQ(SQLITE_OK, AddModel("test", 1, 3.14));
// }

// TEST_F(DataBasedTest, FetchesModel)
// {
//     ASSERT_EQ(SQLITE_OK, AddModel("test", 1, 3.14));

//     Model *model = GetModel(1);
//     ASSERT_STREQ("test", model->content);
//     ASSERT_EQ(1, model->extension_id);
//     ASSERT_FLOAT_EQ(3.14, model->fitness);

//     Model *model2 = GetModel(2);
//     ASSERT_EQ(NULL, model2->content);
//     ASSERT_EQ(-1, model2->extension_id);
//     ASSERT_FLOAT_EQ(-1, model2->fitness);

//     UnloadModel(model);
//     UnloadModel(model2);
// }

// TEST_F(DataBasedTest, FetchesBestModel)
// {
//     AddModel("test1", 1, 3.14);
//     AddModel("test2", 2, 5.7);
//     AddModel("test3", 2, 13.5);
//     AddModel("test4", 1, 0);
//     AddModel("test5", 1, -3);
//     AddModel("test6", 1, 10.3);
//     AddModel("test7", 1, 25.2);
//     AddModel("test8", 1, 1.5);

//     ModelCollection *collection = GetBestModelsCollection(5, 1);
//     ASSERT_NE(nullptr, collection);
//     ASSERT_FLOAT_EQ(25.2, collection->models[0]->fitness);
//     ASSERT_FLOAT_EQ(10.3, collection->models[1]->fitness);
//     ASSERT_FLOAT_EQ(3.14, collection->models[2]->fitness);
//     ASSERT_FLOAT_EQ(1.5, collection->models[3]->fitness);
//     ASSERT_FLOAT_EQ(0, collection->models[4]->fitness);

//     UnloadCollection(collection);
// }

// TEST_F(DataBasedTest, FetchesAllModels)
// {
//     AddModel("test1", 1, 3.14);
//     AddModel("test2", 2, 5.7);
//     AddModel("test3", 2, 13.5);
//     AddModel("test4", 1, 0);
//     AddModel("test5", 1, -3);
//     AddModel("test6", 1, 10.3);
//     AddModel("test7", 1, 25.2);
//     AddModel("test8", 1, 1.5);

//     ModelCollection *collection = GetBestModelsCollection(-1, 1);
//     ASSERT_NE(nullptr, collection);
//     ASSERT_EQ(6, collection->size);

//     UnloadCollection(collection);
// }

// TEST_F(DataBasedTest, KeepsOnlyMostFitModels)
// {
//     AddModel("test1", 1, 3.14);
//     AddModel("test2", 2, 5.7);
//     AddModel("test3", 2, 13.5);
//     AddModel("test4", 1, 0);
//     AddModel("test5", 1, -3);
//     AddModel("test6", 1, 10.3);
//     AddModel("test7", 1, 25.2);
//     AddModel("test8", 1, 1.5);

//     ASSERT_EQ(SQLITE_OK, TrimModelsTable(3, 1));
//     ModelCollection *collection = GetBestModelsCollection(-1, 1);
//     ASSERT_EQ(3, collection->size);

//     UnloadCollection(collection);
// }

int main()
{
    testing::InitGoogleTest();
    return RUN_ALL_TESTS();
}