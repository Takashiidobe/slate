# The Rewrite Engine

Slate turns a lowered function into an arena of Rust AST nodes. Rules match node
shapes and use local facts to decide whether an edit is safe. This lets a rule
reason about the scopes, bindings, and expressions that will actually appear in
Rust.

## Apply an edit and revisit its neighbors

The engine dispatches registered rules by node kind and call anchors. When a
rule replaces a node, the engine schedules affected parents and neighbors again.
For example, removing a redundant block can expose a final return to the tail
expression rule. An edit budget bounds repeated changes.

A fact belongs to the current tree. If a rule moves or replaces a use, cached
information about that use must be refreshed before a later rule relies on it.
Use the arena edit operations so the tree, scheduling, and local facts stay in
sync.

Start with `crates/slate/src/backend/engine/` for scheduling and the rule
registry, and `engine/rules/` for the individual matches.
