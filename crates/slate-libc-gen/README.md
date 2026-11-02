# slate-libc-gen

Generate the checked-in libc catalogs used to canonicalize system declarations
as `slate_support` paths for each translation target.

Run from the workspace root after installing the target sysroots:

```sh
cargo build --release -p slate-parser
cargo run --release -p slate-libc-gen
```

- `--target <triple>`: restrict the matrix; repeat for multiple targets. The
  default covers the supported Linux, Android, macOS, FreeBSD and MSVC Windows targets.
- `--libc <dir>`: libc source to probe; defaults to `crates/slate/vendor/libc`.
- `--out <file>`: record catalog; defaults to
  `crates/slate/src/backend/clib_records_table.rs`.
- `--symbols-out <file>`: function, global, scalar typedef and integer constant
  catalog; defaults to `crates/slate/src/libc_symbols_table.rs`.
- `--report <file>`: accepted mappings and exclusions with reasons; defaults to
  `target/libc-gen-report.md`.
- `--work <dir>`: retain the probe crates and object files for inspection.

Use alternate output files for a partial matrix to avoid replacing the full
checked-in catalogs. Regenerate both catalogs after updating libc or sysroots.

Mappings use the C translation target, independently of the machine running
Slate. Identical mappings share a target group; a mismatch excludes only the
affected target. Unprobed targets retain their local declarations.

Records must match size, alignment and public field offsets. Reserved fields
are ignored unless header macros reach through an aggregate. Scalar typedefs
must match width, alignment and signedness. Function and global probes must
compile with the C declaration's type and refer to the same linker symbol.
Integer constants must match by name and value. Lowering requires system-header
macro references preserved by sema, rather than provenance on expanded literals.
The checked-in catalogs contain names, canonical destinations
and target eligibility; compatibility details stay in the generator report.

The vendored libc defaults to FreeBSD 15 and to 64-bit file offsets and time on
32-bit glibc. Header probes use `_FILE_OFFSET_BITS=64` and `_TIME_BITS=64` on
those glibc targets. Workspace builds select the same libc settings. Explicit
libc version and time-width overrides take precedence.

Translation honors each project's compiler flags. A source declaration using
another ABI must select compatible libc settings before using these mappings.

The report identifies missing bindings, unverified signature types, ABI
attributes, differing linker names, and incompatible layouts or values. Keep
local declarations for these cases; check suspected libc bugs against the
platform compiler before reporting upstream.
