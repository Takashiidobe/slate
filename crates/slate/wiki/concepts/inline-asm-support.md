# GNU inline asm support

Tracks what GCC-style `asm`/`__asm__` inline assembly slate can lower today,
found by probing real-world constraint/operand shapes (harvested via
`gh search code`) against `cargo run -- translate`. Parent epic:
slate-3f8g.4. Code: `src/frontend/lowerer/asm.rs`.

If a construct has no equivalent in Rust's stable `core::arch::asm!`, that's
an **upstream blocker**, not a slate gap — no amount of lowering work fixes it.

## Supported

- Explicit register variables (`register int x asm("eax")`) —
  `tests/fixtures/gnu_asm_register_variable.c`. Rust's `asm!` forbids
  referencing explicit-register operands via `{N}` template placeholders, so
  `translate_asm_template` substitutes the literal register name (with a
  `%` prefix under AT&T dialect) directly into the template text instead.
- Basic asm (`asm("nop")`, no operands) — `tests/fixtures/gnu_basic_asm.c`
- Extended asm with output/input operands, `"r"`/`"g"`/`"imr"` generic
  register constraints — `tests/fixtures/gnu_extended_asm.c`
- Fixed-letter x86 register constraints (`"a"`/`"b"`/`"c"`/`"d"`/`"S"`/`"D"`)
  — `tests/fixtures/asm_fixed_register_cpuid.c`, `asm_ebx_output_cpuid.c`
- Explicit register constraints via `"{regname}"`
  — `asm_reg_for_constraint` in `src/frontend/lowerer/asm.rs:358`
- `"+r"` read-write constraints — compiles to `inlateout(reg)`
- Numeric matching constraints (`"0"` tying an input to an output) — compiles
  to `inlateout(reg)`
- `asm goto` with labels, including a macro-expanded statement expression —
  `tests/fixtures/gnu_asm_goto.c`, `asm_goto_statement_expr.c`
- Named operands (`%[name]`) — handled in template translation,
  `src/frontend/lowerer/asm.rs:200`
- Whole-template Intel wrappers (`.intel_syntax noprefix` through
  `.att_syntax prefix`) and `%Vn` no-prefix operands — normalized to Rust's
  native Intel dialect in `tests/fixtures/asm_dialect_switch.c`
- x86 flag-output constraints (`=@ccX`) — synthesized as `setcc` plus
  zero-extension into a normal 16-, 32-, or 64-bit register output in
  `tests/fixtures/gnu_asm_flag_outputs.c`
- clobber lists including `"cc"`/`"memory"`

## Not supported

- Repeated statement-expression macro expansions that reuse scoped asm-goto
  label names in one function fail explicitly. CIR's goto solver removes the
  scoped label identity while flattening; Slate rejects the missing or
  ambiguous target instead of selecting another same-named label.

- **Memory operand constraints (`"m"`/`"+m"`)** — errors cleanly at lowering
  (`unsupported inline asm input constraint`). Upstream blocker: Rust's
  stable `asm!` has no memory-operand class at all (deliberate inline-asm RFC
  design choice) — there is no operand kind to lower into. The only
  workaround is rewriting the template to take the operand's address
  explicitly via `in(reg)` and hand-editing the addressing mode in the
  template string, which changes codegen shape and doesn't generalize; not
  planned.
## Audit method

`gh search code "<pattern>" language:c` against real repos to find live
usage of obscure constraint/operand shapes, then a minimal standalone
repro under `/tmp` compiled with real `clang` first (to confirm the C
itself is valid) before running `cargo run -- translate` against it. Keep
using this to extend the table above before filing new fixtures/bugs.
