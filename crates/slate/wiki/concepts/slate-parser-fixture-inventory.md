# Slate frontend fixture inventory

The [fixture inventory](slate-parser-fixture-inventory.tsv) records all 444 top-level
`tests/fixtures/*.c` files on the host target. It was generated on 2026-09-25
from Slate `38a847e5` and slate-parser `e5fc31f1` with:

```bash
cargo build --release
python3 tools/inventory_slate_frontend.py
```

The tool uses `target/test-cache/release/slate`, the configured release binary.
It invokes `translate-lowered --frontend=slate` for each fixture and inspects
`emit-slate-ir` when the lowerer stops at its module type gate. The TSV is a
snapshot; rerun the command after either lowerer or parser changes.

| Stage | Fixtures |
| --- | ---: |
| Parser or semantic analysis failure | 13 |
| Unsupported Slate lowering | 348 |
| Rust emitted, differential behavior unverified by this inventory | 83 |

| Scope owner | Fixtures |
| --- | ---: |
| Phase 3 common semantics | 113 |
| Phase 4 difficult semantics and source facts | 276 |
| Phase 5 rewrite and full parity | 55 |

Only `add.c`, `loop_sum.c`, and `pointers.c` currently carry both Slate FileCheck
prefixes. The inventory does not compile the emitted Rust or compare it with C.
`lowered-unverified` and `slate_checks=yes` are never passing statuses.

`first_barrier` is the earliest observable parser or lowering stop, and
`first_ticket` is the ticket for that stop. `scope_owner` routes each fixture
to the Phase 3 common fixture gate (`slate-p58o.3.24`), Phase 4 difficult
semantics (`slate-p58o.4`), or Phase 5 rewrite/full parity gate (`slate-p58o.5`).
The scope route uses fixture names, with explicit exceptions in the inventory
tool; it is triage, not a claim that the later phase already passes.

The current lowerer rejects every nonempty `Module.types` before visiting
functions. This masks deeper gaps in 202 fixtures: 130 first reach an alias,
47 a record, 20 an enum, and 5 a union. Recompute the report after those
module-level tickets land to expose the next operation. Parser failures remain
separate from lowerer failures, and no skipped fixture is counted as passing.

## Parser and sysroot failures

The 13 initial parser/sema failures have specific first-barrier tickets in the
TSV. The current GNU sysroot from `slate-sysroots` pins glibc 2.36-8; its
headers lack `stdbit.h`, `free_sized`, and `memset_explicit`. Its `complex.h`
gates `CMPLX` and `CMPLXL` behind a GNU compiler version test that the
parser's Clang-like predefines do not satisfy. The migrated path must stop
defaulting to `libc-shim`.

| First barrier | Fixtures | Ticket |
| --- | --- | --- |
| Missing C23 libc APIs and headers | `c23_library.c`, `c23_stdlib_memory_management.c` | `slate-p58o.1.6` |
| `complex.h` feature macro gate | `c11.c`, `long_double_complex.c` | `slate-p58o.1.7` |
| `auto` and `__auto_type` inference | `c23_language.c`, `gnu_auto_type.c`, `statement_expr_auto_type.c` | `slate-parser-dyd.38` |
| `nullptr_t` | `c23_nullptr.c` | `slate-parser-dyd.51` |
| GNU function pointer arithmetic | `gnu_function_pointer_arithmetic.c` | `slate-parser-dyd.52` |
| GNU flexible array in union | `gnu_init_designator_survey.c` | `slate-parser-dyd.53` |
| Attributed module statement | `gnu_misc_extension_survey.c` | `slate-parser-dyd.26` |
| GNU `sizeof(function)` | `misc_language_survey.c` | `slate-parser-dyd.54` |
| Transparent union call conversion | `transparent_union_call.c` | `slate-parser-dyd.34` |

`c23_library.c` reaches parser `char8_t` lowering (`slate-parser-dyd.50`)
after its missing headers are supplied. `c23_language.c` also uses
`nullptr_t` (`slate-parser-dyd.51`) after auto inference. The sysroot and
independent C oracle need aligned target headers and runtime libraries;
`slate-p58o.1.5` tracks that integration.
