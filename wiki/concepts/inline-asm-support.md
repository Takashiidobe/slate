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
- `asm goto` with labels — `tests/fixtures/gnu_asm_goto.c`
- Named operands (`%[name]`) — handled in template translation,
  `src/frontend/lowerer/asm.rs:200`
- clobber lists including `"cc"`/`"memory"`

## Not supported

- **Memory operand constraints (`"m"`/`"+m"`)** — errors cleanly at lowering
  (`unsupported inline asm input constraint`). Upstream blocker: Rust's
  stable `asm!` has no memory-operand class at all (deliberate inline-asm RFC
  design choice) — there is no operand kind to lower into. The only
  workaround is rewriting the template to take the operand's address
  explicitly via `in(reg)` and hand-editing the addressing mode in the
  template string, which changes codegen shape and doesn't generalize; not
  planned.
- **Flag-output constraints (`"=@ccX"`)** — lowers *without error* but emits
  invalid Rust (`lateout("@cce")`, not a real register — fails at `rustc`,
  not at slate translation time). This is the dangerous case: it looks
  successful until something actually builds the output. Tracked:
  slate-3f8g.4.15.

## Audit method

`gh search code "<pattern>" language:c` against real repos to find live
usage of obscure constraint/operand shapes, then a minimal standalone
repro under `/tmp` compiled with real `clang` first (to confirm the C
itself is valid) before running `cargo run -- translate` against it. Keep
using this to extend the table above before filing new fixtures/bugs.
