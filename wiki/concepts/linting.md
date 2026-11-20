# Linting

<!-- toc -->
- [Suppressions](#suppressions)
- [Parser paths do not panic](#parser-paths-do-not-panic)
- [Custom lints](#custom-lints)
  - [`slate::sensitive_wildcard`](#slatesensitive_wildcard)
  - [`slate::sensitive_field_rest`](#slatesensitive_field_rest)
- [Adding a lint](#adding-a-lint)
<!-- /toc -->

The lints that `slate` and `slate-parser` must pass, the project's own
`slate::` lints, and when a suppression is acceptable. Setup and commands for
the custom lints are in
[`crates/slate-lints/README.md`](../../crates/slate-lints/README.md).

## Suppressions

- `#[allow]` is denied in both crates (`allow_attributes`). Suppress with
  `#[expect(lint, reason = "...")]`: an expect fails once the code it covered
  is gone, so stale suppressions do not pile up.
- Every expect needs a `reason` (`allow_attributes_without_reason`). The reason
  is the rationale for the exception, written for someone deciding whether the
  exception still holds.
- Put the expect on the smallest item the lint allows: an arm or pattern for
  the `slate::` lints, the function for clippy's `wildcard_enum_match_arm`
  (clippy reports at the `match`, so an arm-level expect is never fulfilled).

## Parser paths do not panic

`slate-parser`'s `lib.rs` and `main.rs` deny `unwrap_used`, `expect_used`,
`panic`, `unreachable`, `todo`, `unimplemented`, and the `assert!` family
(`disallowed_macros`, listed in `crates/slate-parser/clippy.toml`). Failures go
through `Result`. The denies are crate-root attributes, so tests and `build.rs`
are exempt.

Do not suppress these. Restructure so the failure case cannot happen (a
non-empty type instead of `last().unwrap()`, a `let ... else` returning
`ResolveError::Internal`).

## Custom lints

`crates/slate-lints` is a dylint library. `slate-parser` and `slate` register
the `slate` tool (`#![register_tool(slate)]`), so the marker attributes and
`#[expect(slate::...)]` compile in ordinary builds and are checked only under
`cargo dylint`.

### `slate::sensitive_wildcard`

Marked enums: `#[slate::sensitive]` on the enum definition. Mark an enum when a
new variant needs a decision at most of its match sites (a type family, a value
kind, a compiler flavor).

Fires on a `_` or plain binding in a `match` or `matches!` over a marked enum
when the arms name two or more variants at that position, including tuple
positions (`(Kind::Int, _)`) and nested patterns. That match is a classifier,
and its catch-all silently assigns every future variant to one side.

Does not fire on:

- A projection: the arms name exactly one variant (`Pointer(p) => Some(p),
  _ => None`, `matches!(k, Array { .. })`). This is `if let`, which the lint
  does not check either.
- An arm returning `Err(..)` or `return Err(..)`, or diverging
  (`unreachable!`, `panic!`): a new variant there fails loudly.
- Code from derives and external macros.

The lint is an audit: each hit is a place where an assumption about the variant
set was implicit. Resolve it so the assumption is written down.

1. List the variants when a new variant there needs a decision. If the list is
   unseemly, and especially if the same set recurs at several sites, change the
   representation instead: an audited view on the owning type that lists the
   enum once and returns a small enum the call sites can list cheaply
   (`CTypeKind::integer() -> Option<IntegerKind>`, `complex_domain()`,
   `sugared()`), or a call to an existing helper that already makes the
   decision (`CTypes::adjust_parameter`).
2. Return an error from the catch-all if a new variant reaching it is a bug.
3. When the wildcard is right, `#[expect(slate::sensitive_wildcard, reason =
   "...")]` on the arm. The reason is documentation: it names the property the
   listed arms cover ("every kind with a child type is listed") so the author
   of a new variant can check it. A new variant that breaks the stated
   property means the site needs changing.

While auditing, look for siblings that should travel together. Missing a
sibling variant (`BlockPointer` next to `Pointer`) is the usual bug the lint
uncovers.

### `slate::sensitive_field_rest`

Fires on a struct or tuple-struct pattern whose `..` skips a field marked
`#[slate::sensitive]`. Mark a field when code that destructures the struct
usually has to act on it (a flag that changes layout or ABI, like SVE
`scalable`).

Resolve by binding the field, or by writing `field: _` to record that it was
considered. Opting out with an expect is acceptable only for code that is
correct for every value of the field (printing, hashing).

## Adding a lint

Add it to `crates/slate-lints/src`, register it in `register_lints`, and add
cases to `ui/main.rs` (including ones that must not fire). Declare it with
`declare_tool_lint!` under the `slate` tool so expects use `slate::name`. Use
plain `rustc_private` APIs, not `clippy_utils`, which pins its own nightly.
Then document it here.
