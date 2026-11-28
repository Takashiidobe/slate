# Windows Assembly

Microsoft-style `__asm` blocks use Intel syntax and name C variables directly.
Slate infers memory operands and register clobbers from the instructions. Clang
mode can also translate GNU extended asm on Windows, where operands and clobbers
are explicit.

Save this as `copy.c` and run
`slate translate --flavor=msvc --target=i686-pc-windows-msvc copy.c`.

```c
int copy_value(int value) {
    int out;
    __asm {
        mov eax, value
        mov out, eax
    }
    return out;
}
```

**Generated Rust**

```rust
mod __slate_oldnames {
    #[link(name = "oldnames")]
    unsafe extern "C" {}
}

fn copy_value(mut value: i32) -> i32 {
    let mut __v0: u32 = 0;
    let mut out: i32 = 0;
    unsafe {
        core::arch::asm!(
            "mov eax, [{0}]\nmov [{1}], eax",
            in(reg) core::ptr::addr_of!(value),
            in(reg) core::ptr::addr_of_mut!(out),
            out("ax") __v0,
        );
    }
    out
}
```

## i686

- MSVC mode accepts `__asm` and `_asm` blocks or single-line instructions. Clang
  mode also accepts them with Microsoft extensions, enabled by default for
  Windows MSVC targets.
- C object names denote memory. An index such as `arr[4]` means a four-byte
  displacement. A field such as `s.member` uses the resolved field offset. An
  explicit size such as `dword ptr` controls the instruction's access width.
- `TYPE`, `LENGTH`, and `SIZE` compute layout values for supported operands.
  `OFFSET` requests an address. A semicolon starts a comment through the end of
  the line.
- Local labels work within a block. MSVC mode also supports jumps between C and
  assembly labels within a function. Clang mode rejects those cross-language
  jumps.
- When a supported integer or pointer function falls through after an asm block,
  Slate captures the result from `eax`, or `edx:eax` for a 64-bit result.
  Floating-point and aggregate implicit returns are outside the lowering
  support.

### Operand and instruction choices

- `name` uses the memory of a C object, or a symbol when the name denotes a
  function. For example, `call helper` calls a C function by symbol.
- `arr[4]` adds four bytes to the address of `arr`.
- `s.field` adds the field's byte offset. MSVC mode can also resolve an untyped
  `[reg].field` when the field is unique among visible records.
- `TYPE value` obtains the element size. `TYPE` on a supported C type keyword,
  such as `TYPE int`, is an MSVC-mode form.
- `LENGTH value` obtains the first-level array element count.
- `SIZE value` obtains the total size.
- `OFFSET value` supplies an address. Slate also accepts supported local
  addresses.
- `PTR` with a size, as in `byte ptr [eax]` or `dword ptr [eax]`, supplies an
  explicit memory operand width. Slate can infer the width from a C operand when
  the instruction needs it.
- `SHORT label` accepts the short-branch operand spelling.
- `; comment` consumes the rest of the line. A macro that expands to multiple
  instructions can use `__asm` between instructions.
- `_emit N` emits a byte. Register effects of emitted bytes are left implicit.

### Labels, clobbers, and results

- A label such as `done:` can be a branch target within the block. Slate emits
  numeric local labels in Rust.
- MSVC mode supports `jmp c_label` to a C label and `goto asm_label` from C into
  an asm block. Clang mode rejects both forms.
- Written subregisters count as writes to the corresponding full register. For
  example, a write to `al` clobbers `eax`.
- Implicit instruction effects also contribute clobbers. For example,
  one-operand `mul` can write `edx:eax`, and a `call` can clobber `eax`, `ecx`,
  and `edx`.
- A block can access memory and the stack. Slate treats its effects
  conservatively when it emits `asm!`.

## x86-64

- MSVC mode rejects `__asm` blocks on x86-64.
- Clang mode accepts Microsoft-style blocks on x86-64 with Microsoft extensions
  or `-fasm-blocks`. Return values need explicit C returns.
- Clang mode also accepts GNU extended asm. Select Intel syntax for those
  templates with `-masm=intel` when the original build uses it.

### Use a Microsoft-style block with Clang

Run `slate translate --flavor=clang --target=x86_64-pc-windows-msvc example.c`
on this input.

```c
int increment(int value) {
    int out;
    __asm {
        mov eax, value
        inc eax
        mov out, eax
    }
    return out;
}
```

**Generated Rust**

```rust
mod __slate_oldnames {
    #[link(name = "oldnames")]
    unsafe extern "C" {}
}

fn increment(mut value: i32) -> i32 {
    let mut out: i32 = 0;
    unsafe {
        core::arch::asm!(
            "mov eax, [{0}]\ninc eax\nmov [{1}], eax",
            in(reg) core::ptr::addr_of!(value),
            in(reg) core::ptr::addr_of_mut!(out),
            out("ax") _,
        );
    }
    out
}
```

## AArch64

- MSVC mode rejects `__asm` blocks on AArch64. Clang's Microsoft-style blocks
  are also restricted to x86 targets.
- Clang mode accepts GNU asm for Windows AArch64. See
  [Clang's AArch64 constraints](clang.md#aarch64).

### Use a GNU register operand with Clang

Run `slate translate --flavor=clang --target=aarch64-pc-windows-msvc example.c`
on this input.

```c
int increment(int value) {
    __asm__("add %w0, %w0, #1" : "+r"(value));
    return value;
}
```

**Generated Rust**

```rust
mod __slate_oldnames {
    #[link(name = "oldnames")]
    unsafe extern "C" {}
}

fn increment(mut value: i32) -> i32 {
    unsafe {
        core::arch::asm!(
            "add {0:w}, {0:w}, #1",
            inlateout(reg) value => value,
            options(pure, nomem, nostack, preserves_flags),
        );
    }
    value
}
```

## Arm32

- MSVC mode rejects `__asm` blocks on the `thumbv7a-pc-windows-msvc` target.
- Clang mode accepts GNU asm on that target. Keep its Thumb instruction encoding
  and hard-float ABI when translating the original command.

### Use GNU asm on the Windows Thumb target

Run `slate translate --flavor=clang --target=thumbv7a-pc-windows-msvc example.c`
on this input.

```c
int increment(int value) {
    __asm__("add %0, %0, #1" : "+r"(value));
    return value;
}
```

**Generated Rust**

```rust
mod __slate_oldnames {
    #[link(name = "oldnames")]
    unsafe extern "C" {}
}

fn increment(mut value: i32) -> i32 {
    unsafe {
        core::arch::asm!(
            "add {0}, {0}, #1",
            inlateout(reg) value => value,
            options(pure, nomem, nostack, preserves_flags),
        );
    }
    value
}
```

## Clang asm block flags

- `-fms-extensions` enables Microsoft-style blocks on x86, including non-Windows
  x86 targets.
- `-fasm-blocks` enables those blocks independently of other Microsoft
  extensions.
- `-fno-asm-blocks` clears the independent setting. Microsoft extensions can
  still enable the blocks.

Slate handles inline instructions and supported MASM-style operands. For
instruction-shaped operations available as intrinsics, see
[Microsoft intrinsics](../builtins/microsoft.md).
