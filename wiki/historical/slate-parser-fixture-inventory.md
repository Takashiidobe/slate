# Slate frontend fixture inventory

> Historical record. See the [archive index](index.md) for scope and source revision.

The [fixture inventory](slate-parser-fixture-inventory.tsv) records all 444 top-level
`tests/fixtures/*.c` files on the host target. It was generated on 2026-09-25
from Slate `38a847e5` and slate-parser `e5fc31f1` with:

```bash
cargo build --release
python3 tools/inventory_slate_frontend.py
```

The tool uses `target/test-cache/release/slate`, the configured release binary.
It invokes `translate-lowered` for each fixture and inspects
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

## Phase 3 close-out (2026-10-01)

The inventory tool was retired when the triage reports landed; use
`fixtures_unsupported_triage_report` for current first barriers. This section
records the `slate-p58o.3.24` audit against the TSV above.

- All 113 fixtures routed to `slate-p58o.3.24` now pass and live in
  `tests/fixtures/`. The last five (`enum_return_type_function_pointer_field`,
  `pointer_to_function_pointer_field`, `function_pointer_to_void_ptr_cast`,
  `local_named_err`, `ptr_param_field_addr_of_mut`) were fixed in this audit.
- 172 host fixtures remain in `tests/fixtures.unsupported/`. 132 are TSV rows
  owned by `slate-p58o.4`; 40 were added after the snapshot.
- The 40 post-snapshot fixtures are Phase 4 by family: inline/basic/goto asm
  and register operands, x86 SIMD intrinsics and vector builtins, GNU builtin
  and attribute surveys, `_BitInt`, zero-width bit-field ABI, and
  `printf_specifier_survey` (`slate-p58o.4.6`). `gnu_language` is a parser
  failure.
- The name-based route sent seven common-semantics fixtures to Phase 4. They
  are rerouted to Phase 3 children:

| Gap | Fixtures | Ticket |
| --- | --- | --- |
| switch fallthrough | `switch_fallthrough`, `switch_default_first_fallthrough`, `switch_default_middle_fallthrough` | `slate-p58o.3.41` |
| compound literals | `compound_literal_address` | `slate-p58o.3.42` |
| pointers to extern C functions | `fn_ptr_cmp_libc`, `libc_address_taken_safe_callback` | `slate-p58o.3.43` |
| anonymous members | `anonymous_members` | `slate-p58o.3.44` |

The remaining Phase 4 rows were spot-checked by first barrier: atomics,
volatile and `_Atomic` globals, bit-fields, packed/aligned records, complex,
VLAs, `goto`, GNU case ranges and statement-expression labels, constructors
and destructors, aliases and weakrefs, thread-locals, varargs forwarding,
long double and `_Float128`, FILE-based stdio, and builtins that do not link.
