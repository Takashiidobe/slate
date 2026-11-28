# AST Reference

Slate's abstract syntax tree represents one preprocessed C translation unit. It
preserves declarator nesting, statement and expression structure, and source
context. Semantic analysis resolves names, types, conversions, and layout when
it builds the typed IR.

## Print the AST

```sh
slate-parser parse input.c
slate-parser parse input.c --sexpr
```

Use the original flavor, standard, target, defines, and include paths to inspect
a particular build configuration. Only the preprocessor branches active in that
configuration reach the tree.

| Option            | What it shows                                                                                                                                          |
| ----------------- | ------------------------------------------------------------------------------------------------------------------------------------------------------ |
| Default           | A nested dump with `tag[N]` entries by tag ID followed by `decl[N]` entries in source order. The command runs semantic checks before printing the AST. |
| `--parse-only`    | The same AST output with semantic analysis skipped. Use it to inspect syntax when semantic analysis reports an error.                                  |
| `--sexpr`         | The tree as nested lists. Top-level nodes from system headers are omitted and counted.                                                                 |
| `--show-ids`      | Node IDs printed as `#N`. These identify syntax nodes within the parse.                                                                                |
| `--show-comments` | Comment nodes and comment attachments on parameters.                                                                                                   |

The default dump prints a variant such as `Return(...)` or a named node such as
`Binary { op: Add, … }`. In S-expression form, the same binary expression has
the shape `(Binary (op (Add)) (left …) (right …))`.

## Translation unit

The `TranslationUnit` carries declarations, tag definitions, macro records, and
the dialect for the run. The declaration list includes function definitions,
object and function declarations, static assertions, pragmas, file-scope
assembly, standalone attributes, and comments.

Tag definitions have a `TagId` and live in the tag table. A specifier that
defines a `struct`, `union`, or `enum` refers to that ID. A named tag reference
keeps its written name for semantic analysis to resolve. Two declarators in one
declaration, such as `struct { int x; } a, b;`, share the definition introduced
by the specifier. Field widths and enumerator initializers remain expressions.

## Declarations and declarators

A `Declaration` has a common set of specifiers and a list of declarators. For
`int a, *b = &a;`, the base type is `int` for both entries. Each entry keeps its
own declarator and optional initializer. A tag-only declaration can have an
empty list. A `typedef` uses the same declaration shape with `Typedef` storage.

A declarator records how a name or abstract declarator is nested inside
pointers, arrays, function parameter lists, attributes, and parentheses. The
difference between these two declarations comes from the nesting.

| C declaration     | Declarator shape, abbreviated                                   | Resulting C type                       |
| ----------------- | --------------------------------------------------------------- | -------------------------------------- |
| `int *f(int);`    | `Function { inner: Pointer { inner: Name("f") }, … }`           | Function returning a pointer to `int`. |
| `int (*fp)(int);` | `Function { inner: Grouped(Pointer { inner: Name("fp") }), … }` | Pointer to a function returning `int`. |

The tree also distinguishes `(void)` as `Void`, `()` as `Empty`, typed parameter
lists as `Prototype`, and old-style identifier lists as `IdentifierList`.
Semantic analysis interprets these forms under the selected standard. A function
definition adds a body to its specifiers and declarator.

## Statements, expressions, and initializers

- **Statements** retain source control flow. `For` keeps separate fields for the
  initializer, condition, increment, and body. `Block` records braces, and
  labels and jumps retain the names written in C.
- **Expressions** retain source grouping and operations. `Identifier` carries a
  name, `Paren` wraps a grouped expression, and `Cast` holds the written type
  name. Constructs such as `sizeof`, `_Generic`, and `__builtin_offsetof` have
  their own syntax forms. Their results are determined during semantic analysis.
- **Initializers** are expressions or braced lists. Each list item keeps its
  designators, such as `.field`, `[index]`, or a GNU `[first ... last]` range.
  Semantic analysis selects the subobjects and applies conversions.

For example, save this function as `input.c` and run
`slate-parser parse input.c --sexpr`.

```c
int increment(int x) { return (x + 1); }
```

The return subtree contains this expression. The literal's metadata is
abbreviated.

```text
(Return
  (Paren
    (Binary
      (op (Add))
      (left (Identifier "x"))
      (right (IntegerLiteral …)))))
```

Here `x` is still a spelled identifier and the parentheses have their own node.
The IR resolves `x` to a parameter binding, adds a read and a typed addition,
and expresses the grouping through the nested operations.

## Source locations and annotations

A spanned node has its own `NodeId` and can carry distinct spelling and
expansion locations. Macro expansions can give several nodes the same source
range. Use node IDs to distinguish them in the dump. The default dump can show
system header provenance. The raw spelling and expansion locations are carried
by the AST.

Comments can be leading, trailing, or detached. Attributes retain their
placement on declarations, declarators, tags, or statements. Pragmas are kept in
source order so semantic analysis can apply their effects to the constructs that
follow. The tree represents expanded tokens, and the translation unit retains
macro invocation records for later stages.

For the full node model and dump syntax, see the
[AST spec](https://github.com/Takashiidobe/slate/blob/main/wiki/concepts/ast-spec.md)
and [Printed AST Grammar](ast-grammar.md).
For the resolved representation, see [IR Reference](ir.md).
