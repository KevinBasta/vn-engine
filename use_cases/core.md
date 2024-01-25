# Use Cases for core

Components:
Chatper
Character
Relation
Graph
Node


Use Case 1:

[static]  Characters are loaded into and owned by main() (from file or as static code)
[dynamic] Characters are added to a Chapter as references on chapter load

[static] Graph contains Nodes and owns all its Nodes
[static] Graph and its Nodes are loaded in a chapter (from file or as static code)
[static] Graph eventually leads to last node which has no more children signifying the end of the Chapter

[static]  A Node contains pre actions, the main event, and post actions (to be defined precisily with gui library)
[static]  A Node main event may contain multiple dialogues for the same or differnet characters or a single dialogue
[dynamic] A Node that runs out of main action steps needs to go into its post action of giving choice of what child Node to go to
[dynamic] A Node can have side effects on Characters and Characters Relation
