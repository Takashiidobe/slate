# slate-libc-gen

Workspace tooling for the checked-in
[`clib_records_table.rs`](../slate/src/backend/clib_records_table.rs), the
`CLIB_RECORD_TYPES` table that maps system-header records to
`slate_support` (the vendored libc) paths.

Run from the workspace root, after building `slate-parser` and installing the
sysroots for every target:

```bash
cargo build --release -p slate-parser
cargo run -p slate-libc-gen
```

- Defaults: `--parser target/test-cache/release/slate-parser`,
  `--libc crates/slate/vendor/libc`, `--out` the checked-in table,
  `--report target/libc-gen-report.md`. Repeat `--target` to restrict the
  matrix; the default is Linux (glibc x86_64/i686/aarch64/armv7, musl
  x86_64/aarch64), Apple, FreeBSD and Android on their x86_64 and aarch64
  triples. `--work <dir>` keeps the probe crates for inspection.
- C side: `slate-parser ir -include <header>` per target and header keeps every
  record in that header. Size, alignment and field offsets come from the IR;
  typedef names are resolved to their record, so macOS `pthread_attr_t` and its
  `_opaque_pthread_attr_t` record both map.
- Rust side: a probe crate per target takes `size_of`, `align_of` and
  `offset_of!` of each `libc::<name>` as `u32` statics, builds with
  `--emit=obj`, and reads the values from the object file. Names or fields libc
  lacks or hides are removed from the probe and treated as missing.
- A name enters the table only if, on every target where C defines it, libc
  defines it with the same size and alignment, and every non-reserved field
  matches by name and offset. Reserved (`_`-prefixed) fields are ignored unless
  they are aggregates reached through header macros (`sigaction`, `siginfo_t`).
  Types with no common non-reserved field are opaque.
- The report lists every exclusion with the target and reason. Layout and field
  disagreements between a header and libc are candidates for upstream libc
  reports; check them against a native compiler first.
