# slate-parser

Slate-Parser is a C frontend for Slate. It is being built to ingest real C
headers and source files and turn them into a structured, semantically
resolved intermediate representation that Slate can translate to Rust.

The frontend processes one compiler configuration at a time. Command-line
defines, target predefines, and include files are resolved during preprocessing;
conditional branches are selected there before parsing. The pipeline is:

```text
source text
    │
    ▼
lexing → preprocessing → parsing → sema → IR
```

## Lexing

The lexer turns source text into tokens while retaining source locations and
the spelling and expansion provenance needed for diagnostics. It handles C
identifiers, keywords, literals, comments, and punctuators.

Implementation: [`src/lexer.rs`](src/lexer.rs)

## Preprocessing

The preprocessor consumes lexer tokens, handles directives and macro
expansion, resolves includes, and selects the active branch of `#if`-
family directives for the requested configuration. Include provenance is
retained so declarations can be traced back to their headers.

Implementation: [`src/pp/`](src/pp/), with file and search-path handling in
[`src/files.rs`](src/files.rs)

## Parsing

The parser converts the preprocessed token stream into a C AST. It preserves
the structure and source information of declarations, types, expressions,
statements, attributes, and inline assembly so later stages can interpret the
source without reparsing it.

Implementation: [`src/parser/`](src/parser/), with the AST definitions in
[`src/ast.rs`](src/ast.rs)

## Semantic analysis

Semantic analysis resolves names, scopes, declarations, tags, and types. It
also validates C type rules, performs implicit conversions and integer
promotions, evaluates expressions where the target data layout matters, and
records the information needed for lowering.

Implementation: [`src/sema/`](src/sema/)

## Intermediate representation

The IR is a small, Clang-IR-inspired representation designed as the handoff to
Slate's Rust conversion. It normalizes resolved types, functions, globals,
records, enumerators, places, values, conversions, arithmetic, calls, and
control-flow statements. The IR is currently focused on declarations and
simple function bodies; unsupported C constructs report an error during
lowering.

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

Popular C projects that slate-parser can parse with `--flavor=clang` on
`x86_64-unknown-linux-gnu`.

| Project                                              | Revision               |
| ---------------------------------------------------- | ---------------------- |
| [musl](https://musl.libc.org)                        | `v1.2.6-20-gf21a9653`  |
| [SQLite](https://sqlite.org)                         | `0eaef28cf2`           |
| [Lua](https://www.lua.org)                           | `v5.5.1`               |
| [PCRE2](https://github.com/PCRE2Project/pcre2)       | `a2b146a`              |
| [giflib](https://giflib.sourceforge.net)             | `6.1.3-4-ga8e3114`     |
| [cJSON](https://github.com/DaveGamble/cJSON)         | `fb16e5c`              |
| [libyaml](https://github.com/yaml/libyaml)           | `0.2.5-16-g893682b`    |
| [libexpat](https://libexpat.github.io)               | `R_2_8_2-51-gdfdbaadf` |
| [zlib](https://zlib.net)                             | `e3dc0a8`              |
| [QuickJS](https://bellard.org/quickjs/)              | `04be246`              |
| [LZ4](https://github.com/lz4/lz4)                    | `0774d05`              |
| [TinyCC](https://bellard.org/tcc/)                   | `2ba12e8`              |
| [chibicc](https://github.com/rui314/chibicc)         | `90d1f7f`              |
| [yyjson](https://github.com/ibireme/yyjson)          | `757305b`              |
| [utf8proc](https://github.com/JuliaStrings/utf8proc) | `0075ed7`              |

Last swept 2026-09-30. Work toward the rest of the corpus is tracked in
the `bd` epic `slate-parser-6x05`.

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
