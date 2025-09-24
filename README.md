# visual-novel-engine

<img src="test/assets/preview.gif" alt="Early Stage Demo"/>

This repository is in progress. The above gif shows a demonstration of some of the engine's functionality.

## Running locally
- Currently external dependancies are used as submodules in the "external" directory. This means that cloning the repository and running cmake is all that is needed. In the future something like vcpkg will be used instead of submodules.
- Navigate to the "test/assets" folder and follow the readme.md instructions there to add needed assets.

## Class variable prefixes
- `m_` denotes member variable
- `g_` denotes global variable
- `sg_` denotes static global variable

## Design patterns

current:
- `Observer` for state subject, model subject, game observer, engine observer
- `Visitor/injection` for sending down the state subject to the model nodes
- `Iterator` for iterating chapters and running nodes
- `Singleton` for model subject, node strays and text rendering (may change for multithreading)
- `Builder` for constructing nodes and characters

future additions:
- `Memento` for supporting undo operations
- `Command` for menu commands
- `Visitor` for serialization