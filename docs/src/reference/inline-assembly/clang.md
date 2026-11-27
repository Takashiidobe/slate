# Clang Assembly

Use `--flavor=clang` and the target and ISA flags from the original build. Clang
mode accepts GNU `asm` in functions and basic asm at file scope.
`-fno-gnu-inline-asm` disables both GNU statement forms while leaving asm labels
and register variable declarations available.

Clang mode rejects GCC's `{reg}` hard-register constraint and inputs that tie to
different outputs across alternatives. A C register variable used as an operand
can select its named register where the constraint permits it.

## x86-64

See [x86 operand choices](index.md#x86-operand-choices) for constraints and
template modifiers shared with GCC.

- `-masm=att` selects AT&T syntax, the default. `-masm=intel` selects Intel
  syntax and the Intel branch of `{att|intel}` templates.
- `r` and `q` accept general registers. `R` selects legacy registers. Fixed
  constraints such as `a` and `c` select their associated register families.
- `x`, `v`, and `k` cover vector and mask operands. `Yi`, `Yt`, and `Y2` select
  XMM-class operands in Clang mode. YMM and ZMM operands require the
  corresponding function target features.
- `=@ccCOND` captures an x86 condition-code result. Clang also accepts
  `+@ccCOND` for supported operands. The condition comes from the flags after
  the template executes.
- `m` refers to object storage. With an eligible non-preemptible static and
  constant offset, Slate can use a RIP-relative symbol reference.

### Select an Intel template

Run
`slate translate --flavor=clang --target=x86_64-unknown-linux-gnu -masm=intel example.c`
on this input.

```c
int add_three(int value) {
    __asm__("add %0, %1" : "+r"(value) : "i"(3) : "cc");
    return value;
}
```

**Generated Rust**

```rust
fn add_three(mut value: i32) -> i32 {
    unsafe {
        core::arch::asm!(
            "add {0:e}, {1}",
            inlateout(reg) value => value,
            const 3i32,
            options(pure, nomem, nostack),
        );
    }
    value
}
```

## i686

See [x86 operand choices](index.md#x86-operand-choices) for the shared forms.

- `q` restricts byte-register operands to the `a`, `b`, `c`, and `d` register
  families. `R` accepts a general register, and `A` can use `edx:eax` for 64-bit
  values.
- `%b`, `%h`, `%w`, and `%k` select low byte, high byte, 16-bit, and 32-bit
  register views. Clang rejects tie constraints with incompatible widths.
- Some x87 or MMX operands lower through memory temporaries. A symbol under `%a`
  can require an absolute relocation that Slate rejects on i686.
- Microsoft-style `__asm` blocks are available with Microsoft extensions or
  `-fasm-blocks`. See [Windows Assembly](windows.md) for block rules, including
  their use on non-Windows x86 targets.

### Tie the input to the output

Run `slate translate --flavor=clang --target=i686-unknown-linux-gnu example.c`
on this input.

```c
int add_three(int value) {
    int result;
    __asm__("addl $3, %0" : "=r"(result) : "0"(value) : "cc");
    return result;
}
```

**Generated Rust**

```rust
fn add_three(value: i32) -> i32 {
    let mut result: i32 = 0;
    unsafe {
        core::arch::asm!(
            "addl $3, {0:e}",
            inlateout(reg) value as u32 => result,
            options(pure, nomem, nostack, att_syntax),
        );
    }
    result
}
```

## AArch64

See [AArch64 operand choices](index.md#aarch64-operand-choices) for the
register, immediate, and clobber lists.

- `r` selects core registers. `%w` and `%x` select their 32-bit and 64-bit
  views.
- `w` selects an FP or vector register. `x` and `y` restrict it to the first 16
  and first 8 vector registers. Use `%s`, `%d`, or `%q` for the relevant FP or
  vector view.
- `I`–`N` select target immediate classes. `Z` accepts zero. `Q` addresses
  memory through a base register, and `S` selects a symbol.
- `=@ccCOND` captures a condition result using `cset`. The Clang-only `+@ccCOND`
  form also sets NZCV from the input. A `%w` reference to the flags operand
  names NZCV.
- Supported SVE vector and predicate clobbers require the appropriate target
  feature. Clang mode rejects the `ffr` clobber.

### Read and write NZCV

Run
`slate translate --flavor=clang --target=aarch64-unknown-linux-gnu example.c` on
this input.

```c
unsigned long was_equal(unsigned long flags) {
    __asm__("nop" : "+@cceq"(flags));
    return flags;
}
```

**Generated Rust**

```rust
fn was_equal(mut flags: u64) -> u64 {
    {
        let __t0: u32;
        unsafe {
            core::arch::asm!(
                "msr nzcv, {1:x}\nnop\ncset {0:w}, eq",
                out(reg) __t0,
                in(reg) flags as u64,
                options(pure, nomem, nostack),
            );
        }
        flags = unsafe { core::mem::transmute::<u64, u64>(__t0 as u64) };
    }
    flags
}
```

## Arm32

See [Arm32 operand choices](index.md#arm32-operand-choices) for the shared
constraints and template modifiers.

- `r` selects a core register. In ARM state, `l` also accepts general registers.
  A 64-bit core operand can use a low and high pair with `%Q` and `%R` or `%H`.
- `t`, `w`, and `x` select supported VFP and NEON subsets by width.
  Quad-register operands require the NEON feature on the function.
- `Q` and supported `U` constraints express memory operands. Target immediate
  constraints require foldable constants. `%c` omits the usual immediate prefix.
- `asm goto` supports C label operands. Clang mode rejects `@cc` flag
  constraints on Arm32.

### Read the high half of a register pair

Run
`slate translate --flavor=clang --target=armv7-unknown-linux-gnueabihf -marm example.c`
on this input.

```c
unsigned int high_half(unsigned long long value) {
    unsigned int result;
    __asm__("mov %0, %H1" : "=r"(result) : "r"(value));
    return result;
}
```

**Generated Rust**

```rust
fn high_half(value: u64) -> u32 {
    let mut result: u32 = 0;
    {
        let __t0: u64 = value as u64;
        unsafe {
            core::arch::asm!(
                "mov {0}, r1",
                lateout(reg) result,
                in("r0") __t0 as u32,
                in("r1") (__t0 >> 32) as u32,
                options(pure, nomem, nostack, preserves_flags),
            );
        }
    }
    result
}
```
