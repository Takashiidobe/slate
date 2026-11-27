# Macros and Comments

The engine sees Rust AST nodes carrying source context. A rewrite that moves an
expression or removes a statement must account for macro argument holes and
comments attached to the old shape.

## Treat macro arguments as opaque

During rewriting, a recovered macro's argument appears as a hole. A rule cannot
inspect the Rust expression that will later fill it. For example, combining two
syntactically equal macro argument uses into one evaluation could remove a call
the original expansion made twice. The hole remains opaque until the macro
template is finished. See [Macro Provenance](../parsing/macro-provenance.md).

## Retire a node with its comments

Use `Arena::retire` for a removed leaf and `Arena::retire_subtree` for a removed
container. Use `release_comments` when a rule splices the node away itself. The
arena can attach the comments to the replacement or surviving sibling. If no
sibling remains, comments can stay detached at the end of the block.

For example, if a rule replaces a statement that has a leading comment, the
comment should follow the replacement. Removing the node from the statement list
alone would also remove the comment's anchor.
