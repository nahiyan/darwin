use serde::{Deserialize, Serialize};
use serde_json::{json, Result as SJResult};
use std::cmp::Ordering;
use std::fs::File;
use std::io::Read;
use std::io::{BufReader, Error as IOError, ErrorKind, Write};

#[derive(Serialize, Deserialize, PartialEq, Clone)]
pub struct Model {
    pub definition: String,
    pub fitness: f32,
}

impl Model {
    pub fn new(definition: String, fitness: f32) -> Model {
        Model {
            definition: definition,
            fitness: fitness,
        }
    }
}

#[derive(PartialEq, Clone)]
pub struct PersistentModels {
    pub models: Vec<Model>,
    pub stage: Vec<Model>,
}

impl PersistentModels {
    pub fn blank() -> PersistentModels {
        PersistentModels {
            models: vec![],
            stage: vec![],
        }
    }

    pub fn load_definition(definition: &str) -> SJResult<PersistentModels> {
        #[derive(Serialize, Deserialize)]
        struct PersistentModelsIntermediate {
            models: Vec<Model>,
        }

        let v: PersistentModelsIntermediate = serde_json::from_str(definition)?;
        let p = PersistentModels {
            models: v.models,
            stage: vec![],
        };

        Ok(p)
    }

    pub fn to_json(&self) -> String {
        let json = json!({
            "models": self.models
        });

        json.to_string()
    }

    pub fn len(&self) -> usize {
        self.models.len()
    }

    pub fn get(&self, index: usize) -> Model {
        self.models[index].clone()
    }

    pub fn add_to_stage(&mut self, definition: &str, fitness: f32) {
        self.stage.push(Model {
            definition: String::from(definition),
            fitness: fitness,
        });
    }

    pub fn commit(&mut self, amount: usize) {
        self.models.append(&mut self.stage);
        self.models.sort_by(|a: &Model, b: &Model| {
            if a.fitness < b.fitness {
                Ordering::Greater
            } else if a.fitness == b.fitness {
                Ordering::Equal
            } else {
                Ordering::Less
            }
        });

        while self.models.len() > amount {
            self.models.pop();
        }
    }

    pub fn load_file(file_path: &str) -> Result<PersistentModels, IOError> {
        let file = File::open(file_path)?;
        let mut buf_reader = BufReader::new(file);
        let mut contents = String::new();
        buf_reader.read_to_string(&mut contents)?;

        match PersistentModels::load_definition(contents.as_str()) {
            Ok(p) => Ok(p),
            _ => Err(IOError::new(ErrorKind::Other, "Failed to load definition")),
        }
    }

    pub fn save_file(&self, file_path: &str) -> Result<(), IOError> {
        let mut file = File::create(file_path)?;
        file.write_all(self.to_json().as_bytes())?;

        Ok(())
    }
}

#[cfg(test)]
mod tests {
    use super::*;
    use speculate::speculate;
    use std::fs;

    speculate! {
        const SAMPLE_DEFINITION: &str = r#"{"models":[{"definition":"one","fitness":300.0},{"definition":"two","fitness":100.0},{"definition":"three","fitness":1000.0}]}"#;

        before {
            let _sample_models = PersistentModels {
                models: vec![
                    Model::new(String::from("one"), 300.0),
                    Model::new(String::from("two"), 100.0),
                    Model::new(String::from("three"), 1000.0),
                ],
                stage: vec![]
            };
        }

        describe "definition load/generate" {
            it "loads definition" {
                assert_eq!(
                    true,
                    match PersistentModels::load_definition(SAMPLE_DEFINITION) {
                        Ok(models) => models == _sample_models,
                        _ => false,
                    }
                );
            }

            it "represents definition as JSON" {
                assert_eq!(
                    SAMPLE_DEFINITION,
                    _sample_models
                    .to_json()
                );
            }
        }

        describe "loads files" {
            after {
                fs::remove_file("load.json").expect("Failed to remove load.json.");
            }

            it "loads from file" {
                let mut file = File::create("load.json").expect("Failed to create load.json.");
                file.write_all(SAMPLE_DEFINITION.as_bytes())
                    .expect("Failed to write to file.");

                let models = PersistentModels::load_file("load.json").expect("Failed to load load.json.");
                assert_eq!(true, _sample_models == models);
            }
        }

        describe "saves files" {
            after {
                fs::remove_file("save.json").expect("Failed to remove save.json.");
            }

            it "saves to file" {
                _sample_models
                    .save_file("save.json")
                    .expect("Failed to save JSON file.");

                let file = File::open("save.json").expect("Failed to open save.json");
                let mut buf_reader = BufReader::new(file);
                let mut contents = String::new();
                buf_reader
                    .read_to_string(&mut contents)
                    .expect("Failed to read contents of save.json.");

                assert_eq!(
                    SAMPLE_DEFINITION,
                    contents
                );
            }
        }

        describe "manages models" {
            it "counts models" {
                assert_eq!(0, PersistentModels::blank().len());
                assert_eq!(3, _sample_models.len());
            }
            it "fetches models" {
                assert_eq!(true, _sample_models.models[0] == _sample_models.get(0));
            }
            it "stages models" {
                let mut models = PersistentModels::blank();
                models.add_to_stage("Hello", 1.5);
                models.add_to_stage("Hello2", 2000.0);
                let expected_stage = vec![
                    Model::new(String::from("Hello"), 1.5),
                    Model::new(String::from("Hello2"), 2000.0),
                ];
                assert_eq!(true, models.stage == expected_stage);
            }

            it "commits" {
                let mut models = _sample_models.clone();
                models.add_to_stage("Hello", 1.5);
                models.add_to_stage("Hello2", 2000.0);
                models.commit(2);
                let expected_stage = vec![];
                let expected_models = vec![
                    Model {
                        definition: String::from("Hello2"),
                        fitness: 2000.0,
                    },
                    Model {
                        definition: String::from("three"),
                        fitness: 1000.0,
                    },
                ];

                assert_eq!(true, models.stage == expected_stage);
                assert_eq!(true, models.models == expected_models);
            }
            it "generates blank models" {
                assert_eq!(true, PersistentModels::blank() == PersistentModels::blank());
            }
        }
    }
}
