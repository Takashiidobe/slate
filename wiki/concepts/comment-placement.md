# Comment placement

<!-- toc -->
- [Problem](#problem)
- [Ownership model](#ownership-model)
- [Taxonomy](#taxonomy)
  - [Owned](#owned)
  - [Detached](#detached)
  - [Dropped](#dropped)
- [Migration when an owner
  disappears](#migration-when-an-owner-disappears)
- [Doc vs regular comments](#doc-vs-regular-comments)
  - [Text normalization](#text-normalization)
  - [Crate settings](#crate-settings)
- [Declarations and definitions](#declarations-and-definitions)
- [Item order](#item-order)
<!-- /toc -->

Where a C comment ends up in generated Rust, and whether it becomes a doc
comment. Current IR shape: [pipeline](ir/pipeline.md#node-identity-and-metadata).

## Problem

Comments are currently free-floating spans (`Module.comments`,
`Statement::Comment`) re-anchored after lowering by searching for the next item
at a later source offset (`lowerer/comments.rs::module_comments`). Lowering
reorders items (hoisted types, exported functions before `static` ones), and an
origin lookup can miss, so comments drift. Observed in sqlite `backup.rs`:

| C source | Rust placement |
| --- | --- |
| File header | before `findBtree`, near the end of the file |
| Leading comment on `struct sqlite3_backup` | before `findBtree`; struct is ~2000 lines earlier |
| Trailing field comments | a loose pile before `findBtree` |
| Parameter comments on `sqlite3_backup_init` | before `isFatalError` |
| `sqlite3_backup *p; /* Value to return */` | above the `let` (trailing became leading) |
| `#endif /* SQLITE_OMIT_VACUUM */` | above `sqlite3BackupRestart` |
| Comment on a local hoisted to a `__slate_slot` | on the next unrelated statement |

Text is also rendered raw: `// /*`, `// **`.

## Ownership model

The preprocessor and parser classify every comment, since only they see
newlines and blank lines. In the AST a comment stays a sibling item tagged
`attach: Leading | Trailing | Detached`; its owner is the next sibling
(`Leading`) or the previous one (`Trailing`). Parameters have no sibling list,
so `ParameterDeclaration.comments` holds theirs. Rules:
[ast-spec](ast-spec.md#comments).

From the IR on, owned comments become fields of their node in the IR and the
Rust AST, so reordering and rewriting move them for free. Only detached
comments are placed by position, and they anchor to a node id, never to an
offset search.

## Taxonomy

### Owned

| Kind | Shape | Owners | Rust |
| --- | --- | --- | --- |
| Leading | group directly above the node, no blank line | function, prototype, record, enum, typedef, global, field, enumerator, statement | above the node |
| Trailing | same line, after the node's last token; includes multi-line continuations at the same column | field, enumerator, parameter, global, local, statement, `#define` | statement/local: trailing `//` on the same line; others: see [doc rules](#doc-vs-regular-comments) |
| Inner trailing | last comment in a block before `}`, no following statement | block | at the end of the Rust block |
| Interior | inside an expression, condition, or argument list | enclosing statement | hoisted to the statement as leading (rustfmt cannot hold them in place) |

### Detached

| Kind | Shape | Rust |
| --- | --- | --- |
| File prologue | first group in the file, before any code or `#include` | `//!` at the top of the module |
| Section, file scope | blank lines on both sides | `//` above the next emitted item from the same file, separated by a blank line; if that item is erased (prototype, typedef, include region), walk forward; fall back to the previous item; never the end of the module |
| Section, block scope | blank lines on both sides inside a body | `Stmt::Comment` sibling, kept in order |

### Dropped

| Kind | Examples |
| --- | --- |
| Preprocessor labels | `#endif /* X */`, `#else /* X */`, `#if 0` bodies |
| Tool annotations | `/* FALLTHROUGH */`, `/* NOTREACHED */`, `/* ARGSUSED */`, `/* NO_TEST */`, `NOLINT`, `/* OPTIMIZATION-IF-TRUE */` (fixed list) |

Requirement tags such as `EVIDENCE-OF:` and `IMP:` are content and stay.

## Migration when an owner disappears

When lowering or a rewrite rule deletes or merges a node, its comments go to,
in order: the replacement node, the next surviving sibling, the parent block's
inner-trailing slot. This is one rule-engine helper used by every rule
(`inline_temps`, slot hoisting, dispatcher lowering) rather than per-rule
handling. A local hoisted into a `__slate_slot` carries its comment onto the
synthesized slot `let`.

## Doc vs regular comments

Default to `//`. A comment becomes a doc comment only when its position or
marker makes that certain.

| Source | Rust |
| --- | --- |
| Explicit marker: `/** */`, `/*! */`, `///`, `//!`, `/**< */`, `///<` | doc, always |
| Leading comment on a function, record, enum, typedef, global, field, enumerator | `///` (positional promotion) |
| Trailing comment on a field, enumerator, global, `#define`→`const` | moved above as `///` (Rust has no trailing doc form) |
| Parameter comments | folded into the function doc as a `# Arguments` list (``* `pDestDb` - Database to write to``); Rust rejects docs on parameters |
| File prologue | `//!` |
| Section comments, every in-body comment, commented-out code | `//` |

Positional promotion is the default for `translate-project` and single-file
`translate` (it must not churn FileCheck fixtures, which render the parser IR,
not Rust). Statement-level
`///` would trigger `unused_doc_comments`, and a `///` with no following item
or a `//!` after an item is a hard error; the ownership model rules both out.

### Text normalization

Strip `/*`, `*/`, `**` and ` * ` gutters, dedent, drop separator-only lines
(`*****`), keep interior blank lines (`///` / `//`). Text is not fenced or
escaped for Markdown.

### Crate settings

The generated crate owns its `Cargo.toml` and `lib.rs`:

- `[lib] doctest = false`: indented C prose is an indented Markdown code block,
  which rustdoc would compile as a doctest.
- `#![allow(rustdoc::all)]`: C prose trips `broken_intra_doc_links`,
  `invalid_html_tags`, and similar.

## Declarations and definitions

A Rust item has one doc. When both a prototype (usually in a header) and the
definition carry a leading comment, the definition's wins; the prototype's is
the fallback. Typedef and tag comments for the same record merge the same way,
definition first.

## Item order

Items from a source file are emitted in source order, functions included;
Rust has no declare-before-use constraint. Section comments then land between
their original neighbours.
