# Darwin

Darwin is an open-source, extensible, and modular evolution simulator. It's primarily designed for evolution of entities based on neural networks.

# Getting Started

The extensions of Darwin usually relies on few basic components, which includes OpenNN (for neural networks), SQLite3 (for database), FlatBuffers (for serialization/deserialization), etc.

Please note:

- The include files for such components are located in the `/includes` directory.
- Extensions are placed in the `/extensions` directory.

## Building

Darwin is technically a cocos2d-x project, so please visit the website of [Cocos Project](https://www.cocos.com) to download Cocos Console, which is recommended for building Darwin or any other cocos2d-x project.

To build Darwin, run `cocos compile -p <platform>` inside the root directory. Replace "\<platform>" with the platform of your choice; run `cocos compile --help` for list of available platforms.

Note: Since the CLI tool, Cocos Console, uses CMake to generate the files required for building, you can use CMake directly instead. In fact, using CMake directly can give you more control over the build process.

# Dependencies

- __wxWidgets__ for cross-platform GUI of the Control Panel.
- __cocos2d-x__ for visual simulation of evolution.
- __SQLite3__ for relational data storage.
- __FlatBuffers__ for binary data serialization/deserialization.

## wxWidgets, cocos2d-x, and SQLite3

wxWidgets, cocos2d-x, SQLite3 are stored in `/wxwidgets`, `/cocos2d`, and `/core/sqlite3` respectively, as [sub-directories](https://cmake.org/cmake/help/latest/command/add_subdirectory.html).

## FlatBuffers

FlatBuffers is used for storing/retrieving states of generations, along with scores of population members, to/from the database in form of binary. 

FlatBuffers schema files are included in `/extensions/<extension_name>/flatbuffers`, where "\<extension_name>" is the name of the extension. You can use FlatBuffers schema compiler to compile the schemas. Afterwards, place the newly generated files in their respective locations.

Note that FlatBuffers isn't used as a library, but as a tool which generates header files. The header files are what makes serialization/deserialization of data possible.