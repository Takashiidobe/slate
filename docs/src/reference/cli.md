# Command Line

Run `slate --help` to list commands. Use the command forms below for the
translation options. Compiler options follow the command and precede the input
file.

| Command                                                       | Output                                                                                         |
| ------------------------------------------------------------- | ---------------------------------------------------------------------------------------------- |
| `slate translate [compiler options] FILE`                     | Rust source on standard output.                                                                |
| `slate emit-slate-ir [--passes=LIST] [compiler options] FILE` | Typed IR on standard output. Put `--passes` first and use comma-separated pass names or `all`. |
| `slate lowering-barriers [compiler options] FILE`             | Diagnostic report of constructs that prevent Rust lowering.                                    |
| `slate translate-project [options] PROJECT OUTPUT`            | A Cargo crate in `OUTPUT` from the selected C build.                                           |
| `slate project target …` and `slate project feature …`        | Edit a [project manifest](project-manifest.md).                                                |
| `slate sysroot …`                                             | Manage [target and compiler headers](sysroots.md).                                             |

For a single file, use `--flavor=gcc` or `--flavor=msvc` to override the default
Clang flavor. Use `--target=TRIPLE` to override the default target, or
`translate --targets=TRIPLE,TRIPLE` to merge translations of that file. See
[Compiler Flags](flags/index.md) for C compiler options.

## Project translation

```text
slate translate-project --compile-commands DATABASE [options] PROJECT OUTPUT
slate translate-project --manifest FILE --manifest-target NAME [options] PROJECT OUTPUT
slate translate-project --manifest FILE --merge [options] PROJECT OUTPUT
```

| Option                                            | Effect                                                                                                                  |
| ------------------------------------------------- | ----------------------------------------------------------------------------------------------------------------------- |
| `--compile-commands DATABASE`                     | Read the C compilation database for a direct translation.                                                               |
| `--build-target NAME`                             | Select the C build target in the database.                                                                              |
| `--crate-type staticlib` or `--crate-type cdylib` | Produce a C-compatible static archive or shared library.                                                                |
| `--manifest FILE`                                 | Read named targets and feature definitions.                                                                             |
| `--manifest-target NAME`                          | Select one manifest target for a direct translation. For a merge, pass a comma-separated list to restrict the targets.  |
| `--features NAME,NAME`                            | Enable named manifest features for a direct translation.                                                                |
| `--merge`                                         | Combine manifest configurations into one crate with Rust conditional compilation.                                       |
| `--check-projection all`                          | Compare all valid merged configurations with their direct translations. See [Merge](../project/merge.md#check-a-merge). |
| `--plan`                                          | Report the configurations to translate from a manifest. Give the project path and omit the output path.                 |

For a complete example, see [Translate zlib](../tutorials/zlib.md). To inspect a
single translation unit, see [Debug a translation](../tutorials/debugging.md).
