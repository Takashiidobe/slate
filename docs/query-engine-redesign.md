# Item-Centric Query Engine

## Goal

Every rewrite should use the same pipeline:

```text
iterate Rust AST entities
    -> match structural and semantic fields
    -> retain stable semantic handles
    -> query facts and relationships
    -> produce anchored edits
    -> reject conflicting edits
    -> apply every surviving match
```

The engine must support declarations, functions, parameters, locals,
statements, expressions, and whole-program relationships without adding a new
rule engine or a pass-specific method to `QueryContext` for each rewrite.

## Boundaries

The engine has four independent layers:

1. Traversal enumerates queryable entities from one immutable `Program`.
2. Matchers select entities and bind structural captures.
3. Fact queries resolve semantic identities and relationships.
4. Recipes return typed edits against stable anchors.

A rule chooses one component from each layer. Candidate kind must not determine
which facts the rule can inspect or which other anchored entities it can edit.

## Queryable entities

Use one shared entity vocabulary rather than separate call, definition, value,
statement-window, and program candidate systems:

```rust
enum QueryItem<'program> {
    Definition(DefinitionRef<'program>),
    Function(FunctionRef<'program>),
    Binding(BindingRef<'program>),
    Statement(StmtRef<'program>),
    Expression(ExprRef<'program>),
}
```

Each reference contains a stable snapshot anchor. Semantic entities also carry
their fact identity, such as `FunctionId` or `BindingId`. A function reference
exposes its signature, parameters, return type, and body through the same
snapshot.

`AstPath` remains the location identity within a function. Top-level items need
an equivalent item anchor that survives planning and is validated before apply.

## Matchers

Matchers follow the `Field<T, Cx>` composition in `patterns.rs`, but implement a
common interface and return typed captures:

```rust
trait Matcher {
    type Capture;

    fn matches(
        &self,
        query: &QueryContext<'_>,
        item: &QueryItem<'_>,
    ) -> Option<Self::Capture>;
}
```

Matchers may compose other matchers. Examples include a function matcher with
parameter and body matchers, a local matcher with a type and usage matcher, and
a return matcher whose value is captured as an expression.

Structural matching does not perform mutation. Semantic matcher fields use the
same generic fact API available to rule cases rather than calling pass-specific
planners.

## Semantic handles and facts

Fact access starts from handles, not names:

```rust
query.function(function_ref)
query.binding(binding_ref)
query.def_use(binding_ref)
query.type_of(binding_ref)
query.effects(expr_ref)
query.value(expr_ref)
query.place(expr_ref)
query.call(expr_ref)
query.parent(expr_ref)
```

`def_use` returns anchored reads and writes. A rule can follow those sites back
into the AST, inspect their parents, and continue querying facts from the new
entities. Missing identity or an incomplete domain rejects the match; it never
defaults to zero uses.

Specialized analyses remain facts, but `QueryContext` only adapts them to stable
handles and evidence. Candidate search, pass policy, and AST construction do not
belong in context.

## Edits

A successful case returns an `EditSet` containing any number of typed anchored
edits:

```rust
enum Edit {
    ReplaceExpr(ExprRef, Expr),
    ReplaceStmt(StmtRef, Vec<IndentStmt>),
    ReplaceFunction(FunctionRef, FnDef),
    ReplaceDefinition(DefinitionRef, Item),
    Delete(Anchor),
}
```

Selection and edit scope are independent. A matched parameter may update its
function signature and every call site. A matched local may replace its sole use
and delete its declaration. Existing overlap detection, immutable-snapshot
planning, evidence, tracing, and touched-item reporting remain.

## Final return temporary example

`final_return_temps` should match an immutable local with one read and zero
writes. It follows the binding's def-use fact to the read, proves that the read
is the direct value of the immediately following return, then replaces the
return and deletes the local declaration.

The local name is irrelevant. `_vN` is not proof that a binding is temporary.
The binding identity, def-use cardinality, direct-return shape, and adjacency
are sufficient and also work for non-generated local names.

## Migration

1. Introduce reusable binding handles and generic def-use queries. Migrate
   `final_return_temps` and remove its `_vN` check.
2. Generalize traversal into `QueryItem` enumeration and make the existing
   pattern structs implement a common matcher interface.
3. Replace rule-specific case contexts with one context over typed captures and
   semantic handles.
4. Generalize edits into `EditSet`, retaining the current planner's conflict
   checks and touched-item accounting.
5. Migrate local and parameter rewrites, starting with `dead_locals`,
   `inline_temps`, `zero_init`, and `unused_params`.
6. Migrate function and definition rewrites, then remove pass-specific planners
   from `context.rs`.
7. Move whole-program symbol and type dependency discovery into reusable facts
   and migrate `unused_items`.

Each migration should delete an old selector, context method, or edit engine.
Adding a compatibility layer without removing the replaced path does not count
as progress.

## Invariants

- Match and fact evaluation use one immutable program and fact snapshot.
- Every semantic relationship is keyed by IDs and paths, never rendered text.
- Missing or ambiguous facts reject conservatively.
- Rules expose policy as ordered cases.
- Recipes own AST construction and mutation descriptions.
- Every matching entity is proposed; conflict resolution, not traversal order,
  decides which edits survive.
