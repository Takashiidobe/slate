# Preprocessor logical-line merging

_created 2026-09-12_

`src/pp/syntax.rs::logical_lines` splits a file's raw token stream into
`LogicalLine`s, one per physical source line, which `GroupParser::group`
turns into `Item::Text`/`Item::Directive`/`Item::Conditional`. Macro
expansion (`Preprocessor::expand_line` in `src/pp/mod.rs`) then runs
separately per `Item::Text`, i.e. per logical line.

That per-line boundary matters because a function-like macro invocation's
argument list is free to span multiple physical lines in valid C (only
*directives* are required to fit on one line). Before
`merge_open_lines` existed, `expand_macros`'s `invocation_arguments` scan
for the matching `)` never crossed a line boundary, so any macro call
split across lines was silently left unexpanded -- it just fell through
as a plain, un-substituted identifier token, and later parsed (if it
still could) as an ordinary function call. This masked itself in most
cases (an ordinary function call to an "undefined" identifier still
parses fine) until an unexpanded argument wasn't valid as a call-argument
expression, e.g. a bare type-name like `long double` passed positionally
through a couple of layers of macros to `__builtin_complex` (fixed in
slate-parser-wf8.2.3).

`merge_open_lines` (`src/pp/syntax.rs`) fixes this by joining consecutive
non-directive logical lines whose net `(`/`[` depth is still positive at
line's end, so `expand_line` sees the whole invocation as one unit. Two
things it deliberately does NOT do, both load-bearing:

- **Never merge across a `#` line.** GCC-dg fixtures rely on being able to
  select one *argument* of an open call via `#ifdef`/`#else`/`#endif`
  (e.g. `printf(..., limits_total,\n#ifdef X\n bounds_total\n#else\n 0\n#endif\n);`
  in `tests/fixtures/c11.c`). If merging swallowed the directive lines as
  plain tokens, the conditional would corrupt the token stream. A line
  whose depth is still open when a directive is hit is left exactly as
  before (its own, individually-unbalanced `Item::Text`) -- this already
  worked pre-merge, because `parser.rs` reconstructs statements across
  multiple `PPNode`s regardless of how the preprocessor split them (see
  `matching_brace`/`matching_paren` callers in `parser.rs`), so nothing
  needed to change there.
- **Only merge when a line's own depth is `> 0`.** A line that *starts*
  with unmatched closing brackets (net `<= 0`) is closing a span opened on
  an earlier, already-flushed line (or by a directive-interrupted run,
  see above) -- not something that itself needs to pull in more lines.
  Treating `!= 0` as the trigger instead of `> 0` causes cascading,
  incorrect merges forward from an unrelated trailing `);`.

**Comment ordering inside a merge.** A comment on the *first* line of a
merge is kept as that merged line's leading comment (matches the
pre-existing single-line rule: `GroupParser::group` always emits a line's
comments before its `Item::Text`, regardless of whether the comment
trailed the code on that physical line). But comments on *continuation*
lines are **deferred to their own entry emitted right after** the merged
line, not bundled into the head. This matters because `parser.rs`
reconstructs a statement that spans multiple `PPNode`s by skipping over an
interleaved `Comment` node and re-emitting it once the statement
construction completes -- e.g. a trailing comment on the closing line of a
multi-line `asm volatile(...)` ends up positioned right after the
resulting `Asm` statement, not before it (see
`tests/fixtures/gcc-dg/guality__pr43329-1.c`,
`guality__pr45003-*.c`, `guality__pr58791-*.c`). Bundling a
continuation line's comment into the head (the simplest merge
implementation) reproduces the *old* per-line node layout's comment
*content* correctly but changes its *position* relative to the
reconstructed statement, which several `guality__*` FileCheck goldens
pin down precisely (including, for at least one fixture, whether a
trailing comment lands before or after a `return` and gets marked
`Unreachable`).

**Corollary for `parser.rs`'s own multi-node span helpers.** Because a
`Comment` PPNode can land *between* two `Code` PPNodes that belong to the
same declaration (see above), any helper in `parser.rs` that walks
`nodes[..]` looking for a span-terminating token must skip `Comment` nodes
rather than treat them as a stopping point. `signature_node_span` used to
`return index.max(1)` the moment it hit a non-`Code` node, which
silently truncated the span for a declaration like `_Complex float a =\n
1.if; /* comment */` (the comment sits between the `=` line and the `;`
line) and caused it to be misparsed as a function signature
(slate-parser-wf8.2.5). Fixed to `continue` past comments, matching
`declaration_node_span`'s existing behavior. `nodes_tokens` had the same
class of bug one level down: it called `node_tokens` on every node in a
span uniformly, and `node_tokens`'s fallback for a non-`Code` node
re-lexes `node_text`, which for a `Comment` node is the raw `/* ... */`
text -- producing garbage tokens. Fixed by filtering to `Code` nodes
before flat-mapping.
