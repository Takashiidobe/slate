# slate-parser

slate-parser is a C frontend for slate, which translates C23 to Rust.
Slate used to use clang-ir, but having to join clang-ir with the
clang-ast for comments, macro provenance, and any unsupported feature
was suboptimal, since it requires a bunch of machinery on slate's side +
requires the user compile their own clang (since clang-ir isn't in
mainline yet).

slate-parser's goal is to "emulate" the three major C compilers. It does
this with its `--flavor` argument, which can be gcc, clang, or msvc.

The pipeline is pretty classic:

```text
source text -> lexing -> preprocessing -> parsing -> sema -> IR
```

## Lexing

Implementation: [`src/lexer.rs`](src/lexer.rs)

## Preprocessing

Implementation: [`src/pp/`](src/pp/), with file and search-path handling in
[`src/files.rs`](src/files.rs)

## Parsing

Implementation: [`src/parser/`](src/parser/), with the AST definitions in
[`src/ast.rs`](src/ast.rs)

## Semantic analysis

Implementation: [`src/sema/`](src/sema/)

## Intermediate representation

Our IR is inspired by clang-ir, except it's not mainly for optimization.
We keep any data around that's useful for lowering to rust (say
different externs) but if these aren't expressible in rust, the IR will
apply them as well.

Implementation: [`src/ir/`](src/ir/), with AST-to-IR lowering in
[`src/sema/module.rs`](src/sema/module.rs)

For example, a source function such as:

```c
int main() { return 1 + 2; }
```

is represented textually in the IR as:

```text
module {
    target [char_signed=true, short_width=16, int_width=32, ...];
    fn %0 @main() -> i32 [linkage=external] [c_storage="none", c_return="Int"] {
        return add<i32, overflow=undefined>(const<i32>(1), const<i32>(2));
    }
}
```

The exact target line depends on the selected target. Arithmetic carries
semantic metadata such as integer overflow behavior. With `--show-metadata`,
the printer also emits source-level metadata in brackets, such as
`c_storage` and `c_return`, attached to the corresponding IR node.

## Command-line usage

```text
slate-parser <parse|ir|pp> <source.c> [options]
```

`parse` prints the parsed AST. `ir` lowers the AST through sema and prints the
textual IR. `pp` prints the preprocessed token stream, which
[`tools/pp_diff.py`](tools/pp_diff.py) compares with `clang -E -P`. The current flags are:

- `-DNAME` or `-D NAME`: define a preprocessor macro.
- `-std=c89|gnu89|c99|gnu99|c11|gnu11|c17|gnu17|c23|gnu23`: select the C
  language standard.
- `-isystemPATH` or `-isystem PATH`: add a system include search path.
- `--flavor=gcc|clang|msvc`: select compiler personality defaults.
- `--dump-ir`: print IR while using the `parse` command.
- `--dump-ir-names`: print sema name resolution.
- `--dump-ir-expressions`: print resolved expression roots.
- `--show-metadata`: include IR metadata; requires `ir` or `--dump-ir`.
- `--show-spans`: include source spans in expression output; requires
  `--dump-ir-expressions`.
- `--show-comments`: retain comments in AST output.
- `--show-ids`: include AST node identifiers in AST output.

Compiler compatibility flags currently recognized include `-fwrapv`,
`-ftrapv`, `-frounding-math`, `-ftrapping-math`, and the
`-mlong-double-{64,80,128}` target-layout options. The complete parsing and
validation logic is in [`src/compiler_args.rs`](src/compiler_args.rs) and
[`src/compiler_options.rs`](src/compiler_options.rs).

## Trophy case

C projects that slate-parser can parse with `--flavor=clang` on
`x86_64-unknown-linux-gnu`.

| Project                                              | Revision                         |
| ---------------------------------------------------- | -------------------------------- |
| [musl](https://musl.libc.org)                        | `v1.2.6-20-gf21a9653`            |
| [Redis](https://redis.io)                            | `e1d7d50`                        |
| [Mbed TLS](https://github.com/Mbed-TLS/mbedtls)      | `522e2e4`                        |
| [SQLite](https://sqlite.org)                         | `version-3.53.0-768-g0eaef28cf2` |
| [Lua](https://www.lua.org)                           | `v5.5.1`                         |
| [libexpat](https://libexpat.github.io)               | `R_2_8_2-51-gdfdbaadf`           |
| [giflib](https://giflib.sourceforge.net)             | `6.1.3-4-ga8e3114`               |
| [cJSON](https://github.com/DaveGamble/cJSON)         | `fb16e5c`                        |
| [libyaml](https://github.com/yaml/libyaml)           | `0.2.5-16-g893682b`              |
| [zlib](https://zlib.net)                             | `e3dc0a8`                        |
| [QuickJS](https://bellard.org/quickjs/)              | `04be246`                        |
| [LZ4](https://github.com/lz4/lz4)                    | `0774d05`                        |
| [TinyCC](https://bellard.org/tcc/)                   | `2ba12e8`                        |
| [chibicc](https://github.com/rui314/chibicc)         | `90d1f7f`                        |
| [utf8proc](https://github.com/JuliaStrings/utf8proc) | `0075ed7`                        |
| [yyjson](https://github.com/ibireme/yyjson)          | `757305b`                        |
| [zstd](https://github.com/facebook/zstd)             | `10da6ba`                        |

## Development

The standard test gate is:

```bash
cargo nextest run --release
```

FileCheck fixtures live under `tests/`. When a fixture or its renderer
changes, regenerate expectations with:

```bash
python3 tools/update_filecheck.py --in-place <fixture>
```

Project work is tracked with `bd`; run `bd ready` for available tasks.
