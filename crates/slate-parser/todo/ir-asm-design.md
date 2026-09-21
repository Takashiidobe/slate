# IR asm design

The IR should sit between GCC/Clang extended asm and Rust `asm!`/`naked_asm!`.

**The body stays text.** Rust `asm!` is itself a template-with-placeholders
format, so translating `"movl 8(%[base],%[index],4), %[out]"` into
`"movl 8({base},{index},4), {out:e}"` is a substitution at the operand-reference
sites. Parsing the body into a typed instruction IR and re-rendering it returns
the same string while acquiring a per-target assembler and a large `Opaque`
escape hatch. `AsmPiece::{Text, Operand}` is already the right shape.

**The operands are what need structure.** Everything the Rust backend has to
decide should be decided once during lowering.

## Operands

One list in GCC numbering order (outputs then inputs), so
`AsmPiece::Operand { index }` is a direct subscript. Each entry carries:

```text
operand {
    name       = [name]          // %[name], if given
    direction  = in | out | lateout | inout | inlateout
    class      = reg | reg_abcd | xmm_reg | ... | mem | imm | explicit(rax)
    width      = 8 | 16 | 32 | 64 | ...
    modifier   = <explicit GCC modifier char, if any>
    value      = <Value for reads, Place for writes>
}
```

`direction` folds the `=`/`+`/`&` modifiers, and the mapping is inverted from
the naive reading — GCC's plain `=` lets the allocator reuse an input's
register, which is Rust's *late* form:

```text
"=r"  -> lateout        "=&r" -> out
"+r"  -> inlateout      "+&r" -> inout
```

A `Matching(n)` tie plus its separate input entry is one `inout` operand;
collapse it here. `%` (commutative) and `-` (pic) stay as separate flags, they
are not direction.

`class` replaces the raw constraint letters: `r`→`reg`, `q`→`reg_abcd`,
`x`→`xmm_reg`, `a`→`explicit(rax)`, `i`→`imm`, `m`→`mem`. Keep the original
letters for diagnostics and as a fallback. Multi-alternative constraints
(`"r,m"`) have no Rust equivalent, so pick one alternative here and keep the
rejected ones.

`width` is not optional. In AT&T, `%0` for an `int` prints `%eax`, but Rust's
`{0}` for `in(reg)` always prints the full-width register — without an explicit
`:e`/`:x`/`:r` modifier derived from the width, the emitted asm silently uses
the wrong register size.

A `mem` operand is an address, not a loaded value: `in(reg) &raw mut x`, with
the reference site rewritten from `{x}` to `({x})`.

## Node

```text
asm x86 att {
    options { volatile, nomem, preserves_flags, ... }

    operands {
        %dst: i32 = inlateout reg    { place = x }
        %src: i32 = in        reg    { value = y }
        %lo:  u64 = inlateout rax    { place = lo }
    }

    clobbers { memory, flags, reg(rcx) }

    body { "addl ", %src, ", ", %dst }
}
```

Target and dialect belong on the node: they decide `options(att_syntax)`, the
register-class mapping, and per-target clobber rules. One such rule: on x86,
GCC treats flags as clobbered by extended asm whether or not `"cc"` was
written, so `preserves_flags` can never be inferred there.

Rust `asm!` options (`nomem`, `readonly`, `pure`, `nostack`, `preserves_flags`)
are computed here from the volatile qualifier and the clobber list.

## naked

`naked_asm!` selection belongs on the *function*, not the asm statement: the
condition is that the function is `__attribute__((naked))` and its body is a
single basic asm. Record it as an IR function kind.

## Frontends

MSVC `__asm` is the one frontend that must read instructions, because it
supplies no constraints and reads/writes/clobbers have to be inferred. Even
there the job is identifier resolution (which tokens are C places), the set of
registers mentioned, and a flags/implicit-def table — not a typed instruction
IR. It feeds this same operand model. See `msvc-asm-design.md`.

Out of scope: the MASM/`ml64` PROC/directive/unwind layer. That is a standalone
assembler file format, not C.

Tracked as `slate-parser-25m.10` through `.18`.
