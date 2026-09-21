# MSVC Inline ASM design

> Scope (see `ir-asm-design.md`): MSVC `__asm` is the one frontend that must
> read instructions, because it supplies no constraints. It needs identifier
> resolution, the set of registers mentioned, and a flags/implicit-def table —
> not the typed instruction IR sketched below; it feeds the shared *operand*
> model. The MASM/`ml64` PROC/directive/unwind sections are out of scope: that
> is a standalone assembler file format, not C.

For x86-32 MSVC `__asm`, treat it as a separate frontend syntax that lowers into the same shared asm IR as GCC/Clang. Unlike GCC asm, there are no explicit constraints: the asm body directly references C variables, registers, labels, and symbols, so the frontend has to resolve identifiers against C scope and infer reads/writes/clobbers from the parsed instructions.

```c
// MSVC x86
int f(int x, int y) {
    __asm {
        mov eax, x
        add eax, y
        mov x, eax
    }
    return x;
}
```

```text
// IR
asm x86 intel {
    bindings {
        %x: i32 = inout place(x)
        %y: i32 = in    place(y)
    }

    body {
        mov.i32 eax, load(%x)
        add.i32 eax, load(%y)
        mov.i32 store(%x), eax
    }
}
```

```rust
// Rust
asm!(
    "mov eax, dword ptr [{x}]",
    "add eax, dword ptr [{y}]",
    "mov dword ptr [{x}], eax",
    x = in(reg) &raw mut x,
    y = in(reg) &raw const y,
    out("eax") _,
);
```

So the x86-32 frontend mostly maps textual constructs into semantic ones:

```text
MSVC source          -> IR

eax                  -> reg(eax)
x                    -> c_place(x)
obj.field            -> field(c_place(obj), field)
[ebx + ecx*4 + 8]    -> mem(base=ebx, index=ecx, scale=4, disp=8)
DWORD PTR [...]      -> mem(width=32, ...)
call foo             -> call(sym(foo))
label:               -> label(label)
jmp label            -> branch(label)
```

The instruction parser should also infer effects:

```text
add eax, y
    -> reads  eax
    -> reads  place(y)
    -> writes eax
    -> writes flags
```

That lets you reconstruct Rust-facing operands and clobbers even though MSVC never supplied `"r"`, `"+r"`, `"cc"`, etc.

For x86-64, MSVC itself has no inline `__asm`; MASM/`ml64` is effectively a standalone assembly language. Treat that as a second frontend feeding the same instruction IR, but include procedure/directive nodes because MASM contains more than instructions.

```asm
add2 PROC
    mov rax, rcx
    add rax, rdx
    ret
add2 ENDP
```

```text
// IR
asm_function add2 x86_64 abi(win64) naked {
    params {
        %a = abi_reg(rcx)
        %b = abi_reg(rdx)
    }

    returns {
        %ret = abi_reg(rax)
    }

    body {
        mov.u64 rax, rcx
        add.u64 rax, rdx
        ret
    }
}
```

```rust
#[unsafe(naked)]
unsafe extern "win64" fn add2(a: u64, b: u64) -> u64 {
    naked_asm!(
        "mov rax, rcx",
        "add rax, rdx",
        "ret",
    );
}
```

MASM directives should be separate IR nodes rather than pretending they are instructions:

```asm
foo PROC
    push rbp
    .PUSHREG rbp
    sub rsp, 32
    .ALLOCSTACK 32
    .ENDPROLOG
    ret
foo ENDP
```

```text
asm_function foo abi(win64) naked {
    body {
        push rbp
        unwind push_nonvolatile(rbp)

        sub rsp, 32
        unwind alloc_stack(32)

        unwind end_prologue
        ret
    }
}
```

The overall split is then:

```text
MSVC x86 __asm
    C symbol resolution
    + Intel/MASM-like instruction parser
        ↓
    shared AsmUnit
        ↓
    asm! / naked_asm!


MASM x86_64
    PROC/directive/macro layer
    + instruction parser
        ↓
    shared AsmUnit
        ↓
    naked_asm! / global_asm! / external .asm
```

The shared IR can stay small: `Instruction`, `Reg`, `Imm`, `Mem`, `CPlace`, `Symbol`, `Label`, `Directive`, `Unwind`, and `Opaque`. The x86-32 path mainly reconstructs missing operand effects; the x86-64 MASM path mainly preserves procedure/directive structure while normalizing instructions.
