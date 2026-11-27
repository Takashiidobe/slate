# GCC Assembly

Use `--flavor=gcc` and the target from the original build. GCC mode accepts GNU
`asm` and `__asm__`, including extended asm with operands at file scope.
File-scope Rust output still requires constant or symbol operands.

GCC mode accepts hard-register constraints such as `"{eax}"` on x86. An input
can match different outputs in different alternatives. Slate ties the input to
the output selected by the chosen alternative. A memory-only constraint on a
register variable is rejected.

## x86-64

See [x86 operand choices](index.md#x86-operand-choices) for constraints and
template modifiers shared with Clang.

- `-masm=att` selects AT&T syntax, the default. `-masm=intel` selects Intel
  syntax, including the Intel branch of a `{att|intel}` template.
- `r` and `q` accept general registers. `R` restricts the operand to legacy
  registers. `a`, `b`, `c`, and `d` select fixed registers.
- `A` uses `rax` for word-sized values and the `rdx:rax` pair for a double-width
  value.
- `x` selects an XMM, YMM, or ZMM class according to operand width. `v` reaches
  the extended vector register set, and `k` selects a mask register. Preserve
  the AVX and AVX-512 flags for wider classes.
- `=@ccCOND`, for example `=@cce`, exports a condition-code result. Slate emits
  a `setcc` instruction to record it.
- `%a` on an eligible symbol can emit a RIP-relative reference. Symbol
  eligibility depends on linkage and PIC or PIE settings.

### Pin an operand to EAX

Run `slate translate --flavor=gcc --target=x86_64-unknown-linux-gnu example.c`
on this input.

```c
int add_three(int value) {
    __asm__("addl $3, %0" : "+{eax}"(value));
    return value;
}
```

**Generated Rust**

```rust
fn add_three(mut value: i32) -> i32 {
    unsafe {
        core::arch::asm!(
            "addl $3, %eax",
            inlateout("ax") value => value,
            options(pure, nomem, nostack, att_syntax),
        );
    }
    value
}
```

## i686

See [x86 operand choices](index.md#x86-operand-choices) for the shared forms.

- Use an i686 target or select its 32-bit variant with `-m32` where available.
  `q` restricts byte-register operands to the `a`, `b`, `c`, and `d` register
  families. `R` selects a general register on i686.
- `A` can carry a 64-bit value in `edx:eax`. `%b`, `%h`, `%w`, and `%k` name low
  byte, high byte, 16-bit, and 32-bit register views.
- `=@ccCOND` captures an x86 condition-code result as on x86-64.
- Some x87 and MMX operands use memory temporaries in generated Rust. Absolute
  symbol references through `%a` are a lowering barrier on i686.

### Capture an equality flag

Run `slate translate --flavor=gcc --target=i686-unknown-linux-gnu example.c` on
this input.

```c
int equal(int left, int right) {
    int result;
    __asm__("cmpl %2, %1" : "=@cce"(result) : "r"(left), "r"(right));
    return result;
}
```

**Generated Rust**

```rust
fn equal(left: i32, right: i32) -> i32 {
    let mut result: i32 = 0;
    {
        let __t0: u8;
        unsafe {
            core::arch::asm!(
                "cmpl {2:e}, {1:e}\nsete {0}",
                lateout(reg_byte) __t0,
                in(reg) left as u32,
                in(reg) right as u32,
                options(pure, nomem, nostack, att_syntax),
            );
        }
        result = unsafe { core::mem::transmute::<u32, i32>(__t0 as u32) };
    }
    result
}
```

## AArch64

See [AArch64 operand choices](index.md#aarch64-operand-choices) for the
register, immediate, and clobber lists.

- `r` selects a core register. Use `%w0` for its 32-bit view and `%x0` for its
  64-bit view.
- `w` selects an FP or vector register. `x` and `y` restrict it to `v0`–`v15`
  and `v0`–`v7`, respectively.
- `I`–`N` select target immediate classes. `Z` accepts zero. For a zero selected
  as an immediate, `%w` and `%x` can emit `wzr` and `xzr`.
- `Q` selects memory addressed through one base register. `S` selects a symbol,
  including eligible `:lo12:` references.
- `=@ccCOND` captures an Arm condition result using `cset`.
- For SVE templates, preserve the function's SVE feature. GCC mode accepts `ffr`
  clobbers. A `vg` clobber is a lowering barrier when SVE or SME state may be
  live.

### Use a 32-bit logical immediate

Run `slate translate --flavor=gcc --target=aarch64-unknown-linux-gnu example.c`
on this input.

```c
int low_byte(int value) {
    __asm__("and %w0, %w0, %1" : "+r"(value) : "K"(0xff));
    return value;
}
```

**Generated Rust**

```rust
fn low_byte(mut value: i32) -> i32 {
    unsafe {
        core::arch::asm!(
            "and {0:w}, {0:w}, {1}",
            inlateout(reg) value => value,
            const 255i32,
            options(pure, nomem, nostack, preserves_flags),
        );
    }
    value
}
```

## Arm32

The covered Arm32 examples use Armv7 in ARM state. Preserve the original `-marm`
or `-mthumb` setting and float ABI flags.

See [Arm32 operand choices](index.md#arm32-operand-choices) for the shared
constraints and template modifiers.

- `r` and `l` select core registers in ARM state. A 64-bit operand can use a
  pair. `%Q` selects the low register, and `%R` or `%H` selects the high
  register.
- `t`, `w`, and `x` select supported VFP or NEON register subsets according to
  operand width. Use `%P` for a double register and `%q` for a quad register.
  GCC's unmodified single-register view of a wider operand can stop lowering.
- `Q` and supported `U` constraints describe memory operands. `%m` prints the
  base register without the surrounding brackets.
- `=@ccCOND` captures an Arm condition result. GCC mode rejects the read-write
  form `+@ccCOND` on Arm32.
- Supported NEON quad-register operands require the NEON feature on the
  function. Clobbering `sp`, `pc`, or `vfpcc` stops lowering.

### Capture a condition code in ARM state

Run
`slate translate --flavor=gcc --target=armv7-unknown-linux-gnueabihf -marm example.c`
on this input.

```c
int nonzero(int value) {
    int result;
    __asm__("cmp %1, #0" : "=@ccne"(result) : "r"(value));
    return result;
}
```

**Generated Rust**

```rust
fn nonzero(value: i32) -> i32 {
    let mut result: i32 = 0;
    {
        let __t0: u32;
        unsafe {
            core::arch::asm!(
                "cmp {1}, #0\nmov {0}, #0\nmovne {0}, #1",
                lateout(reg) __t0,
                in(reg) value as u32,
                options(pure, nomem, nostack),
            );
        }
        result = unsafe { core::mem::transmute::<u32, i32>(__t0 as u32) };
    }
    result
}
```
