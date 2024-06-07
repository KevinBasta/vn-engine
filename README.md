# visual-novel-engine


## Class variable prefixes
- `m_` denotes member variable
- `g_` denotes global variable
- `sg_` denotes static global variable


## Design patterns

- `Observer pattern` for state_subject, model subject, game observer, engine observer
- `Visitor pattern/injection` for sending down the state subject down to the model nodes
- `Iterator pattern` for iterating chapter, graph, and node
- `Command pattern` for menu commands