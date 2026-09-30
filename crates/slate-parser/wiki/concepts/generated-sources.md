# Generated sources

<!-- toc -->
- [Builtins](#builtins)
- [Attributes](#attributes)
- [Predefines](#predefines)
- [Related, not generated here](#related-not-generated-here)
<!-- /toc -->

Checked-in files produced by a tool or captured from a compiler. Regenerate
instead of editing by hand.

| File | Generator | Source of truth | Rerun when |
| --- | --- | --- | --- |
| `src/sema/clang_builtins.rs` | `tools/generate_clang_builtins.py --clang-tblgen <bin> --llvm-project <dir> --output src/sema/clang_builtins.rs` | clang's `Builtins.td` via `clang-tblgen --dump-json` | upgrading the clang oracle |
| `src/attribute_support/registered.rs` | `tools/generate_attribute_lists.py` | installed clang (per `CLANG_TRIPLES`) and gcc drivers, probed with `__has_attribute` / `__has_c_attribute` | upgrading clang or gcc, adding a probed triple |
| `src/pp/has_checks/clang.rs` | `tools/generate_clang_pp_tables.py` | installed clang, probed with `__has_warning` over `diagtool tree` groups and `__is_identifier` over libclang-cpp identifiers in each C mode | upgrading clang |
| `src/predefines/<compiler>-<version>_<triple>.h` | `-dM -E` capture ([adding-a-target](adding-a-target.md#steps)) | clang, gcc, `tools/cl.exe` | adding a target, upgrading a compiler |
| `tests/fixtures/clang/linux/x86_64/parser_scope_combinations.c` | `tools/generate_scope_fixtures.py` | the script | changing the script; keeps existing `CHECK` lines |

## Builtins

- `include!`d into `src/sema/builtins.rs`: one `BuiltinPrototype` per
  clang builtin signature. Hand-written entries and custom lowering live in
  `builtins.rs` ([ir/builtins](ir/builtins.md)).

## Attributes

- `registered.rs` is the union across probed targets: a name counts if any
  target registers it. The hand-maintained `Gate` table in
  `src/attribute_support.rs` refines modeled attributes per target and
  flavor ([attributes](attributes.md#registration)).

## Predefines

- Snapshots are `-dM` output with configuration-dependent macros deleted
  (ISA, FPU, float ABI, CPU, GNU namespace). Those and the
  standard-dependent macros are computed in `Preprocessor::configure`
  ([preprocessor](preprocessor.md#predefines)).
- `slate_*.h` files (`__SLATE_*` target facts, GNU-namespace macros) are
  hand-written.
- Check a snapshot with `tools/gcc_macro_diff.py '<cc> <flags>'
  '<slate-parser args>'`; only `__SLATE_*` may differ.
- `../slate-predefines` holds CI workflows (apple-clang, msvc-2022,
  msvc-2026) that run compilers and publish their `-dM` output. A sanity
  check for compilers not installed locally; local capture is the default.

## Related, not generated here

- `src/pp/has_checks.rs`: seeded from clang tablegen, now extended by hand
  because gcc, clang, and msvc answer differently.
- Include trees: `sysroot.rs` / `compiler_headers.rs` point at headers
  installed by `../slate-sysroots`.
- FileCheck `CHECK` lines: generated per fixture by `tools/update_filecheck.py`
  ([fixture-layout](fixture-layout.md)).
