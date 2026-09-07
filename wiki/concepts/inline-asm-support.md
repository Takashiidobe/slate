# GNU inline asm support

How Slate resolves a GCC/Clang inline-asm constraint to a concrete Rust
`asm!` operand. Epic: `slate-3f8g.4.15` (extends `slate-3f8g.4`). Code:
`src/frontend/lowerer/asm.rs`, `intrinsics.rs::lower_extended_asm`.

Rust's `asm!` has no virtual constraint language — every operand is one
concrete kind. GCC's constraint letters admit alternatives (register, memory,
immediate) the compiler picks among; Slate must resolve that choice ahead of
time. Two orthogonal lattices do this: **direction** (read/write/tie) and
**storage class** (register/immediate/memory). A third table handles the one
x86-specific register Rust reserves that C compilers don't.

## Direction

Total function — no error rows.

| write (`=`/`+`) | tied to an input | early-clobber (`&`) | Rust binding     |
| --------------- | ---------------- | ------------------- | ---------------- |
| .               | .                | .                   | `in(reg)`        |
| x               | .                | .                   | `lateout(reg)`   |
| x               | .                | x                   | `out(reg)`       |
| x               | x                | .                   | `inlateout(reg)` |
| x               | x                | x                   | `inout(reg)`     |

## Storage class

Whenever a constraint's alternative set includes `r`, `reg` is sound
regardless of what else is in the set — the compiler that accepted the
constraint already proved a register substitution works for how the template
uses the operand. That's why `g`/`imr` resolve to `reg` outright rather than
needing a register-pressure heuristic.

| Constraint                                                                                                         | Resolves to                                                                                                                                                                                                |
| ------------------------------------------------------------------------------------------------------------------ | ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| `{name}` explicit register                                                                                         | `Explicit(name)`                                                                                                                                                                                           |
| fixed letter `a`/`b`/`c`/`d`/`S`/`D`                                                                               | `Explicit(<width-name>)`; `b` routes through the ebx overlay below                                                                                                                                         |
| `=@ccX` flag output                                                                                                | synthesized `setcc` + zero-extend, bound `reg`                                                                                                                                                             |
| `r`, `g`, `imr`, any alternative set containing `r` (`ri`, `rm`, …)                                                | `reg`                                                                                                                                                                                                      |
| `i`, `n`, x86 range immediates `I J K L M N O`                                                                     | `const`, when CIR proves the value compile-time-constant; error otherwise (no fallback exists — no `r` alternative was offered)                                                                            |
| `p` (address operand)                                                                                              | address materialized in a register                                                                                                                                                                         |
| `m`/`o`/`V`/`+m`, or any set with no `r` alternative, template references the slot exactly once, no `%aN` modifier | address bound `in(reg)` (or read-before/write-after for `+m`/`=m`), template placeholder wrapped in deref syntax: AT&T `({0})`, Intel `[{0}]` with a synthesized `dword ptr`/`byte ptr`/`qword ptr` prefix |
| same, but the slot is referenced more than once or uses a `%aN` address modifier                                   | **error** — no per-template analysis attempted                                                                                                                                                             |
| `x` (SSE register)                                                                                                 | `xmm_reg` (`AsmReg::Class("xmm_reg")` — the backend's `AsmReg::Class` already takes an arbitrary class name, so no codegen changes were needed, only a new `AsmRegConstraint::Sse` resolution case)         |
| `Q` (always abcd), `q` in 32-bit mode, on a non-byte operand                                                        | `reg_abcd` (`AsmRegConstraint::ByteAddressableAbcd`/`ByteAddressableGpr`)                                                                                                                                    |
| `q` in 64-bit mode, on a non-byte operand                                                                          | `reg` (equivalent to unrestricted `Generic`, since GCC's `q` means "any GPR" there)                                                                                                                          |
| `q` in 64-bit mode, on a byte operand                                                                              | `reg_byte`                                                                                                                                                                                                  |
| `y` (MMX register), `Q` on a byte operand, `q` in 32-bit mode on a byte operand, `A` (edx:eax pair)                 | **error** — see below                                                                                                                                                                                       |

CIR evidence for the memory row: an `m`/`g`/`imr` operand arrives as an
address (`!cir.ptr<T>`, `maybe_memory` marker) rather than a plain SSA value,
for both directions — Clang has already resolved "register or memory" down
to a concrete address, deferring only the reg-vs-mem choice (which Slate
never makes; it always takes the register/address-in-register form since
memory has no direct Rust operand kind).

### Output wiring for addressed (`maybe_memory`) operands

CIR groups `cir.asm` operands as `out = [...], in = [...], in_out = [...]`.
Only operands passed by address ever appear in `out`; a register output never
does — it only ever comes through `op.res`/`op.res_ty` (packed into a struct
when there is more than one). This means the `out` group's length is exactly
the count of addressed outputs, and `register_output_count = total_output_count
- out.len()` where `total_output_count` is the number of raw constraints
starting with `=` (flag outputs included).

A raw output constraint's leading `*` (after stripping `=`/`&`) is CIR's own
signal that this operand is addressed — independent of whether the letter set
also contains `r`. `Constraint::Reg` carries this as `indirect: bool`.
Addressed + register-eligible outputs (`=g`, `=imr`, `+g`, ...) bind directly
to the target place via `place_or_deref_expr`, so `out(reg) x` or
`inlateout(reg) tied_input => x` writes straight into the real variable — no
temp, no separate write-back statement, and (for the tied case) the read-side
value already arrives pre-loaded through `in_out` as a plain SSA value, not
as another address to dereference. Addressed outputs with no `r` alternative
(pure `m`/`o`/`V`) fall back to the existing address-passthrough form (a plain
`in(reg)` of the address with the template deref-wrapped) since there's no
Rust-visible result to bind at all.

Before this wiring existed, `out`-group entries were flattened together with
`in`/`in_out` into one `input_operands` list with no group boundary tracked;
an addressed output with a register alternative (`=g`/`=imr`) silently landed
in the *input* constraint slot instead, reading the address as data with no
deref — no error, just a wrong runtime result. Fixed in `slate-3f8g.4.15.4`.

## `ebx`/`rbx` handling (from zstd to work around gcc's limitation)

Applies after storage-class resolves to an explicit register in the `b`
family. rustc rejects `ebx`/`rbx` as an explicit `asm!` operand
unconditionally (`rbx is used internally by LLVM`) — a Rust-target
restriction, not one either C compiler imposes.

| Resolved register              | Result                                                                                                                                                                                    |
| ------------------------------ | ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| not `bl`/`bh`/`bx`/`ebx`/`rbx` | used as-is                                                                                                                                                                                |
| in that family                 | rewritten: push `rbx`, move the real value in/out of a spare fixed-letter scratch register (`D`,`S`,`a`,`c`,`d`, first unused in the block), bind the scratch register instead, pop `rbx` |

## Explicit / non-constraint operands

| Form                                                     | Handling                                                                                                                      |
| -------------------------------------------------------- | ----------------------------------------------------------------------------------------------------------------------------- |
| Explicit register variable (`register int x asm("eax")`) | literal register name substituted directly into the template (Rust forbids `{N}` placeholders for explicit-register operands) |
| `asm goto` labels                                        | separate label-operand mechanism, not a constraint; tied to CIR's dispatch state                                              |
| Operand-free template (no placeholders referenced)       | operand bindings omitted; volatility and memory clobber preserved                                                             |
| Clobber list (`"cc"`, `"memory"`)                        | passed through directly                                                                                                       |

## Todo

`x` (SSE) implemented (`slate-3f8g.4.15.6`). `q`/`Q` on non-byte operands
implemented (`slate-3f8g.4.15.8`), which also threads target pointer-bitness
(`TargetArch::pointer_bits`, computed from `module.triple` once in
`Lowerer::target_pointer_bits`) down through `asm_operand_bits`,
`asm_reg_for_constraint`, and the template-modifier-suffix machinery —
fixing the pre-existing `asm_operand_bits` bug where pointer/fn-pointer
operands were hardcoded to 64 bits regardless of target. `y`, byte-sized
`Q`/`q`, and `A` remain unconditional errors — audited via GitHub code
search (exact-case, x86 context) before deciding scope:

- **`y`** (MMX register): real but rare/legacy usage (msieve/yafu lanczos,
  kvm-unit-tests, pm123 3DNow). Blocked regardless of demand — Rust's
  `mmx_reg` register class can only be used as a clobber, never bound as an
  input or output (`rustc` error: "register class `mmx_reg` can only be used
  as a clobber"). No lowering is possible against stable `asm!`.
- **byte-sized `Q`, and byte-sized `q` under a 32-bit target**: confirmed via
  real Clang codegen that GCC/Clang fully support this, including the `%h`
  high-byte modifier (`ah`/`bh`/`ch`/`dh`) — not a rare corner case. The gap
  is entirely Rust's: `reg_abcd` has no `i8` arm at all (only
  `i16/i32/i64/f16/f32/f64`), and naming `ah`/`bh`/`ch`/`dh` as an explicit
  register operand is flatly rejected (`rustc`: "high byte registers cannot
  be used as an operand on x86_64"). `q` under a 64-bit target sidesteps
  this entirely — it resolves to `reg_byte`, which does support `i8`
  directly. A sound workaround exists (verified empirically, same shape as
  the `ebx`/`rbx` fixup below): bind a wider temp via `reg_abcd` and use
  Rust's `{N:l}`/`{N:h}` template modifiers, which *are* permitted on a
  class-bound operand even though naming the register directly isn't —
  `asm!("incb {0:h}", inlateout(reg_abcd) v)` on `0x1234` correctly produced
  `0x1334`, disassembling to a bare `inc %ah` with no REX prefix. Filed as
  `slate-3f8g.4.15.10` (needs a new `TemplateModifier::HighByte` for CIR's
  `${N:h}`, confirmed distinct from bare `$N`, plus widen/narrow glue
  analogous to the `EbxFixup` machinery).
- **`A`** (edx:eax pair): very rare (~4 hits, all the legacy `rdtsc` idiom;
  modern code mostly writes `"=a","=d"` directly instead). Not a
  register-class mapping like the others — one constraint spanning two tied
  registers is a new lattice shape, lowest priority given the frequency.

`o`/`V` confirmed CIR-identical to `m` (`slate-3f8g.4.15.5`): both arrive as
`*o`/`*V` in the raw constraint string with the same `maybe_memory` address
shape. `strip_memory_marker` treats `Offsettable`/`NonOffsettable` atoms as
memory-like alongside `Memory`, so `o`/`V` (and their `+o`/`+V` ties) fold
into the same `Constraint::Memory` address-passthrough path as `m` — no
distinct offsettable-vs-not handling exists or is needed, since Slate always
materializes the address in a register rather than picking a displacement
form.

## AArch64

Supported: `r` -> `reg` (arch-agnostic, no AArch64-specific code needed).
Register width: implicit width attaches `{N:w}`/`{N:x}`
(`rust_asm_register_modifier`); explicit `%w`/`%x` template modifiers ->
`TemplateModifier::RegisterWidth`. `options(att_syntax)` is never emitted for
non-x86 targets (`cir_asm_dialect` gated on `TargetArch::is_x86()`).

Fixture: `tests/fixtures/aarch64/asm_aarch64_reg_width_modifiers.c`.

## ARM (32-bit)

Supported: `r` and `l` (Thumb1 low regs r0-r7, alias for `r` elsewhere) both
-> `reg`. Sound without tracking ARM/Thumb state: Rust's ARM `reg` class
already narrows to r0-r7 under Thumb1 and expands under Thumb2/ARM. No
register-width modifier on ARM32.

Limitation: `h` (Thumb r8-r15) is unsupported (clear error, not silently
wrong) — Rust has no register class for that set, and unlike `l` it isn't a
subset of `reg`, so mapping it there would be unsound.

Fixture: `tests/fixtures/arm/asm_arm_general_low_reg.c`.

Memory constraints: `Uv`, `Uy`, and `Uq` are canonicalized by CIR as
`*^Uv`/`*^Uy`/`*^Uq` and reuse the `Constraint::Memory` address-passthrough
path used by `m`/`o`/`V`. ARM templates render the materialized address with
`[reg]` rather than x86's `(reg)` syntax. The ARM stack-pointer constraint `k`
is rejected by the current CIR-enabled Clang before CIR generation, so it has
no Slate-side workaround.

Fixture: `tests/fixtures/arm/asm_arm_vfp_memory_constraint.c`.

Immediate constraints: scalar `I` and `M` resolve to Rust `const` operands when
CIR proves the input constant. `J`, `K`, `L`, and `N` use the same path; `O` is
not fixture-covered because the CIR-enabled Clang rejects it for the ARM target
even with Thumb enabled. ARM NEON vector-immediate letters are out of scope:
Rust `asm!` has no equivalent vector-constant operand class.

## Fixture layout

Arch-exclusive fixtures live in `tests/fixtures/{arm,aarch64,x86_64}/` (mirrors `bionic`/`macos`/`msvc`) instead of by-name skip-lists or FileCheck-prefix tricks; each differential runner reads only its own directory plus the shared root, and `update_filecheck.py` auto-generates single-target checks for paths under them.
