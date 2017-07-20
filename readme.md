Labyfou
=========================================

A Simple labyrinth generator made with SFML

Install
-------

First SFML needs to be built.

```sh
cd 3rd-party/SFML-2.4.2
mkdir _build
cd _build
cmake .. -DCMAKE_BUILD_TYPE=Release
make install
```

This install SFML locally in the project.
Then build the project itself.

```sh
mkdir _build
cd _build
cmake .. -DCMAKE_BUILD_TYPE=Release
make install
```

On windows, copy the files in SFML\bin into bin.
The game is located in the bin folder. Just run it.

Authors
-------

Vincent SAMY