# GNU inline asm support

Tracks what GCC-style `asm`/`__asm__` inline assembly slate can lower today,
found by probing real-world constraint/operand shapes (harvested via
`gh search code`) against `cargo run -- translate`. Parent epic:
slate-3f8g.4. Code: `src/frontend/lowerer/asm.rs`.

If a construct has no equivalent in Rust's stable `core::arch::asm!`, that's
an **upstream blocker**, not a slate gap — no amount of lowering work fixes it.

## Lowering model

GCC and Clang describe inline assembly operands in a virtual constraint
language. A constraint gives the compiler a set of legal representations and
the compiler chooses among them for the current target and register pressure.
Rust's `asm!` does not expose that virtual choice: each operand must be
lowered to a concrete Rust operand kind.

| GNU constraint   | Meaning                                                           | Rust representation      |
| ---------------- | ----------------------------------------------------------------- | ------------------------ |
| `i`              | compile-time immediate                                            | `const`                  |
| `m`              | memory lvalue/addressing mode                                     | no direct equivalent     |
| `r`              | general-purpose register                                          | `reg`                    |
| `g`              | target-defined general operand, usually immediate/memory/register | choose one concrete form |
| `imr`            | explicitly immediate/memory/register                              | choose one concrete form |
| `+`              | read/write                                                        | `inout` or `inlateout`   |
| `=`              | write-only                                                        | `out` or `lateout`       |
| `&`              | early-clobber; cannot overlap inputs                              | non-late `out`/`inout`   |
| numeric matching | ties an input to an output                                        | tied `inout`             |

On x86, `g` and `imr` normally admit the same practical locations, but `g` is
target-defined while `imr` spells out the alternatives. A Rust lowering must
choose whether to keep an immediate, load a register, or pass an address; it
cannot preserve that choice as a single `asm!` constraint.

CIR makes this mismatch visible. For example, a C `=m` output may appear as
`=*m` in `cir.asm`: the `*` records that CIR passes the address of the memory
object. The `operands_segments` attribute separates outputs, inputs, and label
operands; the constraint string alone is not enough to infer operand roles.

Lowering follows these rules:

1. Preserve the asm template and all referenced operands. A constraint may be
   simplified only when the chosen Rust form has the same observable value,
   aliasing, and clobber behavior.
2. Map register-only constraints directly to Rust register operands. Map
   immediates to `const` only when CIR proves the value is compile-time
   constant.
3. Do not treat `g`, `rm`, or `imr` as register constraints. Selecting `reg`
   is a valid specialization only when the generated template and operand
   semantics permit it.
4. Memory constraints require either an explicit address-based template
   rewrite or a compiler/runtime shim. Rust has no general memory operand
   class, so arbitrary `%0` memory syntax cannot be transliterated faithfully.
5. An operand-free template is a special case. If no operand placeholder is
   referenced, operand bindings can be omitted, while preserving volatility and
   the memory clobber. Read/write values that are semantically observed still
   need an explicit preservation strategy.
6. `=r` normally maps to `lateout(reg)` because the output may overlap an input
   after inputs are consumed. `=&r` maps to `out(reg)` because early-clobber
   forbids that overlap.

This policy is why the `.66` GCC dg cases split into an easy operand-free group
and separate real-memory/asm-goto and target-specific register-class groups.

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
