# Inline asm

<!-- toc -->
- [`Statement::Asm`](#statementasm)
- [Operand kinds and directions](#operand-kinds-and-directions)
- [Alternative selection](#alternative-selection)
  - [Constraint letters](#constraint-letters)
  - [Immediates and symbols](#immediates-and-symbols)
  - [Memory operands](#memory-operands)
- [Widths and modifiers](#widths-and-modifiers)
- [Rust options](#rust-options)
- [MSVC `__asm`](#msvc-__asm)
- [Naked functions](#naked-functions)
- [File-scope asm](#file-scope-asm)
- [Slate lowering](#slate-lowering)
<!-- /toc -->

Part of the [IR spec](../ir-spec.md). MSVC `__asm` parsing and effects are in
[msvc-asm](../msvc-asm.md); printed forms in the
[grammar](../ir-grammar.md#statements).

## `Statement::Asm`

A GNU `asm` keeps everything the parser resolved: the raw template,
`volatile`/`inline`/`goto`, resolved template pieces, outputs as places,
inputs as values, clobbers, and goto labels as `BindingId`s. The
instructions themselves are never parsed; Slate treats the template as text
with holes.

- Templates and text pieces contain decoded C string characters, including
  newlines and tabs; the Rust emitter must not decode them again.
- `InlineAsm::operands` is one list in GCC order (outputs, then inputs), so
  `AsmPiece::Operand { index }` is a direct subscript. A `Label(n)` piece
  indexes the label list, so `%l1` and `%l[done]` both print `%l0` when
  `done` is the first label.
- Constraints keep the parsed alternative list; the printer rebuilds the GNU
  spelling from it, so output round-trips the parse.
- Operand effects hoist ahead of the statement: outputs first in operand
  order, then inputs in source order, as clang and gcc evaluate them. A
  tied input sits in its output's slot as an `AsmTiedInput` that keeps its
  source operand number; `InlineAsm::inputs_in_source_order` rebuilds the
  order (`tied_order` in `ir_asm.c`).
- Each asm carries `AsmDialect::{Att, Intel}` on x86 (`None` elsewhere),
  per statement because MSVC `__asm` and GNU asm share a unit. GNU asm is
  AT&T until `-masm` is supported. `{att|intel}` alternations resolve during
  lowering; `%{`, `%|`, `%}` are literal.

## Operand kinds and directions

`AsmOperandKind`: `In(Value)`, `InPlace(place)`, `Out { place }`,
`InOut { place, input }`, `Symbol(AsmSymbol)`, and MSVC's `Memory`.

- Rust's forms are the inverse of the naive reading, since GCC's plain `=`
  lets an input share the register: `"=r"` is `lateout`, `"=&r"` `out`,
  `"+r"` `inlateout`, `"+&r"` `inout`. `=`/`+` come from the first
  alternative; `&` in any alternative makes it early-clobber (always safe).
- An input matching output `n` (`"0"`, `"[out]"`, or `"0,m"` when
  alternative 0 is chosen) folds into `InOut { input: Some(tied) }` and
  template pieces renumber. With no alternative chosen, only a tie in every
  alternative folds. `+` is `InOut { input: None }`.
- Parser rejections (both compilers agree): an output without `=`/`+`,
  `=`/`+`/`&` on an input, a match past the outputs or to a `+` output, two
  inputs tied to one output. `-` is rejected inside functions (`Pic` only
  survives in `Module::asm`). clang also rejects an input matching two
  outputs and counts any-alternative matches as ties; gcc only full ties.

## Alternative selection

Rust takes one class per operand, so lowering picks one alternative for the
whole asm: the first where every operand has a usable class.

- Within it, a constant input prefers the immediate (`"ri"(5)` prints `$5`
  in both compilers), then a register class the width fits, then an
  explicit register, then memory. For `rm`, gcc prefers the register and
  clang memory; we take the register, which Rust expresses without a
  pointer.
- Unusable: unresolved letters; x87/MMX/AMX (clobber-only in Rust); a
  register the width doesn't fit; an immediate that isn't an integer
  constant; a symbol that isn't a link-time address; a match on an output.
- Recorded as `InlineAsm::alternative`, `AsmOperand::selected`, and
  `AsmRejection`s. With nothing usable, `alternative` is `None` and
  operands lower unselected (`ir_asm_alternatives.c`).

### Constraint letters

- Letters resolve per target to `AsmOperandClass` (register class, explicit
  register, memory, immediate), keeping the raw letters. `g` is reg + mem +
  imm. `q` is `reg_abcd` on i686 but any register on x86-64; `R` is `reg` on
  i686 but `reg_legacy` on x86-64.
- Multi-letter: x86 `Y`/`W`/`j`/`B`, AArch64 `U` (three chars), Arm `U`.
  `?`, `!`, `*`, `^`, `$` are hints and skipped; `#` ends the alternative.
  Anything else (`l`, `X`, `s`, `p`, ...) is `Unresolved`, never guessed.
- `reg_legacy` and `vreg_low8` (AArch64 `y`) have no Rust class; emission
  pins a free explicit register from the set, avoiding the asm's other
  explicit operands and clobbers.
- `x` is `xmm_reg`, widened to `ymm_reg`/`zmm_reg` by a 256/512-bit operand;
  `v` is `zmm_reg`, the only Rust class reaching xmm16-31
  (`ir_asm_classes.c`).

### Immediates and symbols

- `i` (and `g`) offer `Symbol` besides `Immediate`; `n` and target
  immediate letters offer only `Immediate`.
- An input selects `Symbol` when it folds to a static-storage object or
  function plus a constant offset, through `&`, decay, field and constant
  index projections, constant pointer arithmetic, and pointer-width casts
  (`(long)&g`, not `(int)(long)&g` on x86-64). String literals qualify;
  automatics and non-constant indices don't; thread-locals only under gcc.
  Functions are accepted though both compilers reject them under PIE, which
  isn't modeled (`ir_asm_symbols*.c`).
- An `Immediate` input becomes `In(const<ty>(n))`, since Rust's `const`
  operand can't read a C object. The fold follows clang's evaluator: it
  reads const non-volatile objects through their initializers (`static
  const int k = 2`, `table[1]`, `s.field`, string elements, `*ptr` to a
  const, `(int)2.5`). gcc also reads a const tentative definition as zero,
  modeled for gcc only. Volatile, non-const, extern, and non-constant
  initializers stay unfolded (`ir_asm_const_objects*.c`).

### Memory operands

- A memory-class input (`m`, `o`, Arm `Q`, or `rm` whose register won't
  fit) on an addressable lvalue is `InPlace(place)`, emitted as
  `in(reg) &raw const x`. An input that chose a register reads the place.
  With no alternative chosen, any memory-capable constraint gives
  `InPlace`.
- An unaddressable operand (bit-field, lane, register variable, temporary)
  falls back to a value: compilers spill it, which an input can't tell from
  a copy. Errors only where compilers agree, for memory-only constraints:
  non-lvalues and bit-fields, and register variables under gcc. clang also
  rejects bit-fields and lanes under any memory-capable constraint; we
  follow gcc (`ir_asm.c` `memory()`, `ir_asm_memory_gcc.c`,
  `error/asm-memory-*.c`).

## Widths and modifiers

- Each operand carries `width`, its type's storage size in bits (x86-64
  `long double` is 128).
- A width modifier resolves to an `AsmRegisterView`: x86 `b` 8, `w` 16,
  `k` 32, `q` 64, `h` high byte, `x`/`t`/`g` 128/256/512; AArch64
  `b`/`h`/`s`/`d`/`q` 8-128 and `w`/`x` 32/64. Others keep only the letter.
- An unmodified x86 reference prints at the operand's width (`%dil` for a
  `char`), but Rust's `{0}` prints the full register, so emission adds a
  Rust modifier from the view or the width: GPR 8 `l`, 16 `x`, 32 `e`, 64
  `r`, high `h`; vector ≤128 `x`, 256 `y`, 512 `z`; AArch64 `reg` 32 `w`,
  64 `x`, `vreg` `b`/`h`/`s`/`d`/`q`. AArch64 unmodified references already
  match Rust. An explicit register is named at its width (`eax`).
- A tied input keeps its own width at its reference sites on x86 (gcc
  prints `"=r"(char) : "0"(int)` as `%dil` and `%edi`); clang rejects the
  mismatch (`ir_asm_widths.c`).
- Clobbers and explicit registers carry the source spelling and, when
  recognized, the canonical name; width is dropped.

## Rust options

A statement asm carries `InlineAsm::options: Some(AsmOptions)`; file-scope
asm and asm in naked functions have `None`. Verified against clang
`-emit-llvm` and gcc `-fdump-rtl-expand`:

| Option | Rule |
| --- | --- |
| `memory` (`None`/`ReadOnly`/`Any` = `nomem`/`readonly`/neither) | from operands: memory inputs read, memory outputs write. A `"memory"` clobber or basic asm forces `Any`. clang also forces `Any` for side-effecting asm (volatile, `goto`, no outputs); gcc keeps `nomem` for volatile extended asm |
| `pure` | not volatile, not `goto`, has an output, and `memory` is not `Any` |
| `nostack` | always for GNU asm (neither compiler guarantees stack alignment or red zone); off for MSVC `__asm` |
| `preserves_flags` | never on x86; elsewhere unless `"cc"` is written (gcc aarch64 and 32-bit ARM add a CC clobber only for `"cc"`) |
| `may_unwind` | an `"unwind"` clobber |

Fixtures: `ir_asm_options.c`, `ir_asm_options_gcc.c`, and the aarch64 and
armv7 variants.

## MSVC `__asm`

`src/sema/ms_asm.rs` lowers `__asm` to the same `Statement::Asm`: volatile,
Intel dialect, options `memory = Any` and nothing else (it may `push`/`pop`),
or `None` in a naked function. `template` is rebuilt from the AST.

- Each C object is one `Memory { place, access }` operand however often it
  appears; the asm receives its address, and `AsmAccess::{Read, Write,
  ReadWrite}` joins every reference (`src/sema/ms_asm_effects.rs`).
  Functions are `Symbol` operands; `offset x` is `In(AddressOf(x))`.
- Each reference is `AsmPiece::Address { operand, displacement, base,
  index, size }`: `arr[4]`, `s.f`, `x + 4` are byte displacements. `size` is
  the explicit `PTR`, or, where MASM infers it from the C type (no register
  operand, or `movzx`/`movsx`, shifts, rotates, `shld`/`shrd`), the
  innermost element size, matching the `dword ptr` clang inserts.
- Labels: clang flavor `AsmPiece::LocalLabel(name)`, lowercased; MSVC
  flavor `AsmPiece::EntryLabel { name, binding }` so a C `goto` can target
  it. A jump to a C label is an asm-goto `%lN` piece.
- Clobbers come from the effects table: written registers widened to 32
  bits (`al` → `eax`), implicit defs, `st`..`st(7)` for any x87
  instruction, `eax`/`ecx`/`edx` for `call`. Never `esp` or flags.
- 32-bit x86 implicit returns: integer and pointer results ≤ 8 bytes add
  EAX (and EDX above 4 bytes) `Out` operands writing shared synthetic `u32`
  locals. A written return register is an early output and leaves the
  clobber list. `Fallthrough::Return(Value)` (printed
  `fallthrough=ret(value)`) combines `u64(eax) | (u64(edx) << 32)` at the
  function end and converts it; narrow results truncate, `_Bool` takes the
  low bit. Explicit returns bypass it. C99 `main` zero-initializes its
  capture. Naked functions have none; noreturn keeps `ub`. Float, aggregate,
  wider, and x86-64 implicit returns are not lowered.
- Fixtures: `sema/i686-pc-windows-msvc/ms_asm_*.c`.

## Naked functions

`naked` on any declaration sets `FunctionSemantics::naked`, so the backend
picks `naked_asm!` from the function, not from its body. Keyed on the
attribute alone: clang accepts any number of asm statements (basic, or
extended with operands referencing no parameter, like `"i"(42)`) plus null
statements, and all fit one `naked_asm!`. clang rejects other statements;
gcc accepts anything, and so do we (permissive), so a backend must check the
body. Fallthrough is `ub`; inner asm has `options: None` (`ir_naked.c`).

## File-scope asm

File-scope `asm` lowers to `Module::asm`, a source-ordered list of
`InlineAsm` printed before the type definitions. It declares no entity, so
it stays out of `globals`/`functions`. It reuses `InlineAsm` because the GNU
personality accepts operands there, bound like a statement asm's; labels are
impossible.

## Slate lowering

- `slate/src/frontend/lowerer/asm.rs` consumes selected classes and pieces;
  `backend/rust_ast.rs` stores operands/options and `backend/codegen.rs` emits `asm!`.
- Core support: x86-64 `reg`/`reg_abcd` with 16/32/64-bit integers or pointers,
  integer `const` operands, and register clobbers. Other classes/types remain barriers.
- Outputs use scratch temporaries, then ordinary place writeback (including volatile stores).
  Tied inputs retain their input expression; directions come from `AsmOperand::direction()`.
- Register placeholders use the reference view or operand width. Literal braces escape;
  percent pieces become `%`; ordinary AT&T constants receive `$`.
- Unused operands get references in assembler comments to satisfy Rust's operand-use check.
- Extended asm maps parser options directly. Basic asm uses the decoded template with
  `raw`; its empty piece list does not distinguish text references.
- `memory`/`cc`/`unwind` clobbers are represented by the computed options;
  register clobbers become discarded `lateout` operands. Reserved registers remain barriers.
- Symbols, memory operands, special immediate modifiers, asm goto, naked/module asm,
  and operand type bridges are tracked by the other `slate-3f8g.4.17` children.
