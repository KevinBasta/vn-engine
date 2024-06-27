# visual-novel-engine

This repository is in progress.

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