# Inline Assembly

Slate translates GNU inline assembly in GCC and Clang modes and Microsoft
`__asm` blocks in the supported x86 modes. Translate with the original target,
compiler flavor, and ISA flags so the register classes and assembler dialect
match the C build.

For example, this x86 GNU extended asm adds a constant to a read-write register
operand. Save it as `add.c` and run
`slate translate --target=x86_64-unknown-linux-gnu --flavor=clang add.c`.

```c
int add_three(int value) {
    __asm__("addl %[delta], %[value]"
            : [value] "+r"(value)
            : [delta] "i"(3)
            : "cc");
    return value;
}
```

**Generated Rust**

```rust
fn add_three(mut value: i32) -> i32 {
    unsafe {
        core::arch::asm!(
            "addl ${1}, {0:e}",
            inlateout(reg) value => value,
            const 3i32,
            options(pure, nomem, nostack, att_syntax),
        );
    }
    value
}
```

- [GCC Assembly](gcc.md) covers x86-64, i686, AArch64, and Arm32.
- [Clang Assembly](clang.md) covers the same architectures, including
  Clang-specific constraints and diagnostics.
- [Windows Assembly](windows.md) covers Microsoft-style blocks and Clang's GNU
  assembly on Windows targets.

## GNU extended asm

### Constraint syntax

- `=` marks a write-only output. Slate can reuse an input register after the
  input is read.
- `+` marks an operand read before it is written.
- `&` marks an early-clobber output. Its register cannot overlap inputs.
- `%` marks commutative operands.
- `0`, `1`, and other numeric matches tie an input to an output by index.
- `[NAME]` in an input constraint ties it to a named output.
- `,` separates alternatives. Slate selects the first alternative usable by
  every operand.
- Adjacent letters offer several classes within one alternative. Slate tries a
  foldable immediate first, then a fitting register class, an explicit register,
  and memory.
- `?`, `!`, `*`, `^`, and `$` are treated as hints during selection.
- `#` ends the current constraint alternative.
- `-` is accepted as a file-scope constraint modifier. Inside a function it is
  rejected.

### General operand choices

- `r` selects a general-purpose register.
- `m` selects an addressable memory operand. For a memory output, Slate passes
  the destination address to the template.
- `o` selects an offsettable memory operand.
- `V` is handled as a memory operand.
- `g` offers a general register, memory, or an immediate. Eligible link-time
  symbols can also satisfy it.
- `i` offers a foldable integer constant or an eligible link-time symbol. Keep
  the original optimization level when a constant emerges after inlining.
- `n` requires a foldable integer constant.
- `s` requires a link-time symbol. A static object or function plus a constant
  offset can qualify. PIC rules affect symbol selection on x86.

### Template choices

- `%N` substitutes operand number `N`. Outputs come before inputs. Read-write
  operands have a hidden input number after the explicitly listed inputs.
- `%[NAME]` substitutes a named operand.
- `%lN` and `%l[NAME]` substitute an `asm goto` label.
- `%%` emits a literal percent.
- `%=` can form a local label such as `.Lloop%=:`. Slate rewrites these labels
  to numeric local labels. A bare `%=` or its use in a directive requiring a
  named symbol stops lowering.
- `{att|intel}` chooses the branch for the x86 dialect. `%{`, `%|`, and `%}`
  escape those characters.

### Forms and clobbers

- Basic `asm("...")` has no operand list. Slate allows memory and stack effects.
- Extended `asm("..." : outputs : inputs : clobbers)` describes operand effects
  explicitly.
- `inline` accepts the GNU inline-assembly size hint.
- `volatile` keeps the block side-effecting. For volatile extended asm without
  memory operands, GCC mode can preserve a no-memory result. Clang mode models
  possible memory effects.
- `"memory"` declares memory effects beyond the listed operands. It is a
  compiler-level effect. Hardware ordering still depends on the instructions.
- `"cc"` declares condition-code clobbering. On Arm, omitting it lets extended
  asm preserve flags. Slate treats x86 blocks conservatively.
- `"unwind"` records that the block can unwind.
- A named register clobber reserves that register from operand allocation.
  Unknown register names are diagnosed.
- `"redzone"` is recognized. The Rust output drops that clobber.
- `asm goto` can leave the block through a listed C label or fall through. With
  outputs, Slate arranges writeback before continuing along either path.
- File-scope asm preserves source order in `global_asm!`.
- A supported naked function joins its asm statements into one `naked_asm!`. Its
  body may contain asm, comments, and empty statements.

The Rust `global_asm!` and `naked_asm!` forms accept constant and symbol
operands.

To inspect how Slate resolved a constraint, run `slate emit-slate-ir` with the
same flags. To locate an unsupported operand or template, run
`slate lowering-barriers`. See
[Builtins and Inline Assembly](../../parsing/sema/builtins-asm.md) for a worked
IR example.

## x86 operand choices

### Register constraints

- `r` selects a general-purpose register.
- `q` selects a general-purpose register on x86-64. On i686 it selects the AX,
  BX, CX, and DX families.
- `Q` selects a register with a high-byte view.
- `R` selects a legacy register on x86-64 and a general register on i686.
- `l` selects an index register. Slate uses a general register other than the
  stack pointer.
- `a` selects the accumulator family.
- `b` selects the BX family. A reserved register can require scratch code.
- `c` selects the CX family.
- `d` selects the DX family.
- `S` selects the source-index register family.
- `D` selects the destination-index register family.
- `A` uses AX for a word-sized operand and DX:AX for a double-width operand. For
  example, a 64-bit i686 operand uses EDX:EAX.
- `x` chooses XMM, YMM, or ZMM according to width.
- `v` reaches the extended vector register set.
- `Yz` pins the operand to the first vector register.
- `k` and `Yk` select mask registers.
- `y` and `Ym` select MMX. Slate uses a temporary to move supported values
  between memory and an MMX register.
- `t` selects the top of the x87 stack.
- `u` selects the second x87 stack position.
- `f` selects an x87 input. Supported x87 outputs use `t` or `u`.

YMM operands require AVX. ZMM and mask operands require AVX-512F. A 32-bit or
64-bit mask also requires AVX-512BW.

### Immediates, memory, and flags

- `X` can select an immediate, symbol, general register, vector register, or
  memory. Slate can choose a register where GCC puts an output in memory.
- `p` passes an address in a register. Clang pairs it with `%a`.
- `Ws` selects a symbol. PIC rules can prevent a symbol from qualifying.
- `I`, `J`, `K`, `L`, `M`, `N`, `O`, `e`, and `Z` select x86 immediate classes.
  The instruction's encoding still limits their values.
- `=@ccCOND` captures a condition as a scalar. The accepted suffixes are `a`,
  `ae`, `b`, `be`, `c`, `e`, `g`, `ge`, `l`, `le`, `na`, `nae`, `nb`, `nbe`,
  `nc`, `ne`, `ng`, `nge`, `nl`, `nle`, `no`, `np`, `ns`, `nz`, `o`, `p`, `s`,
  and `z`.

### x86 template modifiers

- `%bN` selects the low byte.
- `%hN` selects the high byte where available.
- `%wN` selects 16 bits.
- `%kN` selects 32 bits.
- `%qN` selects 64 bits where available.
- `%xN` selects the XMM view.
- `%tN` selects the YMM view.
- `%gN` selects the ZMM view.
- `%cN` omits the ordinary immediate or symbol prefix.
- `%nN` negates an integer immediate.
- `%aN` prints an address. An eligible x86-64 symbol can use RIP-relative
  addressing. The absolute i686 symbol form stops lowering.
- `%VN` prints a bare register name without the AT&T percent sign.
- `%HN` adds eight bytes to a supported memory reference.

## AArch64 operand choices

- `r` selects a core register.
- `w` selects an FP or vector register.
- `x` restricts FP or vector operands to `v0` through `v15`.
- `y` restricts FP or vector operands to `v0` through `v7`.
- `I` through `N` select immediate classes. Examples include `"I"(100)` for
  addition, `"K"(0xff)` for a 32-bit logical mask, and `"L"(0xffL)` for a 64-bit
  logical mask.
- `Z` accepts zero. An immediate zero under `%w` or `%x` prints `wzr` or `xzr`.
  For a nonzero operand, `rZ` can select a register.
- `Q` selects memory addressed by one base register.
- `Ump` and `Utf` currently stop lowering if no usable alternative exists.
- `S` selects a symbol. A template can add a relocation modifier such as
  `:lo12:`.
- `%wN` and `%xN` select 32-bit and 64-bit core register views.
- `%bN`, `%hN`, `%sN`, `%dN`, and `%qN` select 8-, 16-, 32-, 64-, and 128-bit FP
  or vector register views.
- `=@ccCOND` captures a condition. AArch64 and GCC Arm32 accept `eq`, `ne`,
  `cs`, `cc`, `hs`, `lo`, `mi`, `pl`, `vs`, `vc`, `hi`, `ls`, `ge`, `lt`, `gt`,
  and `le`.
- `fp` and `lr` are aliases for `x29` and `x30`. Slate can save and restore
  Rust-reserved `x19` and `x29` around supported uses.
- A register-variable input can name `sp`. A clobber or a referenced output on
  `sp` stops lowering.
- SVE `zN` and `pN` clobbers require the appropriate SVE feature. Slate
  represents a `zN` clobber with the corresponding `vN` register.
- Slate omits `za`, `zt0`, and `fpmr` clobbers in Rust. Function-level
  [SME state attributes](../attributes/sme.md) stop lowering
  when the function requires them.

## Arm32 operand choices

These choices cover Armv7 in ARM state. Thumb has additional constraint and
instruction-encoding rules.

- `r` selects a core register or a pair for a 64-bit operand.
- `l` acts like `r` in ARM state.
- `t` selects a width-dependent FP subset.
- `w` selects a width-dependent FP or vector register class.
- `x` restricts the FP or vector operand to a smaller register subset.
- `I` and `M` select target immediates, for example `"I"(1)` and `"M"(32)`.
- `Q` selects memory. `Uv`, `Uy`, `Uq`, `Ut`, `Um`, `Un`, and `Us` also select
  memory.
- `%QN` selects the low register of a pair.
- `%RN` and `%HN` select the high register.
- `%PN` prints a VFP double register.
- `%qN` prints a NEON quad register.
- `%eN` and `%fN` select the two halves of a quad register.
- `%cN` omits the immediate prefix.
- `%BN` prints the bitwise inverse of a foldable integer.
- `%LN` prints the low 16 bits of a foldable integer.
- `%mN` prints the base register of a memory operand without brackets.
- NEON quad-register operands require the NEON feature on the function.
- Slate maps `sN` and `qN` clobbers to the covering `dN` registers. It can save
  and restore `r6` and `r11` around supported uses. Clobbering `sp`, `pc`, or
  `vfpcc` stops lowering.
