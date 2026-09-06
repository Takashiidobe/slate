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
| `x`/`y` (SSE/AVX), `q`/`Q` (byte-addressable subset), `A` (edx:eax pair)                                           | **error** — needs a distinct Rust register class (`xmm_reg`, `ymm_reg`, …), not generic `reg`                                                                                                              |

CIR evidence for the memory row: an `m`/`g`/`imr` operand arrives as an
address (`!cir.ptr<T>`, `maybe_memory` marker) rather than a plain SSA value,
for both directions — Clang has already resolved "register or memory" down
to a concrete address, deferring only the reg-vs-mem choice (which Slate
never makes; it always takes the register/address-in-register form since
memory has no direct Rust operand kind).

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

No CIR shape investigated yet: `o`/`V` distinguished from plain `m`. No
lowering attempted regardless of soundness: `x`/`y`/`q`/`Q`/`A` register
classes. Tracked as `slate-3f8g.4.15.{1,2,3,4,5,6,7}`.
