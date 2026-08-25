# Predicate tokenizer folds unknown tokens into `Opaque` leaves, not the whole expression

`src/frontend/preprocess.rs`'s `#if`/`#elif` predicate tokenizer used to
return `None` for the whole predicate the moment it hit any unrecognized
token (numeric literals, `>=`, `==`, etc.) — collapsing the entire expression
to one `PredExpr::Opaque` and losing the `&&`/`||` tree. That meant `eval()`
could never short-circuit even when a leading `defined(X)` was `false`.

`parse_opaque_atom` now folds unrecognized runs into `Opaque` **leaves**
within the `And`/`Or` tree instead, so a predicate like

```c
defined(_MSC_VER) && _MSC_VER >= 1700 && defined(_M_ARM)
```

still evaluates via short-circuit once `defined(_MSC_VER)` is `false`,
instead of the whole line becoming unconditionally opaque.

This is a general preprocessor-correctness fix, not specific to any one
codebase — version-guard idioms of exactly this shape (`defined(X) && X >=
N && ...`) are common in real C. Found via a `#pragma optimize` guard at
`sqlite3.c:55623` during the `slate-wcf7` sqlite translation epic. Fixture:
`tests/fixtures/pragma_guarded_by_opaque_version_predicate.c`.
