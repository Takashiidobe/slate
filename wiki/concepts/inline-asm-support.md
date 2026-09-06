# GNU inline asm support

Tracks what GCC-style `asm`/`__asm__` inline assembly slate can lower today,
found by probing real-world constraint/operand shapes (harvested via
`gh search code`) against `cargo run -- translate`. Parent epic:
slate-3f8g.4. Code: `src/frontend/lowerer/asm.rs`.

If a construct has no equivalent in Rust's stable `core::arch::asm!`, that's
an **upstream blocker**, not a slate gap — no amount of lowering work fixes it.
Some of what looks like a blocker at first (memory operands) turns out to be
narrower than it looks once you check what CIR actually hands us; see below.

## Lowering model

GCC and Clang describe inline assembly operands in a virtual constraint
language. A constraint gives the compiler a set of legal representations and
the compiler chooses among them for the current target and register pressure.
Rust's `asm!` does not expose that virtual choice: each operand must be
lowered to a concrete Rust operand kind.

| GNU constraint   | Meaning                                                           | Rust representation      |
| ---------------- | ------------------------------------------------------------------ | ------------------------- |
| `i`              | compile-time immediate                                            | `const`                  |
| `m`              | memory lvalue/addressing mode                                     | address rewrite, or none |
| `r`              | general-purpose register                                          | `reg`                    |
| `g`              | target-defined general operand, usually immediate/memory/register | `reg` (see below)        |
| `imr`            | explicitly immediate/memory/register                              | `reg` (see below)        |
| `+`              | read/write                                                        | `inout` or `inlateout`   |
| `=`              | write-only                                                        | `out` or `lateout`       |
| `&`              | early-clobber; cannot overlap inputs                               | non-late `out`/`inout`   |
| numeric matching | ties an input to an output                                        | tied `inout`              |

CIR makes the virtual-choice mismatch visible rather than hiding it. Lowering
follows these rules:

1. Preserve the asm template and all referenced operands. A constraint may be
   simplified only when the chosen Rust form has the same observable value,
   aliasing, and clobber behavior.
2. Map register-only constraints directly to Rust register operands. Map
   immediates to `const` only when CIR proves the value is compile-time
   constant.
3. `g`/`imr`/any multi-alternative constraint that offers `r` always resolves
   to the `reg` class. This is not a heuristic — see "Why `g`/`imr` resolves
   to a register" below for why it's sound by construction, and why it does
   *not* generalize to picking memory.
4. A constraint whose only alternative is memory (`m`, `+m`, with no `r` in
   the set) has no register-class fallback. See "Memory operands" under Not
   supported for the address-rewrite path that partially covers this.
5. An operand-free template is a special case. If no operand placeholder is
   referenced, operand bindings can be omitted, while preserving volatility and
   the memory clobber. Read/write values that are semantically observed still
   need an explicit preservation strategy.
6. `=r` normally maps to `lateout(reg)` because the output may overlap an input
   after inputs are consumed. `=&r` maps to `out(reg)` because early-clobber
   forbids that overlap.
7. A resolved register that lands on the `ebx`/`rbx` family needs the
   save/restore rewrite in "x86 `ebx`/`rbx`" below — Rust's `asm!` rejects it
   as an operand outright, unlike either C compiler.

This policy is why the `.66` GCC dg cases split into an easy operand-free group
and separate real-memory/asm-goto and target-specific register-class groups.

## Why `g`/`imr` resolves to a register, not a heuristic

Compiling two minimal repros through `SLATE_CLANG -Xclang -fclangir -Xclang
-emit-cir` shows Clang has already done most of the work before CIR exists.

`"=r"(out) : "r"(x)`:

```
%4 = cir.asm(x86_att, out = [], in = [%3 : !s32i], in_out = [],
  {"movl $1, $0" "=r,r,~{dirflag},~{fpsr},~{flags}"}) -> !s32i
```

`"=g"(out) : "g"(x)`:

```
cir.asm(x86_att,
  out = [%2 : !cir.ptr<!s32i> (maybe_memory)],
  in = [%3 : !s32i], in_out = [],
  {"movl $1, $0" "=*imr,imr,~{dirflag},~{fpsr},~{flags}"})
```

Three things fall out of this:

- **`g` is already `imr` by the time CIR exists.** Clang canonicalizes `g` →
  `imr` on x86 before emitting CIR. There is no separate `g` case to design
  for.
- **Inputs never carry a memory option.** The input arrives as a plain SSA
  value (`%3 : !s32i`), not an address — Clang has already resolved "register
  or memory" down to "register" for inputs by the time CIR sees them. There is
  nothing left to choose between register and memory on the input side.
- **Outputs defer the choice, but to LLVM, not to us.** The `(maybe_memory)`
  marker plus the address (`%2 : !cir.ptr<!s32i>`) means Clang is punting the
  real register-vs-memory decision to LLVM's InlineAsm operand lowering,
  which runs during instruction selection / register allocation — informed by
  real register pressure slate has no equivalent of (see "Rejected:
  consulting LLVM's register allocator").

None of that requires a heuristic, though, because of one thing that's true
regardless: **whenever a constraint's alternative set includes `r`, picking
`reg` is sound by construction.** The constraint string is the proof — the
alternative was checked and accepted by the compiler that first compiled this
C, which only works if a register substitution is valid for however the
template uses that operand. If the template needed memory-only semantics
(e.g. `lock incl %0`, `leal %0, %%eax`), the author would have had to write
`m` alone, not `g`/`imr`; that's a different, narrower case (see "Memory
operands" below), not this one.

`lower_extended_asm`'s current parser already applies this on the input side
(`asm.rs:436-442`, tracked in `wiki/log/2026-08-30-14-37.md`). It does **not**
yet apply on the output side: `parse_output_reg_constraint` strips a leading
`=` and chokes on the literal `*imr` token it finds instead (the `*` marks
the `maybe_memory` address-passing CIR uses for outputs), so `"=g"`/`"=imr"`
currently hard-errors exactly like pure `"=m"` — despite the "Supported"
section below previously listing `g`/`imr` without qualifying that this only
held for inputs. Since `r` is provably legal per the argument above, the
output case can lower exactly like a plain `=r` output: read the value once
through the CIR-given address, bind it to a Rust `out(reg)`/`lateout(reg)`
temp via the existing `direct_output`/`asm_output_places` path
(`intrinsics.rs:237-260`), and write it back through that address afterward
instead of trusting `op.res`. Not yet implemented; tracked as a follow-up.

## Rejected: consulting LLVM's register allocator

Considered feeding an isolated asm statement through a real LLVM compile to
get its actual register-vs-memory decision for `maybe_memory` outputs, then
translating whatever it picked. Rejected:

- The only input that makes that decision meaningful is the *real* function's
  register pressure. Isolating just the asm statement into its own tiny
  function strips that signal out — an empty function has no competing
  demand for registers, so LLVM will pick `reg` almost every time. That's the
  same answer the constraint-membership proof above already gives for free,
  for zero cost and no new dependency.
- It would add a new per-block subprocess invocation into the *lowering*
  pipeline. Nothing else in slate shells back out to clang/llvm mid-lowering
  — `SLATE_CLANG`/`SLATE_CIR_OPT` only run once, at the frontend boundary, to
  produce CIR in the first place.
- Even a perfect answer wouldn't change what we can emit: Rust's `asm!` still
  has no native memory-operand class, so "LLVM picked memory" doesn't hand us
  a representation on its own — we'd still need the address-rewrite path
  below to act on it. The consultation step buys nothing the constraint
  string didn't already tell us.

## x86 `ebx`/`rbx`: a rustc restriction the source C never had to satisfy

`rustc`'s `asm!` unconditionally refuses to bind `ebx`/`rbx` as an explicit
operand register, regardless of constraint class or PIC mode:

```
error: cannot use register `bx`: rbx is used internally by LLVM and cannot
be used as an operand for inline asm
```

This is **not** inherited from either C compiler. `SLATE_CLANG` compiles a
`"=b"(x)` cpuid-style output cleanly in every configuration tested (no PIC,
`-fPIC`, `-m32 -fPIC`). Real-world C that appears to special-case `ebx`
usually isn't dodging a Clang restriction at all — zstd's `cpu.h` (vendored
at `~/c-corpus/zstd/lib/common/cpu.h:89`) has a manual push/pop-`ebx` branch
gated `defined(__i386__) && defined(__PIC__) && !defined(__clang__) &&
defined(__GNUC__)`, with the comment *"gcc reserves ebx for use of its pic
register so we must specially handle the save and restore"* — that's **GCC**,
on 32-bit `-fPIC`, needing the workaround; Clang's default branch just uses
`"=b"(f7b)` directly, no push/pop. (The file's other `ebx`-adjacent branch,
gated on `__clang_major__ < 16`, works around a bug in the MSVC-style
`__cpuid` *intrinsic*, unrelated to inline-asm constraints entirely.)

So the source C's own conditional branches, if any, don't map onto what
slate needs to emit — a GCC-only or pre-16-Clang-only branch in the original
source is not evidence that the path Slate is translating needs special
handling, and a plain `"=b"` with no source-side workaround at all still
needs one on the Rust side, unconditionally. Slate detects when a resolved
output/input register lands on the `ebx` family (`is_ebx_family_reg`,
`asm.rs:490`) and rewrites the block: picks a spare fixed-letter scratch
register not already used by another operand in the same block
(`pick_ebx_scratch_letter`, preferring D/S then a/c/d), pushes `rbx`, moves
the real value in/out of the scratch register with hand-written literal
`mov` text (not a `{N}` placeholder — rustc forbids referencing an
explicit-register operand from the template), and pops `rbx`. Implemented in
`lower_extended_asm`'s `EbxFixup` handling (`intrinsics.rs:210-284`,
`:398-428`); see `wiki/log/2026-08-30-14-53.md` for the session that built it
and the earlier-rejected draft (binding the scratch register via the generic
`reg` class instead of a second explicit-register operand, which desyncs
`{N}` template indices once rustc's explicit-operand ordering rules kick in).

The general lesson: some of the rewriting slate does exists purely to
satisfy the Rust target's own toolchain restrictions, with no C-side
equivalent or motivation at all. It's worth keeping this distinction sharp in
the lattice — a row can require Rust-side scaffolding that a straight
constraint-to-operand mapping wouldn't predict.

## Supported

- Explicit register variables (`register int x asm("eax")`) —
  `tests/fixtures/gnu_asm_register_variable.c`. Rust's `asm!` forbids
  referencing explicit-register operands via `{N}` template placeholders, so
  `translate_asm_template` substitutes the literal register name (with a
  `%` prefix under AT&T dialect) directly into the template text instead.
- Basic asm (`asm("nop")`, no operands) — `tests/fixtures/gnu_basic_asm.c`
- Extended asm with output/input operands, `"r"` register constraints, and
  `"g"`/`"imr"` generic constraints **on inputs** —
  `tests/fixtures/gnu_extended_asm.c`. `g`/`imr` on outputs is not yet
  implemented; see "Why `g`/`imr` resolves to a register" above.
- Fixed-letter x86 register constraints (`"a"`/`"b"`/`"c"`/`"d"`/`"S"`/`"D"`)
  — `tests/fixtures/asm_fixed_register_cpuid.c`, `asm_ebx_output_cpuid.c`
- Explicit register constraints via `"{regname}"`
  — `asm_reg_for_constraint` in `src/frontend/lowerer/asm.rs:358`
- `ebx`/`rbx`-family register bindings, via the push/pop scratch-register
  rewrite — see "x86 `ebx`/`rbx`" above.
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

- **`g`/`imr` on outputs.** Currently hits the same hard error as pure `m`
  (see next item) because `parse_output_reg_constraint` doesn't yet handle
  the `maybe_memory`/`*`-prefixed CIR shape. Fixable without a heuristic —
  see "Why `g`/`imr` resolves to a register" above; just not wired up yet.

- **Memory operand constraints (`"m"`/`"+m"`, no `r` alternative)** — errors
  cleanly at lowering (`unsupported inline asm input constraint`). Rust's
  stable `asm!` has no memory-operand class at all (deliberate inline-asm RFC
  design choice), so there is genuinely no operand kind to lower an `m`-only
  constraint into directly.

  A narrower path is designed but not implemented: CIR already computes the
  operand's address for us (the same `%2 : !cir.ptr<...>` pointer the
  `maybe_memory` output case gets), so the addressing-mode work — array
  indexing, struct-field offsets, whatever the original lvalue was — doesn't
  need to be re-derived. What's missing is purely a template rewrite: bind
  the address as `in(reg)` (or read-before/write-after through it for
  `+m`/`=m`), and wrap that slot's placeholder in explicit deref syntax
  instead of leaving it bare:

  ```rust
  // C: __asm__("incl %0" : "+m"(*p));
  asm!("incl ({0})", in(reg) p, options(att_syntax));       // AT&T
  asm!("inc dword ptr [{0}]", in(reg) p);                   // Intel
  ```

  AT&T needs no size annotation (the mnemonic suffix already carries operand
  width); Intel needs a `dword ptr`/`byte ptr`/`qword ptr` prefix, derivable
  from the pointee type via the same logic `asm_operand_bits`/
  `rust_asm_register_modifier` already use.

  This only covers the shape where the `m`-constrained slot is referenced
  **exactly once** in the template with no address modifier (GCC's `%aN`
  address-extraction syntax). A slot referenced more than once, or combined
  with an address modifier, or aliasing another `m`-constrained operand in a
  way the naive per-slot rewrite doesn't preserve, needs per-template
  analysis this doesn't attempt — those stay hard errors. Considered and
  rejected asking LLVM's register allocator to resolve this instead; see
  "Rejected: consulting LLVM's register allocator" above.

## Audit method

`gh search code "<pattern>" language:c` against real repos to find live
usage of obscure constraint/operand shapes, then a minimal standalone
repro under `/tmp` compiled with real `clang` first (to confirm the C
itself is valid) before running `cargo run -- translate` against it. Keep
using this to extend the table above before filing new fixtures/bugs.

When the question is what CIR actually represents for a given constraint
shape (not just what real `clang` accepts), dump CIR directly instead of
guessing from the constraint string alone:

```bash
$SLATE_CLANG -Xclang -fclangir -Xclang -emit-cir -S -o - repro.c | grep -A6 cir.asm
```

This is how the `maybe_memory`/output-address shape above was found — the
constraint string alone (`"=*imr,imr,..."`) doesn't distinguish "address
passed, decision deferred" from any other output shape without also reading
the `cir.ptr<...>`/`(maybe_memory)` annotation on the operand.
