# Darwin

Darwin is an open-source, extensible, and modular evolution simulator. It's primarily designed for evolution of entities based on neural networks.

# Contributors
- [Nahiyan Alamgir](https://github.com/nahiyan): Development and planning
- [Momena Akhter Shukhi](https://github.com/MomenaAkhter): Testing, planning, and documentation.

# Table of Contents
   * [Darwin](#darwin)
   * [Documentation](#documentation)
   * [Getting Started](#getting-started)
      * [Building](#building)
         * [CMake](#cmake)
            * [Linux](#linux)
            * [macOS](#macos)
            * [Windows](#windows)
         * [Cocos Console](#cocos-console)
      * [Post-Build](#post-build)
      * [Running](#running)
   * [Dependencies](#dependencies)
      * [wxWidgets, cocos2d-x, and SQLite3](#wxwidgets-cocos2d-x-and-sqlite3)
      * [FlatBuffers](#flatbuffers)
   * [Web Portal](#web-portal)

# Documentation
Documentation is available at [https://nahiyan.github.io/darwin](https://nahiyan.github.io/darwin).

# Getting Started

The extensions of Darwin usually relies on few basic components, which includes OpenNN (for neural networks), SQLite3 (for database), FlatBuffers (for serialization/deserialization), etc.

Please note:

- The include files for all components are located in the `/includes` directory.
- Extensions are placed in the `/extensions` directory.

## Building

There are two ways to build Darwin. You can choose any one of the methods:

- CMake
- Cocos Console

Important: Before building Darwin, make sure you run the following shell command: `python scripts/gen_db_path.py` in the root (`/`) of your project.


### CMake

Darwin is heavily based on CMake for generating the build system and invoking the build tools. Make sure your CMake version is >= 3.6.

Here are the details for the placeholders used in the commands of each of the methods below:

- `<generator>` Build-system generator.
- `<build-directory>` Directory where the build-system files, along with the compiled files of the project, are going to be stored.

> Note: You can check for the default generator, along with all other available generators using the command: `cmake --help`.

#### Linux

You can use CMake to generate build system files for Ninja, or Unix Make, or any other tool:

`cmake . -G <generator> -B <build-directory>`

Next, build the project with the respective build tool yourself, or letting CMake do it for you:

`cmake --build <build-directory>`

#### macOS

You can use CMake to generate files for Xcode (recommended) or any other tool:

`cmake . -G <generator> -B <build-directory>`

Next, build the project with the respective build tool yourself, or letting CMake do it for you:

`cmake --build <build-directory>`

#### Windows

It's recommended to build a 32-bit version of Darwin on Windows, since that's what cocos2d-x is designed for. To generate the build system with CMake, run in the root of the project:

`cmake . -G <generator> -A Win32 -B <build-directory>`

This will generate the build system, based on 32-bit architecture, in a directory called `<build-directory>`.

It's recommended to use a generator for __Visual Studio__ in CMake. You can omit `-G <generator>` from the command above if the default generator is what you want to use.

Next, you can build the project with the respective build tool, or run to let CMake handle it for you:

`cmake --build <build-directory>`

### Cocos Console

Darwin is technically a cocos2d-x project, so visit the website of [Cocos Project](https://www.cocos.com) to download Cocos Console, which makes it easy to build Darwin, or any other cocos2d-x project, with just one command.

To build Darwin, run `cocos compile -p <platform>` inside the root directory. Replace `<platform>` with the platform of your choice; run `cocos compile --help` for list of available platforms.

> Note: Since the CLI tool, Cocos Console, uses CMake to generate the files required for building, you can use CMake directly instead, as instructed in the other method of building Darwin. In fact, using CMake directly can give you more control over the build process.

## Post-Build

Run the following shell comamnds sequentially, in the `/portal` directory, before running Darwin for the first time:

- `composer install` 
- `php artisan migrate`
- `php artisan db:seed`

This will setup the database so all the extensions can be used.

## Running

After building and running the post-build shell commands, you should be getting an executable file named "Darwin" somewhere inside your `<build-directory>/bin` directory.

# Dependencies

- __wxWidgets__ for cross-platform GUI of the Control Panel.
- __cocos2d-x__ for visual simulation of evolution.
- __SQLite3__ for relational data storage.
- __FlatBuffers__ for binary data serialization/deserialization.

## wxWidgets, cocos2d-x, and SQLite3

wxWidgets, cocos2d-x, SQLite3 are stored in `/wxwidgets`, `/cocos2d`, and `/core/sqlite3` respectively, as [sub-directories](https://cmake.org/cmake/help/latest/command/add_subdirectory.html).

## FlatBuffers

FlatBuffers is used for storing/retrieving states of generations, along with scores of population members, to/from the database in form of binary. 

FlatBuffers schema files are included in `/extensions/<extension_name>/flatbuffers`, where `<extension_name>` is the name of the extension. You can use FlatBuffers schema compiler to compile the schemas. Afterwards, place the newly generated files in their respective locations.

> Note: FlatBuffers isn't used as a library, but as a tool which generates header files. The header files are what makes serialization/deserialization of data possible.

# Web Portal

Darwin includes a web portal located in `/portal` which can be used to easily manage the extensions, sessions, generations, etc right from the browser. The portal also visualizes statistical information related to the population members of each generation.

You can start the portal by running `php artisan serve` and visiting the address, printed in the console, in your browser.
