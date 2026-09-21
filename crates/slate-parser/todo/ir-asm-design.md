# IR asm design

The IR should sit between GCC/Clang extended asm and Rust `asm!`/`naked_asm!`. Unlike CIR, I would not leave the body as an opaque normalized string: parse just enough target assembly that the Rust backend mostly pattern-matches an instruction/template IR rather than reparsing AT&T/Intel syntax.

For a normal register read/write operand:

```c
// C
asm("addl %[src], %[dst]"
    : [dst] "+r"(x)
    : [src] "r"(y));
```

```text
// IR
asm x86 att {
    operands {
        %dst: i32 = inout reg(gpr) { value = x }
        %src: i32 = in    reg(gpr) { value = y }
    }

    body {
        add.i32 %src, %dst
    }
}
```

```rust
// Rust
asm!(
    "addl {src:e}, {dst:e}",
    src = in(reg) y,
    dst = inout(reg) x,
    options(att_syntax),
);
```

The important frontend mappings are direct: `"r"` → `reg(gpr)`, `"+r"` → `inout reg(gpr)`, `"=r"` → `out reg(gpr)`, `"=&r"` → an early-clobber/`lateout`-style semantic, fixed constraints such as `"a"` → `reg(rax)`, numeric constraints such as `"0"` → `tied_to(%0)`, and `"m"` → a memory operand rather than merely preserving the letter `m`.

```c
// C
asm("movl (%[ptr]), %[dst]"
    : [dst] "=r"(value)
    : [ptr] "r"(ptr)
    : "memory", "cc");
```

```text
// IR
asm x86 att {
    operands {
        %dst: i32 = out reg(gpr) { value = value }
        %ptr: ptr = in  reg(gpr) { value = ptr }
    }

    clobbers {
        memory
        flags
    }

    body {
        mov.i32 mem[%ptr], %dst
    }
}
```

```rust
// Rust
asm!(
    "movl ({ptr}), {dst:e}",
    ptr = in(reg) ptr,
    dst = out(reg) value,
    options(att_syntax),
);
```

The mini-parser should therefore turn template syntax into only a small number of useful nodes: `Instruction`, `OperandRef`, `Register`, `Immediate`, `Memory`, `Label`, and perhaps `Opaque`. You do not need instruction semantics comparable to LLVM MC. You mainly want to turn text like `"movl 8(%rax,%rcx,4), %edx"` into something like `mov.i32 Memory { base: rax, index: rcx, scale: 4, displacement: 8 }, Reg(edx)`, and `%[foo]`, `%0`, `%c0`, etc. into explicit operand references/modifiers.

```text
// textual asm
"movl 8(%[base],%[index],4), %[out]"
```

```text
// parsed body IR
mov.i32
    mem {
        base  = %base
        index = %index
        scale = 4
        disp  = 8
    },
    %out
```

```rust
// emitter only has to render the structured nodes
asm!(
    "movl 8({base},{index},4), {out:e}",
    base  = in(reg) base,
    index = in(reg) index,
    out   = out(reg) out,
    options(att_syntax),
);
```

I would also make constraint semantics first-class instead of retaining CIR's combined LLVM-style constraint string:

```c
asm("mulq %[rhs]"
    : "+a"(lo), "=d"(hi)
    : [rhs] "r"(rhs)
    : "cc");
```

```text
asm x86 att {
    operands {
        %lo:  u64 = inout reg(rax) { value = lo }
        %hi:  u64 = out   reg(rdx) { value = hi }
        %rhs: u64 = in    reg(gpr) { value = rhs }
    }

    clobbers { flags }

    body {
        mul.u64 %rhs
    }
}
```

```rust
asm!(
    "mulq {rhs}",
    rhs = in(reg) rhs,
    inout("rax") lo,
    lateout("rdx") hi,
    options(att_syntax),
);
```

For `naked_asm!`, keep the same parsed body IR, but separate it from C-expression operand binding. That allows the same assembler parser to handle function-body asm while the emitter chooses a different Rust surface:

```c
__attribute__((naked))
void entry(void) {
    asm volatile(
        "push %rbp\n"
        "mov %rsp, %rbp\n"
        "jmp target"
    );
}
```

```text
asm x86 att {
    kind = naked

    body {
        push reg(rbp)
        mov  reg(rsp), reg(rbp)
        jmp  symbol(target)
    }
}
```

```rust
#[unsafe(naked)]
extern "C" fn entry() {
    core::arch::naked_asm!(
        "push rbp",
        "mov rbp, rsp",
        "jmp {target}",
        target = sym target,
    );
}
```

So the core translation should be:

```text
GCC/Clang source        frontend IR                    parsed asm IR

"+r"(x)          ->     inout reg(gpr), x
"=&r"(x)         ->     out early_clobber reg(gpr), x
"a"(x)           ->     in reg(rax), x
"0"(x)           ->     in tied_to(%0), x
"m"(x)           ->     in memory(x)
"+m"(x)          ->     inout memory(x)
"cc"             ->     clobber flags
"memory"         ->     clobber memory
"%[foo]"         ->     operand reference %foo
"%c[foo]"        ->     operand reference %foo + modifier
"42"             ->     immediate 42
"%eax"           ->     physical register eax
"8(%rax,%rcx,4)" ->                                memory(base=rax,
                                                        index=rcx,
                                                        scale=4,
                                                        disp=8)
"addl %1,%0"     ->                                add.i32 %1, %0
```

That leaves the Rust backend with almost no GCC-specific knowledge: it matches `in/out/inout + register class`, renders parsed instruction operands, and chooses `asm!` versus `naked_asm!`. Keep an `OpaqueAsmFragment` fallback so unsupported directives/instructions can still round-trip without forcing the mini-parser to become a full assembler.
