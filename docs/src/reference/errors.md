# Error Handling

Slate keeps errors typed while they travel between stages. The stage that
detects the failure supplies the error kind and the most specific source
location it knows. A caller adds context such as the file or operation it was
handling. The reporting boundary presents the result to the CLI or another
consumer.

## Error types by stage

| Stage                           | Types                                     | When they occur                                                                                                                                                                                      |
| ------------------------------- | ----------------------------------------- | ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| Compiler configuration          | `CompilerArgError`, `RuleError`           | An argument or combination of options fails validation. Rule combinators can carry the individual failures that explain why a combination failed.                                                    |
| Preprocessing                   | `PPErrorKind`, `PPError`                  | A required header is missing, a macro paste is invalid, or a directive is malformed. The preprocessor adds available source text and a label to the failure.                                         |
| Preprocessor directives         | `DirectiveDiagnostic`, `DirectiveErrors`  | An active `#warning` or `#error`, or a warning such as macro redefinition. The diagnostic carries severity, and a group can report several errors.                                                   |
| Parsing                         | `ParseError`                              | A token sequence cannot form a node in the selected C dialect. The error carries a message and a source label.                                                                                       |
| Semantic analysis and AST to IR | `ResolveError`, `SemaError`, `SemaErrors` | A name, type, operand, or construct fails a semantic rule, or Slate cannot model it. The sema driver attaches source context to individual diagnostics and groups failures for the translation unit. |
| IR to Rust                      | `Barrier`, `InvalidIr`                    | A barrier means the lowerer has no Rust translation for a construct. Invalid IR means an invariant was broken, such as an assembly operand referring to an index outside the operand list.           |

The parser crate's `FrontendError` can wrap preprocessing, parsing, and sema
errors. The translator's separate `slate::frontend::Error` adds stage and file
context. For example, `parse path.c` can wrap a preprocessor error. Read the
cause and its label to find the stage that detected the failure.

## Classify semantic and lowering failures

- Use `ResolveError::Rejected` or a specific variant such as `InvalidOperands`
  for invalid C. Report it in the checker or in a rule shared with the checker.
  A `break` outside a loop or switch is a source error the checker can identify
  before IR emission.
- Use `ResolveError::Unimplemented` for a valid construct that the parser cannot
  model yet. For example, `__declspec(code_seg(".text$custom"))` can produce
  `not implemented: code segment attribute` during AST to IR lowering. The Rust
  lowerer never sees that construct.
- Use `ResolveError::Internal` when AST to IR lowering cannot find a fact the
  checker should have supplied, such as a recorded conversion. Fix the checker
  or the handoff that lost the fact.
- Return a `Barrier` through the Rust lowering path for a construct with valid
  IR that Slate cannot translate. For example, the parser can record an AArch64
  SME streaming function even though Rust lowering reports a barrier for its
  function state.
- Return `InvalidIr` through the Rust lowering path for a broken invariant.
  Trace the invalid node back to the IR producer or transform that created it.

The Rust lowerer collects barriers, and strict translation reports the first.
Use `slate lowering-barriers` to list the collected barriers. Invalid IR aborts
that lowering run. See
[Diagnostics and the Checker Contract](../parsing/sema/checker-contract.md) and
[Lowering](../lowering.md).

## Propagate errors and add context

Return a typed `Result` from a helper that can fail. Use `?` when the caller
cannot recover and the existing error or its `From` conversion gives enough
context. Use `map_err` at a boundary that knows something new, such as which
file failed to parse. Keep the original error in a source field so callers still
have the cause.

```rust,ignore
let parsed = parser.parse_file(path).map_err(|source| Error::Parse {
    path: path.to_path_buf(),
    source: Box::new(source),
})?;
```

`Error::Parse` in `crates/slate/src/frontend.rs` marks the inner error with
`#[source]` and uses `#[diagnostic(forward(source))]` to preserve the underlying
diagnostic metadata while adding the file and operation to the message.

Attach a source location where the failing construct is known.
`ResolveError::at(loc)` keeps an existing location, so callers can supply a
fallback without replacing a precise inner span. In Rust lowering, keep the
primary failure site and add an enclosing return or use site as secondary
context when it helps explain the failure.

Handle an error locally when the stage can recover or collect independent
diagnostics. For instance, the sema driver can abandon a failed declaration and
inspect the next one. It suppresses some follow-on errors caused by failed
bindings and returns the collected diagnostics without emitting an IR module.
Propagate immediately when the caller needs a valid result to proceed.

Preprocessor directive diagnostics can arrive separately from the parse result.
The `slate-parser` CLI checks them before unwrapping that result so earlier
warnings are still reported if parsing fails later.

## Define and render diagnostics

- **`thiserror::Error`** defines the typed Rust error and its display message.
  Use `#[from]` for a direct conversion that needs no new fields or `#[source]`
  when a wrapper also carries context.
- **`miette::Diagnostic`** describes presentation data such as severity, warning
  code, source text, labels, help, and related diagnostics. Use a named source
  with byte spans for labeled snippets. Use `#[related]` for independent
  diagnostics in a group such as `SemaErrors`.

Return diagnostics to the caller so it can choose the output format.
`slate-parser` renders reports with miette at its CLI boundary. `slate` renders
parser and sema reports through its diagnostics adapter and attaches source
labels to Rust lowering failures where source information is available. The
translator can also collect structured diagnostics with severity, messages, and
labels for a caller that needs JSON.

Warning severity belongs to the selected dialect's diagnostic options. Keep a
warning's identity so options such as `-Werror=NAME` can promote it. The
`#warning` and `#error` directives carry their own severity. See the
[diagnostic severity rules](https://github.com/Takashiidobe/slate/blob/main/wiki/concepts/diagnostic-severity.md).

For the derive attributes and reporting API, see the
[thiserror documentation](https://docs.rs/thiserror/latest/thiserror/) and
[miette documentation](https://docs.rs/miette/latest/miette/).
