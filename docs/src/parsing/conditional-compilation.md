# Conditional Compilation

A preprocessing run selects the branches for one compiler configuration. Slate
parses the resulting C stream. Inactive branch bodies supply no AST or IR nodes.
For example, a conditional can choose one of two field declarations before sema
computes the record layout.

## Translate another configuration

Project translation can run the same inputs under several configurations and
merge the resulting translations. Each run resolves its own headers, macro
definitions, declarations, and layout. Merge compares the results after
translation. This matters when a C conditional changes part of an item, such as
a field, an argument to a call, or a typedef used by several functions.

## Explain why a token exists

The project planner can request a preprocessing trace. The trace records
conditional arms and the macros they read, along with definition and removal
events. A token's dependencies include the conditions that made it active and
the macro events that contributed to it.

```c
#ifndef NO_GZIP
#define GUNZIP
#endif

#ifdef GUNZIP
void decompress(void);
#endif
```

Under `-DNO_GZIP`, the definition of `GUNZIP` is skipped. Recording the skipped
event lets the planner connect the later test of `GUNZIP` back to `NO_GZIP`. A
trace describes one run and may report dependencies that do not change the
result. Finding the other branch's C declarations requires a second run with
that branch active.

Start with `pp/trace.rs` and `PreprocessTrace::dependencies` in
`crates/slate-parser/src/`. The trace complements the invocation records used
for [Macro Provenance](macro-provenance.md).
