gcc.dg cases that are not collected by any suite because Rust cannot reproduce
what they test.

- `ssp-1`: expects the oracle's stack protector canary to call
  `__stack_chk_fail`. Rust would need `-Zstack-protector` and unchecked array
  stores, but its bounds check panics on the out-of-bounds write first.
- `torture__pr68264`: expects `sqrt` of a negative number to set `errno`. The
  Rust link resolves `sqrt` to compiler_builtins' errno-less copy before it
  reaches glibc's libm.
- `c2y-if-decls-6`: the clang oracle (23.1) rejects C2y selection-statement
  declarations. `fixtures/c2y_if_declarations.c` covers the lowering with a
  recorded `.expected`.
- `c23-empty-init-5`: reads the padding bytes of `{}`-initialized structs and
  expects zeros. Padding of a by-value Rust aggregate is undef, so LLVM treats
  the read as UB (`ud2` at `-C opt-level=1`) and the result varies by build
  profile.
