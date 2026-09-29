# MSVC inline asm

<!-- toc -->
- [Oracle behavior](#oracle-behavior)
  - [Operands](#operands)
  - [Statement boundaries](#statement-boundaries)
  - [Acceptance](#acceptance)
- [Design](#design)
  - [Recognition and gating](#recognition-and-gating)
  - [Parser and AST](#parser-and-ast)
  - [Sema: names and folding](#sema-names-and-folding)
  - [IR](#ir)
  - [Effects table](#effects-table)
  - [Implicit return](#implicit-return)
- [Rust-side notes for Slate](#rust-side-notes-for-slate)
<!-- /toc -->

MSVC `__asm` supplies no constraints, so the frontend infers reads, writes,
and clobbers. The body stays text and lowers into the same `ir::InlineAsm`
as GNU asm (`dialect = Intel`); only operands and operand references are
structured (`todo/ir-asm-design.md`). The MASM/`ml64` directive layer is
out of scope. Example inputs: `todo/msvc-asm-example.c`.

Needed for `#include <windows.h>` on `i686-pc-windows-msvc`: `winnt.h`
defines `Int64ShllMod32`, `Int64ShraMod32`, `Int64ShrlMod32` with `__asm {}`
bodies (byte offsets into a u64, EDX:EAX implicit return) and
`DbgRaiseAssertionFailure` with `__asm int 0x2c`. Pinned by
`sema/i686-pc-windows-msvc/windows_h_{clang,msvc}.c`.

## Oracle behavior

clang 22.1.8 (`-target i686-pc-windows-msvc -emit-llvm`) and `tools/cl.exe`
(`MSVC_ARCH=x86`, `/FAs`); they agree unless noted.

### Operands

| Source | Meaning |
| --- | --- |
| `mov eax, x` | Memory operand: asm gets `x`'s address (clang `*m` read, `=*m` write). |
| `arr[4]` | Byte offset 4, not element 4. |
| `s.b` | Field byte offset. |
| `TYPE` / `LENGTH` / `SIZE arr` | Element size, count, total size (4/4/16 for `int[4]`); first array level only (`int m[2][3]` → 12/2/24). |
| `[eax]T.f` | `T` a struct typedef: `[eax + offsetof(T, f)]`. A struct tag fails in clang. |
| `jmp x`, C name and asm label both `x` | C name wins. |
| `100h`, `0Ch`, `0x2c`, `10b`, `17o`, `010`, `1u` | MASM suffixes (`h`, `b`, `o`/`q`, `d`) and C spellings both accepted; printed decimal. `t`/`y`: clang only. |
| `mov result, 1`, `fld a`, `mul b` | Size from the C type, inserted only where ambiguous. Explicit `dword ptr [Value]` on a 64-bit variable wins. |
| `call g` | Direct call to the symbol (clang `${8:P}`), not memory. |
| `done:` / `jz done` | Asm-local label, uniqued per asm. |
| Falling off the end | EAX (EDX:EAX for 64-bit) is the return value; clang adds an `{eax}`/`A` output to every MS asm in a register-returning function (`=&` if written). |
| Clobbers | Registers written explicitly or implicitly (`push`/`pop` → `~{esp}`, `popfd` → `~{dirflag}`, `mul` → EDX:EAX), flags always. clang doesn't treat `call` as clobbering caller-saved registers; MSVC saves callee-saved registers the block uses. |
| Attributes | `sideeffect inteldialect`, `nounwind`; not `alignstack`. |

- C/asm label jumps: MSVC accepts `jmp c_label` and `goto asm_label`;
  clang rejects both. The msvc flavor lowers the first as an asm-goto
  label operand and records the second as an entry label at its template
  position.
- An apostrophe in a `;` comment is only a clang warning and lexes fine.

### Statement boundaries

- An instruction ends at end of line, the next `__asm`, or `}`.
- A macro expansion is one line; macros need `__asm` separators.
- `;` comments run to end of line; a `}` inside is ignored.
- A single-line `__asm` continues through further `__asm` on the same line
  and through a following line starting with `__asm` but not `__asm {`. A
  braced block ends at `}`. `__asm` then `{` on the next line is a braced
  block.

### Acceptance

- MSVC: `__asm`, `_asm`, x86 only (x64: C4235). `asm {` is not a keyword.
- clang: `__asm`, `_asm`, `asm {` on every `*-windows-msvc` triple,
  x86_64 included; elsewhere needs `-fasm-blocks`.
- gcc: never.

## Design

### Recognition and gating

- In statement position, `__asm`/`_asm` not followed by `(` or a GNU
  qualifier starts an MS asm statement. `asm {` is not modeled.
- msvc flavor: i686 only; other arches error like C4235.
- clang flavor: any x86 `*-windows-msvc` triple (permissive); other triples
  use the GNU parser, which rejects it like clang without `-fasm-blocks`.

### Parser and AST

`StmtKind::MsAsm(MsAsm)`, separate from `GnuAsm`.

```rust
MsAsm { instructions: Vec<Span<MsAsmInstruction>> }
MsAsmInstruction {
    label: Option<Span<String>>,
    prefixes: Vec<Span<String>>,        // rep, repne, lock
    mnemonic: Option<Span<String>>,     // None for a label-only line
    operands: Vec<Span<MsAsmExpr>>,     // split on top-level commas
}
MsAsmExpr = Register | Number(u64) | Name(String)
          | Member(e, field) | Index(e, e) | Bracket(e)
          | Binary(op, e, e) | Neg(e)
          | Ptr(size, e) | Segment(reg, e)
          | Operator(Type | Length | Size | Offset | Short, e)
          | St(n)
```

- `Parser::mark_ms_asm_lines` rewrites the token stream once, before brace
  matching: drops `;` comments and inserts `Token::Newline` at instruction
  boundaries (line breaks from expansion `Loc` + file line tables).
  `parse_ms_asm_stmt` only reads tokens.
- Mnemonics accept C keyword tokens (`int 3`, `jmp short`).
- Registers, `PTR`, sizes, operators are case-insensitive; C names are not.
- Numbers fold at parse time (`h`, `b`/`y`, `o`/`q`, `d`/`t`, `0x`); MASM
  hex is a pp-number (`0FFh`).
- `_emit n` is an instruction lowering to `.byte n`.

### Sema: names and folding

- `sema/names.rs` resolves each `Name` in the ordinary namespace (object,
  parameter, function, enumerator, typedef), else as an asm label anywhere
  in the function (case-insensitive), else "unresolved label name". Asm
  labels are separate from C labels in the clang flavor; the msvc flavor
  shares one namespace and diagnoses duplicates. Reachability marks the
  same names.
- `sema/ms_asm.rs` folds each operand into a linear value (one symbol,
  displacement, up to two registers with one scale, bracket flag, `PTR`
  size, segment, `OFFSET`, `SHORT`) and classifies it as register,
  immediate, or reference (`MsAsmReference`). Objects become `Place`s via
  `place()`; functions and labels stay symbols; enumerators and
  `TYPE`/`LENGTH`/`SIZE` become constants. `.field` needs a typed operand
  and adds the offset. `x[i]` and `x + i` add bytes. Lowering is in the
  same file.
- A register-only reference typed by a field (`mov [ebx].f, 1`) gets a
  `ptr` size like an object reference.

Compiler disagreements:

- `TYPE s.f`: MSVC gives `TYPE s + offsetof(f)`; we follow clang (field
  size).
- MSVC-only (msvc flavor accepts, clang flavor rejects):
  - `.f` on an untyped operand (`[ebx].f`) or a pointer/scalar (`g.f`;
    C4537 for locals) searches every struct/union visible at that point,
    including function-local tags. Adds the offset; the field type becomes
    the operand type. Two matches (even at the same offset; anonymous
    members count in both records) is C2410; none is C2411. A struct-typed
    operand needs the field in that struct. `TypeResolver::ms_asm_field`.
  - `TYPE` on one C type keyword: `char`/`__int8` 1, `short` 2, `int`/`long`
    4, `__int64` 8, `float` 4, `double` 8, lone `signed`/`unsigned` 1.
    Two-word types, `void`, `_Bool`, `wchar_t`, and `SIZE`/`LENGTH` on a
    keyword are errors.
- `TYPE T` on a typedef: accepted in both flavors.
- Two C symbols in one operand (`s + g`): rejected (C2424 in MSVC too).
- `offset local`: clang accepts, MSVC rejects (C2415); we accept.

### IR

`ir::InlineAsm` with `dialect = Some(Intel)`; node in
[ir/asm](ir/asm.md#msvc-__asm), printing in [ir-grammar](ir-grammar.md).

- `AsmOperandKind::Memory { place, access: Read | Write | ReadWrite }`: one
  operand per object, access joined over all references. In Rust it is
  `in(reg) &raw ... x` (or `sym`), so `access` only decides mutability.
- Function names are `Symbol` operands; `offset x` is an `In` of `x`'s
  address.
- `AsmPiece::Address { operand, displacement, base, index, size }` at each
  C-object reference; Slate renders `dword ptr [{x} + 4]`, `[{arr} + ebx]`,
  or a frame-struct base. Register-only references (`[esp+4]`) stay text,
  immediates in decimal.
- `size` is set when explicit, when the instruction has no register
  operand, or for `movzx`/`movsx`, shifts/rotates by `cl`, `shld`/`shrd`.
  Not always, or `movdqu xmm0, int_array` gets `dword`.
- Asm-local labels are lowercased `LocalLabel` pieces; Slate renumbers to
  `N:` / `Nf` / `Nb` (Rust forbids named labels).
- The quoted template is rebuilt from the AST (numbers decimal).
- Options: `volatile`, `memory = Any`, `nostack = false`,
  `preserves_flags = false`, `may_unwind = false`; `None` in a naked
  function.

### Effects table

`src/sema/ms_asm_effects.rs`, hand-curated and conservative (a spurious
write only costs a `mut` or clobber).

- Operand 0 is read+write, unless on the read-only list (`cmp`, `test`,
  `bt`, `push`, `j*`, `call`, `out`, one-operand `mul`/`imul`/`div`/`idiv`,
  `comis*`/`ucomis*`, `ptest`, `ldmxcsr`, `prefetch*`, …) or write-only
  list (`mov`, `lea`, `pop`, `set*`, `in`, `movd`/`movq`/`movdq*`/`movap*`,
  `cvt*`, `stmxcsr`, …). x87 operands are reads unless the mnemonic stores
  (`fst`, `fstp`, `fist*`, `fbstp`, `f(n)stsw`, `f(n)stcw`, `f(n)stenv`,
  `f(n)save`, `fxsave`). Later operands are reads; `xchg`/`xadd` write
  both.
- Written registers are clobbers, widened to 32-bit (`al`/`ah`/`ax` →
  `eax`). Never `esp` (`nostack = false`) or flags
  (`preserves_flags = false`).
- Implicit defs: EDX:EAX for `mul`/`div`/`idiv`/one-operand `imul`,
  `cwd`/`cdq`, `rdtsc`/`rdpmc`/`rdmsr`/`xgetbv`, `cmpxchg8b`; EAX for
  `cbw`/`cwde`, `lahf`, `xlat`, `cmpxchg`; `rdtscp`; `cpuid`; ECX for
  `loop*`; EBP for `enter`/`leave`; `popa(d)`; operand-less string ops
  (`movs*`/`cmps*` ESI/EDI, `stos*`/`scas*`/`ins*` EDI, `lods*` EAX/ESI,
  `outs*` ESI) plus ECX under `rep`; `mm0`..`mm7` for `emms`/`femms`.
- Any `f*` or `emms` clobbers `st`..`st(7)`.
- `call` clobbers EAX, ECX, EDX.
- Equal to or a superset of clang (`sema/i686-pc-windows-msvc/ms_asm_effects.c`).
  clang misses ECX for `rep`/`loop`, EAX for `xlat`, EBP for `enter`, the
  write in `xchg eax, x`, the x87 stack, and `call` clobbers; it marks
  `push x` as a write and lists `rdtsc` as `rax`/`rdx`.

### Implicit return

- i686 only: every MS asm in a non-naked function returning an integer or
  pointer ≤ 8 bytes captures EAX, plus EDX above 4 bytes, even if a later
  explicit return makes the end unreachable. Float, aggregate, and x87
  returns are not handled. clang x86_64 captures nothing.
- `src/sema/ms_asm_return.rs` allocates shared synthetic `u32` locals at
  the first asm; each asm gets explicit-register `Out` operands before its
  source operands. A written return register is an early output (clobber
  removed), else late; each half has its own direction (clang's `A` marks
  both early).
- `Fallthrough::Return` reads the locals, builds `u64(eax) | (u64(edx) <<
  32)` for two registers, and converts: narrow integers truncate, `_Bool`
  keeps the low bit, enums via underlying type, pointers via `IntToPtr`.
  Evaluated only at the end, so explicit returns and loops need no
  rewriting. Noreturn overrides with `ub`. C99 `main` zero-initializes its
  capture.
- Oracle: `sema/i686-pc-windows-msvc/ms_asm_return.h` with clang
  `-fms-extensions -S -emit-llvm`. Both flavors have fixtures; an x86_64
  clang fixture checks no outputs are added.

## Rust-side notes for Slate

- i686 `asm!` has about five allocatable GPRs; `esi`/`ebp` are reserved.
  Blocks using them need a `push`/`pop` wrapper (legal, `nostack` is off).
  Globals as `sym` cost no register; locals cost one each, or one total
  with a frame struct.
- `_emit` byte sequences (hand-encoded `rdtsc`, `cpuid`) get no inferred
  clobbers; clang has the same hole.
