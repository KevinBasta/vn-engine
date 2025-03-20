# Semantic Conventions:
- Model is used for data which is constant across game instances
- Model Internal has two use cases
	1. Data types which are stored in the model and referenced by state (e.g. nodes, chapters, character)
	2. Data types who's base values are stored in the model and copied by state (e.g. relations)
- State is used for data unique to a particular game/save instance

# Syntactic Conventions:
- Classes start member functions with m_ to allow reuse of name
- Structs do NOT have prefixes for their elements

- Classes, Structs, and Typedefs use UpperCamelCase
- Variables and Functions use lowerCamelCase

- "id" should NOT be all caps

