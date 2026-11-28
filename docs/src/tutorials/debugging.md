# Debug a translation

Start with the first error and the compile command for the affected C file. For
a project, find its entry in `compile_commands.json` and run from the entry's
`directory` so relative paths still resolve. For the inspection commands below,
keep the target, compiler flavor, defines, and include and language options.
Leave out the compiler executable, the original input argument, and build output
options such as `-c` and `-o`.

| Symptom                                             | What to check                                                                                                                                       |
| --------------------------------------------------- | --------------------------------------------------------------------------------------------------------------------------------------------------- |
| Header missing                                      | Confirm the C build generated it and the command has its include path. For standard headers, check the compiler bundle and target sysroot.          |
| Wrong files translated                              | Check the selected C build target. Pass `--build-target` for a direct project translation or set `build_target` in the manifest.                    |
| Wrong `#if` branch                                  | Inspect the preprocessed source. Compare the target, flavor, and defines against the original build.                                                |
| C parses but Slate reports an unsupported operation | Run `lowering-barriers` on the affected file with the same options.                                                                                 |
| Rust fails to compile or link                       | Check the first Cargo diagnostic and the Rust target. Check the target linker, external libraries, and the C compiler for any generated `build.rs`. |

For a missing system header, start with the relevant checks:

```sh
slate sysroot doctor native clang
slate sysroot doctor aarch64-unknown-linux-gnu
slate sysroot doctor compiler-headers clang
```

Use the native check for a native translation. Use the target and bundle checks
for an installed setup. See [Sysroots and Headers](../reference/sysroots.md).

To isolate a failure, use the same options at each stage. For example:

```sh
slate-parser pp input.c -Iinclude -DFEATURE=1
slate emit-slate-ir -Iinclude -DFEATURE=1 input.c
slate lowering-barriers -Iinclude -DFEATURE=1 input.c
```

The first command requires the separate `slate-parser` binary. It prints the
source after preprocessing. `emit-slate-ir` shows the typed program Slate will
lower. `lowering-barriers` reports operations Slate cannot translate.

If a merged crate fails for one configuration, translate that target and feature
set directly. Use the same `--manifest` with a single `--manifest-target` and,
if needed, `--features`. Compare that result with the merge report. See
[Check a merge](../project/merge.md#check-a-merge).

When reporting a failure, include the Slate version, command, target, first
diagnostic, and the smallest input or compile command that reproduces it.
