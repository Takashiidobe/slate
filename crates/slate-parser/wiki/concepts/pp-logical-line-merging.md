# Preprocessor logical-line merging

`src/pp/syntax.rs::logical_lines` splits a file into one `LogicalLine` per
physical line; `GroupParser::group` turns them into `Item::Text` /
`Item::Directive` / `Item::Conditional`, and `Preprocessor::expand_line`
expands each `Item::Text` on its own. A function-like macro call whose
arguments span lines would then go unexpanded and parse as an ordinary call.

`merge_open_lines` joins consecutive non-directive lines while the net
`(`/`[` depth is positive, so `expand_line` sees the whole invocation.

## Rules

- **Never merge across a directive.** The open line is emitted unbalanced
  and the directive stays a directive; gcc-dg code selects one argument of
  an open call with `#ifdef` (`tests/fixtures/clang/linux/x86_64/c11.c`).
  The parser works on the flat token stream, so an unbalanced `Item::Text`
  is fine.
- **Trigger on depth `> 0`, not `!= 0`.** A line starting with unmatched
  closers (`);`) closes an earlier span; treating it as open cascades
  wrong merges forward.
- **Comments.** First-line comments stay leading comments of the merged
  line. Continuation-line comments are emitted as a separate entry right
  after it. `Parser::parse_decls` likewise emits comments inside a
  declaration's token range after the declaration. The `guality__*`
  gcc-dg fixtures pin these positions (`pr43329-1.c`, `pr45003-*.c`,
  `pr58791-*.c`).
