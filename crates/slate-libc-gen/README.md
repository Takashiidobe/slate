# slate-libc-gen

Generate the checked-in binding catalogs used to canonicalize system
declarations as `slate_support` paths for each translation target.

Run from the workspace root after installing the target sysroots:

```sh
cargo build --release -p slate-parser
cargo run --release -p slate-libc-gen
```

- `--target <triple>`: restrict the matrix; repeat for multiple targets. The
  default covers the supported Linux, Android, macOS, FreeBSD and MSVC Windows
  targets.
- `--libc <dir>`: libc source to probe; defaults to `crates/slate/vendor/libc`.
- `--out <file>`: record catalog; defaults to
  `crates/slate/src/backend/clib_records_table.rs`.
- `--symbols-out <file>`: function, global, scalar typedef and integer constant
  catalog; defaults to `crates/slate/src/libc_symbols_table.rs`.
- `--support-manifest <file>`: support manifest with generated Windows feature
  requirements; defaults to
  `crates/slate/vendor/slate-support/Cargo.toml.template`.
- `--report <file>`: accepted mappings and exclusions with reasons; defaults to
  `target/libc-gen-report.md`.
- `--work <dir>`: retain the probe crates and object files for inspection.

Use alternate output files for a partial matrix to avoid replacing the full
checked-in catalogs. Regenerate both catalogs after updating libc or sysroots.

Apple targets also discover public bindings in the vendored `mach2` through
nightly rustdoc JSON. Module documentation supplies C header candidates;
definitions supply `slate_support::mach` paths. Ambiguous names stay local.
Discovery does not bypass the signature, symbol, layout or value checks.

Windows targets discover public `windows-sys` userspace bindings through the
same rustdoc inventory and probe `windows.h` alongside the C library headers.
The support manifest enables only the feature modules required by verified
bindings and their referenced types. Use an alternate `--support-manifest`
alongside alternate catalogs when probing a partial target matrix.

Mappings use the C translation target, independently of the machine running
Slate. Identical mappings share a target group; a mismatch excludes only the
affected target. Unprobed targets retain their local declarations.

Records must match size, alignment and public field offsets. Reserved fields are
ignored unless header macros reach through an aggregate. Scalar typedefs must
match width, alignment and signedness. Function and global probes must compile
with the C declaration's type and refer to the same linker symbol. Integer
constants must match by name and value. Lowering requires system-header macro
references preserved by sema, rather than provenance on expanded literals. The
checked-in catalogs contain names, canonical destinations and target
eligibility; compatibility details stay in the generator report.

The vendored libc defaults to FreeBSD 15 and to 64-bit file offsets and time on
32-bit glibc. Header probes use each target's default C ABI, so on 32-bit glibc
`off_t` is 32 bits. `slate_support` therefore declares `off_t` and the functions
taking it against the plain 32-bit symbols; bindings that depend on 64-bit
`time_t` stay out of the 32-bit glibc catalog. Workspace builds select the same
libc settings. Explicit libc version and time-width overrides take precedence.

Translation honors each project's compiler flags. A source declaration using
another ABI must select compatible libc settings before using these mappings.

The report identifies missing bindings, unverified signature types, ABI
attributes, differing linker names, and incompatible layouts or values. Keep
local declarations for these cases; check suspected libc bugs against the
platform compiler before reporting upstream.
