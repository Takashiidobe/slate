# Preprocessing

Slate preprocesses one translation unit with the compiler arguments used to
build it. The preprocessor supplies expanded tokens, comments, and pragmas to
the parser. It keeps the source information Slate needs after parsing, such as
which header declared a function and which tokens belong to a macro argument.

The lexer scans each file, including headers. The preprocessor handles
directives and expands macros into a flat stream. The parser consumes the tokens
without lexing them again.

The preprocessor gets its flavor, target, and options from the shared
[Translation Unit Configuration](configuration.md).

## Stages

- [Headers and Include Search](headers.md) explains how Slate finds target
  headers and records their origin.
- [Macro Expansion](macro-expansion.md) follows a replacement through
  substitution and rescan.
- [Macro Provenance](macro-provenance.md) shows how Slate carries definitions,
  invocations, and argument ranges into IR.
- [Conditional Compilation](conditional-compilation.md) explains what a single
  run selects and what the project planner learns from a trace.

The supported directives are listed in
[Reference](../reference/index.md#preprocessor-directives).
