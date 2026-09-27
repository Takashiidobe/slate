# MSVC inline asm

MSVC `__asm` is the one asm frontend that has to read instructions. It
supplies no constraints, so the frontend must infer reads, writes and
clobbers itself. The instruction body still stays text: it lowers into the
same `ir::InlineAsm` as GNU asm (`dialect = Intel`), and only the operand
model and a few operand-reference pieces are structured
(`todo/ir-asm-design.md`, decision `slate-parser-25m.18`).

Out of scope: the MASM/`ml64` `PROC`/directive/unwind layer. That is a
standalone assembler file format, not C.

Tracked as `slate-parser-25m.6` and its children. Example inputs:
`todo/msvc-asm-example.c`.

## Why it matters

i686 `winnt.h` (`_M_IX86` branch, line 1064 in the xwin sysroot) defines
`Int64ShllMod32`, `Int64ShraMod32` and `Int64ShrlMod32` with `__asm { }`
bodies and `DbgRaiseAssertionFailure` with `__asm int 0x2c`, so
`#include <windows.h>` on `i686-pc-windows-msvc` fails in both flavors until
this lands (`slate-parser-g097`). Those functions exercise most of the hard
semantics at once:

```c
#pragma warning(disable:4035)   // "no return value"
__inline ULONGLONG Int64ShllMod32(ULONGLONG Value, DWORD ShiftCount) {
    __asm {
        mov     ecx, ShiftCount
        mov     eax, dword ptr [Value]
        mov     edx, dword ptr [Value+4]   // byte offset into a u64
        shld    edx, eax, cl
        shl     eax, cl
    }                                      // EDX:EAX is the return value
}
```

## Oracle behavior

Verified with clang 22.1.8 (`-target i686-pc-windows-msvc -emit-llvm`) and
`tools/cl.exe` (`MSVC_ARCH=x86`, `/FAs` listings). MSVC and clang agree on
everything below unless noted.

| Source | Meaning |
|---|---|
| `mov eax, x` | `x` is a memory operand: the asm gets the variable's address. clang `*m` for a read, `=*m` for a write. |
| `arr[4]` | **byte** offset 4, not element 4 (MSVC `_arr+4`, clang `$6[$$4]`). |
| `s.b` | field byte offset (MSVC `_s$[ebp+4]`). |
| `TYPE arr` / `LENGTH arr` / `SIZE arr` | folded from the C type: element size, element count, total size (4 / 4 / 16 for `int[4]`). Only the first array level counts: `int m[2][3]` gives 12 / 2 / 24. |
| `[eax]T.f` | `T` a typedef of a struct: `[eax + offsetof(T, f)]`. A struct tag does not work in clang. |
| `jmp x` with a C name `x` and an asm label `x` | the C name wins. |
| `100h`, `0Ch`, `0x2c`, `10b`, `17o`, `010`, `1u` | MASM radix suffixes (`h`, `b`, `o`/`q`, `d`) and C integer spellings (`0x`, leading-`0` octal, `u`/`l` suffixes) are both accepted; both compilers print decimal. clang also takes `t` and `y`, MSVC rejects them ("bad suffix on number"). |
| `mov result, 1`, `fld a`, `mul b` | size from the C type; clang inserts `dword ptr` / `qword ptr` only where the instruction is size-ambiguous. An explicit `dword ptr [Value]` on a 64-bit variable wins. |
| `call g` | direct call to the function symbol (clang `${8:P}`), not a memory operand. |
| `done:` / `jz done` | asm-local label, uniqued per asm (`L__MSASMLABEL_.${:uid}__done`). |
| falling off the end after asm | EAX, or EDX:EAX for 64-bit, is the return value. clang gives every MS asm in a register-returning function an `{eax}`/`A` output stored to the return slot (`=&` when the asm writes it). |
| clobbers | registers the asm writes, explicitly or implicitly: `push`/`pop` give `~{esp}`, `popfd` `~{dirflag}`, `mul` EDX:EAX. Flags always. clang does not model `call` as clobbering caller-saved registers. MSVC instead saves any callee-saved register the block uses (`push ebx` in the prologue). |
| attributes | always `sideeffect inteldialect`, `nounwind`. **Not** `alignstack` (an earlier note on 25m.6 said otherwise). |

Statement boundaries:

- an instruction ends at end of line, at the next `__asm`, or at `}`;
- a macro expansion is one line, so macros need `__asm` separators
  (`READ_CPUID` in the example file);
- `;` starts a comment to end of line, and a `}` inside it is ignored
  (`__asm { mov x, eax ; } comment` closes on the next line);
- a single-line `__asm` continues through further `__asm` on the same line,
  and through a following line that starts with `__asm` but not `__asm {`;
  a braced block ends at its `}` (clang; MSVC's listing is
  indistinguishable). `__asm` then `{` on the next line is a braced block.

Acceptance:

- MSVC: `__asm` and `_asm`, x86 only. x64 gives `C4235: '__asm' keyword not
  supported on this architecture`. `asm {` is not a keyword.
- clang: `__asm`, `_asm` and `asm {` on every `*-windows-msvc` triple,
  x86_64 included. Linux targets need `-fasm-blocks`.
- gcc: never.

Jumps between C and asm labels: MSVC accepts `jmp c_label` from asm and
`goto asm_label` into an asm block; clang rejects both ("use of undeclared
label", "cannot jump from this goto statement to label ... inside an inline
assembly block").

Lexing: an apostrophe in a `;` comment (`; don't`) is only a clang warning
(`-Winvalid-pp-token`) and already survives our lexer.

## Design

### Recognition and gating

In statement position, `__asm`/`_asm` not followed by `(` or a GNU qualifier
starts an MS asm statement. clang's `asm {` is not modeled: `asm` is a plain
identifier outside GNU modes, and nothing real spells MS asm that way.

- MSVC flavor: x86 (i686) only; other arches error like `C4235`.
- Clang flavor: any `*-windows-msvc` x86 triple, x86_64 included, matching
  clang (permissive). Other triples keep today's GNU-only parse.

### Parser and AST

A separate `StmtKind::MsAsm(MsAsm)`, not a variant of `GnuAsm`: the two share
nothing until IR. Adding it follows `ast-enum-touchpoints.md`.

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

- Line breaks come from the expansion `Loc` plus the file line tables; `Span`
  carries no start-of-line flag. `Parser::mark_ms_asm_lines` rewrites the
  token stream once, before any brace matching: inside each MS asm region it
  drops `;` comments (so a `}` in a comment cannot close a function body)
  and inserts `Token::Newline` at every instruction boundary and after a
  single-line statement. `parse_ms_asm_stmt` then only reads tokens.
- The MSVC flavor off x86 errors at the statement (`C4235`); the Clang
  flavor off `*-windows-msvc` leaves `__asm` to the GNU parser, which
  rejects it like clang without `-fasm-blocks`.
- Mnemonics must accept C keyword tokens (`int 3`, `jmp short`).
- Registers, `PTR`, size names and operators are case-insensitive; C names
  are not.
- Numbers fold at parse time: MASM suffixes `h`, `b`/`y`, `o`/`q`, `d`/`t`,
  and C `0x`. A MASM hex literal is a pp-number (`0FFh`, `0Ch`).
- `_emit n` stays an instruction and lowers to `.byte n`.

### Sema: names and folding

`src/sema/names.rs` resolves each `Name` in the ordinary namespace first:
an object or parameter, a function, an enumerator or a typedef. Otherwise it
must be an asm label defined anywhere in the same function, in any `__asm`
statement, compared case-insensitively. Anything else is "unresolved label
name", which is also how both compilers word it. Asm labels live apart from
C labels (clang; MSVC shares one namespace and rejects `lbl:` in both), and
duplicates are not diagnosed (clang accepts them, MSVC rejects). Reachability
marks the same names, so header declarations used only from `__asm` survive
pruning.

`src/sema/ms_asm.rs` then folds each operand, MASM style, into a linear
value (one symbol, a displacement, up to two registers with one scale, a
bracket flag, `PTR` size, segment, `OFFSET`, `SHORT`) and classifies it as a
register, an immediate, or a reference (`MsAsmReference`). Objects become
`Place`s through the ordinary `place()` path; functions and labels stay
symbols; enumerators and `TYPE`/`LENGTH`/`SIZE` become constants. `.field`
needs a typed operand (an object or typedef) and adds the field offset.
`x[i]` and `x + i` both add bytes. The same file then lowers the result
into IR.

Where the compilers disagree, this follows clang:

- MSVC binds `TYPE` tighter than `.`: `TYPE s.f` is `TYPE s + offsetof(f)`.
  clang gives the field's size.
- MSVC accepts and clang rejects: `[eax].f` (field looked up across all
  structs), `.f` on a pointer or scalar (warning C4537, offset ignored),
  `TYPE int`, `TYPE T` on a typedef, and two symbols in one operand. We
  accept `TYPE T` and reject the rest.
- clang accepts and MSVC rejects: `offset local` (C2415). We accept.

### IR

Lowers into `ir::InlineAsm` with `dialect = Some(Intel)` (see `ir-spec.md`,
Inline asm, for the node and `ir-grammar.md` for how it prints).

- `AsmOperandKind::Memory { place, access: Read | Write | ReadWrite }`: the
  asm receives the object's address, one operand per object however often
  it is named. In Rust the operand is always `in(reg) &raw ... x` (or `sym`
  for a global), so `access` only decides `mut`-ness and whether the local
  has to be `mut`. It is `ReadWrite` until the effects table lands.
- A function name is a `Symbol` operand (`call f`); `offset x` is an `In`
  of `x`'s address, which works for locals and globals alike.
- `AsmPiece::Address { operand, displacement, base, index, size }` at each
  C-object reference. Emission renders it as `dword ptr [{x} + 4]` for a
  pointer in a register, `[{arr} + ebx]` for a `sym`, or a frame-struct base
  if Slate chooses one. Register-only memory references like `[esp+4]` stay
  text, with immediates normalized to decimal.
- `size` is set when written explicitly, or when the instruction has no
  register operand, plus a short exception list where a register does not
  fix the memory size: `movzx`/`movsx`, shifts and rotates by `cl`,
  `shld`/`shrd`. Always emitting it would break `movdqu xmm0, int_array`
  (the element type says `dword`).
- asm-local labels become `LocalLabel` pieces, lowercased, because Rust
  denies named labels in `asm!`; Slate renumbers them to `N:` / `Nf` / `Nb`.
- `_emit n` becomes `.byte n`.
- The quoted template on the node is the statement rebuilt from the AST
  (numbers in decimal), since a macro-built block has no single source
  string.
- options: `volatile`, `memory = Any`, `nostack = false`,
  `preserves_flags = false`, `may_unwind = false`; `None` in a naked
  function, as for GNU asm.

### Effects table

Hand-curated and conservative, since reading a read as a write only costs a
spurious `mut` or clobber:

- operand 0 is read+write unless the mnemonic is on a read-only list (`cmp`,
  `test`, `bt`, `push`, `comis*`/`ucomis*`, `ptest`, ...); `xchg`/`xadd`
  write both;
- implicit defs: `mul`/`imul` (1-operand)/`div`/`idiv`, `cbw`/`cwde`/`cwd`/
  `cdq`, `cpuid`, `rdtsc`/`rdtscp`, `xgetbv`, `rdmsr`, string ops and their
  `rep` forms (`esi`/`edi`/`ecx`), `loop*` (`ecx`), `xlat`,
  `cmpxchg`/`cmpxchg8b`, `in`/`out`, `fstsw ax`, `lahf`,
  `push`/`pop`/`pushad`/`popad`/`pushfd`/`popfd`/`enter`/`leave`;
- any `f*` mnemonic clobbers the x87 stack;
- every explicitly mentioned register that is written is clobbered;
- `esp` writes are not clobbers; `nostack = false` covers them.

Flags are always clobbered on x86 regardless.

### Implicit return

When the function's end is reachable, the function contains MS asm, and the
return type fits EAX or EDX:EAX (integer or pointer up to 8 bytes), lowering
adds a synthetic return local. Every MS asm in that function gets an `Out`
into it on explicit `eax` (and `edx`), and the fall-off point becomes
`return local`. This mirrors clang's return-slot store. x87 (`float`/`double`
in `st(0)`) returns are deferred.

## Rust-side notes for Slate

- i686 `asm!` has about five allocatable GPRs, and `esi`/`ebp` are reserved.
  A block that uses them needs a `push`/`pop` wrapper, which is legal
  because `nostack` is off. Globals as `sym` cost no register; each local
  costs one, or one in total with a frame struct.
- `_emit` byte sequences (hand-encoded `rdtsc`, `cpuid`) carry no inferred
  clobbers. clang has the same hole.

## Open decisions

- Effects table: hand-curated (recommended, about 60 entries) versus
  generated from LLVM's X86 tablegen.
- MSVC-only jumps between C labels and asm labels: deferred; reject until
  then.
