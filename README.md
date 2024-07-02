# visual-novel-engine

This repository is in progress.

## Running locally
- Currently external dependancies are used as submodules in the "external" directory. This means that cloning the repository and running cmake is all that is needed. In the future something like vcpkg will be used instead of submodules.
- Navigate to the "test/assets" folder and follow the readme.md instructions there to add needed assets.

## Class variable prefixes
- `m_` denotes member variable
- `g_` denotes global variable
- `sg_` denotes static global variable

## Design patterns

current:
- `Observer pattern` for state subject, model subject, game observer, engine observer
- `Visitor pattern/injection` for sending down the state subject to the model nodes
- `Iterator pattern` for iterating chapter, graph, and node
- `singleton pattern` for text rendering, may change for multithreading

future additions:
- `Command pattern` for menu commands
- factory/builder for constructing the game model subject