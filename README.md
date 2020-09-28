# Darwin

Darwin is an open-source, extensible, and modular evolution simulator. It's primarily designed for evolution of entities based on neural networks.

# Getting Started

The extensions of Darwin usually relies on some basic components, which includes OpenNN (for neural networks), SQLite3 (for database), etc.

Please note:

- The include files for such components are located in the `/includes` directory.
- The libraries (static or dynamic) are located inside `/libraries` directory.
- Extensions are placed inside `/extensions` directory.

The first step to get started is to build the basic components, which are required by most extensions of Darwin:

## Building the Basic Components

### Core

Core holds the common and most vital components required by most extensions of Darwin.
Build it by following the instructions in the README inside `/core` directory. Place the compiled library inside `/libraries/core/` directory.

### OpenNN

OpenNN is used by Darwin for its neural networks engine. Download OpenNN's code repository from the (Official Page)[https://www.opennn.net/download/]. To build OpenNN, please follow OpenNN's [Official Guide](https://www.opennn.net/documentation/building_opennn.html). By the end of the guide, you should be having a library of OpenNN (e.g. `libopennn.a`), which you can place inside `libraries/opennn`.

### SQLite3

SQLite3 provides lightweight, server-less database support, which is required by Darwin for persistent data storage. Download the code repository from the [Official Page](https://www.sqlite.org/download.html) - make sure you download the item labeled "C source code as an amalgamation". Amalgamation is essentially combination of multiple source files into one, simplying the repository. Afterwards, follow [this guide](https://www.sqlite.org/howtocompile.html) for building SQLite3.

## Building the Extensions

Before you attempt to build extensions which require cocos2d-x, please visit the website of [Cocos Project](https://www.cocos.com) to download the CLI tools required for building a cocos2d-x project.

### Organisms

To build Organisms, run `cocos compile -p <platform>` inside the directory, `/extensions/organisms`. Replace "<platform>" with the platform of your choice; run `cocos compile --help` for list of available platforms.

### Jumpers

To build Jumpers, run `cocos compile -p <platform>` inside the directory, `/extensions/jumpers`. Replace "<platform>" with the platform of your choice; run `cocos compile --help` for list of available platforms.
