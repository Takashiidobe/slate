# Generate Builtin Inventories

The parser uses checked-in builtin inventories to recognize compiler-specific
names and resolve prototypes. Regenerate the relevant inventory when updating
the compiler inputs. Handwritten semantic handling lives in
`crates/slate-parser/src/sema/builtins.rs`. The separate
[Intrinsic Catalog](intrinsic-catalog.md) supplies LLVM mappings for lowering.

## Clang

Use the LLVM checkout and `clang-tblgen` that correspond to the Clang oracle.

```sh
python3 crates/slate-parser/tools/generate_clang_builtins.py \
  --clang-tblgen /path/to/llvm-project/build/bin/clang-tblgen \
  --llvm-project /path/to/llvm-project \
  --output crates/slate-parser/src/sema/clang_builtins.rs
```

The default output covers the general and x86 builtin definitions. For the
AArch64 and NEON inventory, generate a separate file from the target-specific
TableGen sources.

```sh
python3 crates/slate-parser/tools/generate_clang_builtins.py \
  --clang-tblgen /path/to/llvm-project/build/bin/clang-tblgen \
  --llvm-project /path/to/llvm-project \
  --table CLANG_AARCH64_BUILTINS=BuiltinsAArch64.td \
  --neon-table CLANG_NEON_BUILTINS=arm_neon.td,arm_fp16.td \
  --prototype-prefix AARCH64_PROTOTYPE \
  --output crates/slate-parser/src/sema/clang_aarch64_builtins.rs
```

## GCC

Pass the `gcc/` subdirectory of a GCC source checkout. The generic generator
preprocesses the builtin, type, and attribute definitions. The x86 generator
also reads target registration sources under `gcc/config/i386`.

```sh
python3 crates/slate-parser/tools/generate_gcc_builtins.py \
  --gcc-source /path/to/gcc-checkout/gcc \
  --output crates/slate-parser/src/sema/gcc_builtins.rs
python3 crates/slate-parser/tools/generate_gcc_x86_builtins.py \
  --gcc-source /path/to/gcc-checkout/gcc \
  --output crates/slate-parser/src/sema/gcc_x86_builtins.rs
```

For Arm, the plugin captures the builtins the cross compilers expose after
loading their target headers. This needs the AArch64 and Arm cross compilers and
their GCC plugin headers. The second command turns the checked-in TSV dumps into
Rust declarations.

```sh
python3 crates/slate-parser/tools/gcc_plugin/dump_gcc_builtins.py
python3 crates/slate-parser/tools/generate_gcc_arm_builtins.py \
  --output crates/slate-parser/src/sema/gcc_arm_builtins.rs
```

## MSVC

Set up the MSVC oracle and SDK as described in
[Setup](../setup.md#install-msvc-and-the-windows-sdk). The generator uses
Microsoft's builtin inventory and the installed headers, then probes whether
`cl.exe` recognizes each entry for the selected architecture.

```sh
python3 crates/slate-parser/tools/generate_msvc_builtins.py \
  --slate target/test-cache/release/slate
```

`--documents <directory>` uses saved Microsoft Learn Markdown pages. The
optional Slate binary adds lowering probe results to the inventory. The default
outputs are `src/sema/msvc_builtins.rs` and
`tools/msvc_intrinsics_alphabetical.jsonl` under `crates/slate-parser/`. The
JSON Lines inventory records where the declaration came from, where it was
recognized, and which entries lack compiler evidence.

## Review the output

Review changed names, prototypes, and target restrictions. Parser acceptance and
Rust lowering have separate coverage. Run the `parser` profile for changes to
recognized builtins and use Slate differential fixtures when the emitted Rust
changes too.
