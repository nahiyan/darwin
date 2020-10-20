# Darwin

Darwin is an open-source, extensible, and modular evolution simulator. It's primarily designed for evolution of entities based on neural networks.

# Getting Started

The extensions of Darwin usually relies on few basic components, which includes OpenNN (for neural networks), SQLite3 (for database), FlatBuffers (for serialization/deserialization), etc.

Please note:

- The include files for such components are located in the `/includes` directory.
- The libraries (static or dynamic) are located inside `/libraries` directory.
- Extensions are placed inside `/extensions` directory.

The first step to get started is to build the basic components, which are required by most extensions of Darwin.

## Building the Basic Components

### OpenNN

OpenNN is used by Darwin for its neural networks engine. Download OpenNN's code repository from the [official page](https://www.opennn.net/download/). To build OpenNN, please follow OpenNN's [official guide](https://www.opennn.net/documentation/building_opennn.html). By the end of the guide, you should be having a library of OpenNN (e.g. `libopennn.a`), which you can place inside `libraries/opennn`.

### SQLite3

SQLite3 provides lightweight, server-less database support, which is required by Darwin for persistent data storage. Download the code repository from the [official page](https://www.sqlite.org/download.html) - make sure you download the item labeled "C source code as an amalgamation". Amalgamation is essentially combination of multiple source files into one, simplying the codebase/repository. Afterwards, follow [this guide](https://www.sqlite.org/howtocompile.html) for building SQLite3. Place the built library (e.g. `libsqlite3.a`) inside `libraries/sqlite3`.

## Building Darwin

Once the common libraries, such as OpenNN and SQLite3, are built, you can move on to build Darwin along with its extensions.

Darwin is technically a cocos2d-x project, so please visit the website of [Cocos Project](https://www.cocos.com) to download the CLI tools, which will be required for building Darwin or any other cocos2d-x project.

To build Darwin, run `cocos compile -p <platform>` inside the root directory. Replace "\<platform>" with the platform of your choice; run `cocos compile --help` for list of available platforms.

# Dependencies

- __wxWidgets__ for cross-platform GUI of the Control Panel.
- __cocos2d-x__ for visual simulation of evolution.
- __SQLite3__ for relational data storage.
- __FlatBuffers__ for binary data serialization/deserialization.

## Note

- Repository of wxWidgets is included in `/wxwidgets/`, as it's built from source along with Darwin.
- Repository of cocos2d-x is included in `/cocos2d`, as it's built from source along with Darwin.
- FlatBuffers schema files are included in `/extensions/<extension_name>/flatbuffers`, where "\<extension_name>" is the name of the extension. You can use FlatBuffers schema compiler to compile the schemas. Afterwards, place the newly generated files in their respective locations.