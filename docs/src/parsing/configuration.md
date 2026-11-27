# Translation Unit Configuration

Use the original compile command when translating a source file. Slate needs the
same active code, type layout, and calling convention that the C compiler would
use. Two commands for the same file can produce different typed IR.

<figure style="margin-inline: 0; text-align: center;">

![Compiler arguments produce CompilerArgs and a Dialect. Preprocessing, parsing, and sema read that configuration. Typed IR carries the resolved effects into Rust lowering.](../diagrams/parsing-arguments.svg)

</figure>

## What the command changes

- **Compiler flavor.** Slate emulates the compiler that built the source. Clang,
  GCC, and MSVC can assign different meanings to the same code or flags, even on
  the same target. For example, with `-ftrapv -fwrapv`, GCC uses wrapping signed
  arithmetic because the last flag wins. Clang keeps trapping arithmetic because
  an active `-ftrapv` takes precedence. Slate records the selected overflow
  behavior in IR.
- **C standard.** The standard controls which features are available and can
  change the meaning of existing syntax. For example, sema interprets an empty
  parameter list according to the selected standard.
- **Target.** The target triple selects compiler and system headers, pointer
  widths, type layout, and ABI rules. Target options can refine that choice. An
  x86 long double override changes both the macros seen by headers and the type
  that sema puts in IR.
- **Flags and inputs.** Include paths, forced files, and macro definitions
  change which declarations reach the parser. Some flags change the Rust code
  Slate has to emit.

## Why optimization levels matter

An inline assembly `"i"` operand needs a compile-time immediate. In this
fixture, the operand depends on a function parameter.

```c
static inline __attribute__((always_inline)) int bit(int nr) {
    int out;
    __asm__("movl %1, %0" : "=r"(out) : "i"(1 << (nr & 7)));
    return out;
}

int example(void) { return bit(9); }
```

At `-O0`, the compiler cannot resolve this operand to an immediate. At `-O2`,
inlining and constant folding expose the value `2`. Slate passes the C
optimization level to sema so it knows when to specialize the helper and emit a
Rust asm operand such as `const 2i32`. Preprocessing also uses the level for
macros such as `__OPTIMIZE__`. See the
[inline asm fixture](https://github.com/takashiidobe/slate/blob/main/crates/slate/tests/fixtures/x86_64/asm_immediate_after_inlining.c).

## How a flag changes Rust output

The Linux kernel uses `-fno-delete-null-pointer-checks`. With that flag, a
dereference does not prove that a pointer is non-null. A later check in
`int value = *p; if (!p) return -1;` must survive optimization.

Sema records `Module::null_pointer_is_valid` in IR. Rust lowering emits
`slate_support::opaque_is_null(p)` for the comparison. The helper hides the
pointer value from LLVM so it cannot use the earlier load to fold away the
check. See the
[null pointer fixture](https://github.com/takashiidobe/slate/blob/main/crates/slate/tests/fixtures/no_delete_null_pointer_checks.c).

## How the settings reach each stage

1. **Read the command.** `CompilerArgParser::parse` produces `CompilerArgs`. It
   resolves the flavor and target, including the target ISA. Search paths, macro
   options, forced files, and compiler options come from the same command.
   Relative paths in a compilation database use its command's working directory.
   Project translation uses the compiler name in the command to infer the
   flavor, with `--flavor` available as an override.
2. **Build the dialect.** `Dialect::new` combines the flavor, standard, target,
   and options, and applies target overrides. Preprocessing reads this
   configuration to seed predefines and uses the command's search paths for
   headers. Parsing uses its `StandardFeatures` to recognize syntax.
3. **Resolve C semantics.** `TranslationUnit::dialect` carries the dialect into
   sema. `TypeResolver::with_names` takes that configuration for type,
   conversion, layout, and ABI rules. The sema lowerer builds its numeric
   context from the same dialect.
4. **Emit Rust from IR.** Typed IR carries the effective target and resolved
   behavior, including such details as layout, ABI, and arithmetic overflow
   rules. Rust lowering reads those results. The compiler flavor and original
   flag list stay on the frontend side.

## Query the configuration

| When you need                                          | Read                                                                                                                                 |
| ------------------------------------------------------ | ------------------------------------------------------------------------------------------------------------------------------------ |
| Input and include search configuration                 | `CompilerArgs` and `CompilerArgs::search_paths()` in `compiler_args.rs`.                                                             |
| The selected compiler or standard                      | `Dialect::flavor()` and `Dialect::standard()` in `dialect.rs`.                                                                       |
| A grammar or keyword gate                              | `Dialect::features()` or the parser's `features()` accessor.                                                                         |
| Effective width, alignment, long double format, or ISA | `Dialect::target()` returns the effective `TargetInfo` after target overrides.                                                       |
| Resolved compiler options                              | `Dialect::options()` returns `CompilerOptions`. For example, `CodegenOptions::optimization` holds the selected C optimization level. |
| The configuration during sema                          | Use the dialect on the type resolver or the numeric context built by `Context::for_dialect` in `sema/numeric.rs`.                    |

These paths are under `crates/slate-parser/src/`. For header lookup, see
[Headers and Include Search](headers.md).
