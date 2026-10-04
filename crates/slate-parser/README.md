# slate-parser

slate-parser is Slate's C frontend. It preprocesses and parses C, performs
semantic analysis, and emits a typed IR that Slate translates to Rust.
Compiler personalities (`--flavor=clang|gcc|msvc`) model differences in
predefines, language extensions, type layout, and ABI.

```text
source -> lexing -> preprocessing -> AST -> sema (names + check + lower) -> IR
```

Each run selects one compiler flavor, language standard, target, and flag
set. Preprocessing selects a single configuration; the AST has no
conditional compilation branches. The IR keeps the resolved C semantics
needed for Rust translation, including conversions, storage, linkage,
arithmetic policies, and sequenced side effects.

## Command-line usage

Build and run from the workspace root:

```bash
cargo build --release -p slate-parser --bin slate-parser
target/test-cache/release/slate-parser <parse|ir|pp> <source.c> [options]
```

| Command | Output |
| --- | --- |
| `pp` | Preprocessed token stream. |
| `parse` | AST, after semantic checks. |
| `ir` | Typed IR, after semantic checks and lowering. |

The defaults are `--flavor=clang`, `-std=gnu23`, and
`-target=x86_64-unknown-linux-gnu`.

### Output flags

| Flag | Effect |
| --- | --- |
| `--show-comments` | Retain comments in the AST dump. |
| `--show-ids` | Include AST node identifiers. |
| `--dump-ir` | Print the IR instead of the AST with `parse`; equivalent to `ir`. |
| `--dump-ir-types` | Print resolved types and function signatures without lowering bodies. |
| `--dump-ir-names` | Print name resolution and bindings. |
| `--dump-ir-expressions` | Print resolved expression roots without a module or control flow. |
| `--show-metadata` | Include source-level IR metadata; requires `ir`, `--dump-ir`, or `--dump-ir-types`. |
| `--show-spans` | Include source spans; requires `ir`, `--dump-ir`, or `--dump-ir-expressions`. |
| `--compact-ir` | Hide conversion reasons, operation policies, and call signatures; requires `ir`, `--dump-ir`, or `--dump-ir-types`. |

The four `--dump-ir*` modes are mutually exclusive. The `ir` command
already selects `--dump-ir`, so it cannot be combined with another dump
mode. AST display flags apply only to AST output. `pp` stops after
preprocessing.

### Configuration and preprocessor flags

These spellings are accepted with all three flavors, including the MSVC
personality; native MSVC `/` options are not modeled.

| Flag | Effect |
| --- | --- |
| `--flavor=clang\|gcc\|msvc` | Select compiler personality. |
| `-std=<standard>` | Select a language mode (listed below). |
| `-target=<triple>`, `--target=<triple>` | Select a registered target with predefines for the chosen flavor. |
| `-DNAME[=VALUE]`, `-D NAME[=VALUE]` | Define a macro (`1` when no value is given). |
| `-UNAME`, `-U NAME` | Undefine a macro. |
| `-include <file>` | Process a file before the main source, retaining its declarations. |
| `-imacros <file>` | Process a file before the main source, retaining only its macros. |
| `-I<dir>`, `-I <dir>` | Add a user include directory. |
| `-iquote <dir>` | Add a search directory for quoted includes only. |
| `-isystem <dir>` | Add a system include directory. |
| `-idirafter <dir>` | Add a system include directory searched after standard directories. |
| `-nostdlibinc` | Omit sysroot standard include directories. |
| `-nostdinc` | Omit both compiler builtin headers and sysroot standard directories. |
| `-isysroot <dir>`, `--sysroot=<dir>` | Override the target sysroot; `-isysroot` takes precedence. |

Value options such as `-std`, `-target`, and `-isystem` also accept a
separate argument or `=value`. `-D`/`-U` apply in order, then all
`-imacros` files, then all `-include` files. Relative paths resolve from
the working directory. See [compiler argument rules](../../wiki/concepts/compiler-arg-rules.md)
for include search order and target validation.

## Language standards

| Mode | Accepted `-std=` names |
| --- | --- |
| C89 / C90 | `c89`, `c90`, `iso9899:1990`; `gnu89`, `gnu90` |
| C94 amendment | `iso9899:199409` |
| C99 | `c99`, `c9x`, `iso9899:1999`, `iso9899:199x`; `gnu99`, `gnu9x` |
| C11 | `c11`, `c1x`, `iso9899:2011`; `gnu11`, `gnu1x` |
| C17 / C18 | `c17`, `c18`, `iso9899:2017`, `iso9899:2018`; `gnu17`, `gnu18` |
| C23 | `c23`, `c2x`, `iso9899:2024`; `gnu23`, `gnu2x` |
| C2y draft | `c2y`, `gnu2y` |

These are supported language modes, not claims of complete standards
conformance. C2y support is partial: the mode is accepted, but some draft
features remain unimplemented. Flavor and standard together determine
extensions and diagnostic defaults; selecting an ISO mode does not by
itself reject every extension. Use `-pedantic` or `-pedantic-errors` for
the modeled extension diagnostics.

## Supported compiler flags

The following flags model frontend semantics, ABI, or predefined macros.
Unless marked otherwise, they are available with both GCC and Clang
flavors. On/off `-f` switches also accept `-fno-` forms, except that
`-fno-freestanding` and `-fno-hosted` are GCC only. Target options are
subject to target-specific validation. The MSVC flavor rejects the
modeled `-f`, `-m`, and `-O` flags in this table.

| Flags | Effect / restrictions |
| --- | --- |
| `-ffreestanding`, `-fhosted` | Hosted predefines and library-builtin availability. |
| `-fbuiltin`, `-fno-builtin`, `-fno-builtin-<name>` | Control library builtins; explicit `__builtin_` spellings remain available. |
| `-funsigned-char`, `-fsigned-char` | Plain `char` signedness and predefines. |
| `-fshort-wchar` | `wchar_t` layout and wide-character predefines. |
| `-fwrapv`, `-ftrapv`, `-fstrict-overflow` | Signed arithmetic overflow policies; the pointer wrapping implied by `-fno-strict-overflow` is not yet applied. |
| `-frounding-math`, `-ftrapping-math` | Floating-point rounding and exception policies. |
| `-fgnu89-inline`, `-fcommon` | Inline definitions and tentative-definition linkage. |
| `-fms-extensions`, `-fms-compatibility` | Microsoft language modes; Clang only. |
| `-fms-anonymous-structs` | Microsoft anonymous record members; Clang only. |
| `-fasm-blocks` | MS `__asm` blocks on x86; Clang only. |
| `-fasynchronous-unwind-tables` | Unwind-related predefines. |
| `-fpic`, `-fPIC`, `-fpie`, `-fPIE` | PIC/PIE predefines only. |
| `-fstack-protector`, `-fstack-protector-strong`, `-fstack-protector-all`, `-fno-stack-protector` | Stack-protector predefines only; `-fstack-protector-explicit` is GCC only. |
| `-fcf-protection[=full\|branch\|return\|none\|check]` | CET predefines on x86; `check` is GCC only. |
| `-O`, `-O0` through `-O3`, `-Os`, `-Oz`, `-Og` | Optimization predefines only; no IR optimization. |
| `-m16`, `-m32`, `-m64` | Retarget the selected triple where a corresponding target exists. `-mx32` is recognized but rejected until an x32 target exists. |
| `-mregparm=N` | Register-parameter ABI on 32-bit x86; GCC requires `0..3`. |
| `-mlong-double-64`, `-mlong-double-80`, `-mlong-double-128` | `long double` layout and predefines; x86 only. |
| `-mpreferred-stack-boundary=N` | Stack alignment as a power-of-two exponent; GCC, x86 only. |
| `-mstack-alignment=N` | Stack alignment in bytes (power of two); Clang only. |
| `-masm=att\|intel` | GNU asm dialect; GCC requires x86. |
| `-march=<arch-or-cpu>`, `-m<feature>`, `-mno-<feature>` | Supported ISA features, predefines, and ABI vector widths; accepted names depend on target and flavor. |
| `-m80387`, `-mno-80387`, `-mx87`, `-mno-x87` | x87 configuration; `x87` spelling is Clang only. x87 excess precision is not yet modeled in IR. |
| `-mfp-ret-in-387`, `-mno-fp-ret-in-387` | The positive spelling is GCC only; Clang's negative spelling disables x87. |
| `-m3dnow`, `-m3dnowa` (and `-mno-` forms) | GCC 3DNow! predefines; Clang accepts and ignores them. |
| `-mfpu=<fpu>`, `-mfloat-abi=<abi>`, `-mthumb`, `-marm` | Arm32 ISA and float ABI. |
| `-msve-vector-bits=<bits>` | AArch64 SVE predefines. |
| `-W<name>`, `-Wno-<name>`, `-Werror`, `-Wno-error`, `-Werror=<name>`, `-Wno-error=<name>` | Modeled diagnostic severities; accepted with all flavors. Unknown warning names are ignored. |
| `-pedantic`, `-Wpedantic`, `-pedantic-errors`, `-Wno-pedantic` | Modeled extension diagnostics; accepted with all flavors. |

Some options are recognized with incomplete or no semantic effect:

| Flags | Current behavior |
| --- | --- |
| `-fstrict-flex-arrays=0..3` | Stored, with no semantic consumer yet; bare `-fstrict-flex-arrays` and `-fno-strict-flex-arrays` are GCC only. |
| `-fexperimental-late-parse-attributes` (and `-fno-`) | Clang only; stored, but `counted_by` arguments are not yet resolved. |
| `-mcmodel=tiny\|small\|kernel\|medium\|large` | GCC/Clang: parsed and ignored; backend target restrictions are not checked. |
| `-moutline`, `-mno-outline` | Clang only; accepted and ignored. |

For compile-command compatibility, all flavors also accept and ignore
`-c`, `-M`, `-MM`, `-MD`, `-MMD`, `-MP`, `-MG`, `-pipe`, `-g*`, and
`-o`, `-MF`, `-MT`, `-MQ`, `-MJ` with joined or separate values. No object
or dependency files are produced. Ignored codegen flags are
`-fomit-frame-pointer`, `-flto`, `-ffunction-sections`, `-fdata-sections`,
`-fstrict-aliasing`, `-fplt`, `-fsemantic-interposition`, `-funwind-tables`,
`-fstack-clash-protection`, `-fmerge-all-constants`, `-fident`, and
`-faddrsig` (including `-fno-` forms), plus `-flto=`, `-fvisibility=`,
and `-fdebug-prefix-map=` values.

Unknown options are errors. In particular, `-Ofast`, the fast-math family,
`-fshort-enums`, and `-fpack-struct` are not implemented. See
[compiler flags](../../wiki/concepts/compiler-flags.md) for flavor-specific
precedence, ISA coverage, and remaining gaps, and
[diagnostic severity](../../wiki/concepts/diagnostic-severity.md) for the
warning names that have an effect. Argument parsing and validation live
in [`src/compiler_args.rs`](src/compiler_args.rs).

## From AST to typed IR

Save this as `example.c`. It sums a partially initialized array, using a
typedef, an enum constant, array parameter syntax, and a target layout
query:

```c
typedef unsigned short Sample;
enum { COUNT = 2 + 1 };

unsigned sum(const Sample samples[COUNT]) {
    unsigned total = 0;
    for (int i = 0; i < COUNT; ++i)
        total += samples[i];
    return total + sizeof(Sample);
}

int main(void) {
    Sample samples[COUNT] = {[1] = 7, [2] = 9};
    return (int)sum(samples);
}
```

From the workspace root, emit its AST and IR separately:

```bash
target/test-cache/release/slate-parser parse example.c --flavor=clang -std=c2y -target=x86_64-unknown-linux-gnu
target/test-cache/release/slate-parser ir example.c --flavor=clang -std=c2y -target=x86_64-unknown-linux-gnu
```

The AST preserves source structure. These excerpts show the array
parameter and the return expression in `sum` (surrounding nodes omitted;
indentation reduced):

```text
ParameterDeclarationKind {
    specifiers: DeclarationSpecifiers {
        ty: Named(
            "Sample",
        ),
        qualifiers: Qualifiers {
            is_const: true,
        },
    },
    declarator: Array {
        inner: Name(
            "samples",
        ),
        size: Expression(
            Identifier(
                "COUNT",
            ),
        ),
    },
},
...
Return(
    Binary {
        op: Add,
        left: Identifier(
            "total",
        ),
        right: SizeOfType {
            ty: TypeName {
                specifiers: DeclarationSpecifiers {
                    ty: Named(
                        "Sample",
                    ),
                },
                declarator: Abstract,
            },
        },
    },
),
```

The IR makes the resolved meaning explicit. Only the target storage table
is abbreviated here; binding numbers can change between versions:

```text
module {
    target "x86_64-unknown-linux-gnu" { ... }
    type @type0 Sample = u16;
    type @type1 = enum : u32 {
        %0 COUNT = const<i32>(3);
    } [size=4, align=4];
    fn %3 @sum(%4 samples: ptr<const u16> [array=3]) -> u32 [linkage=external] [fallthrough=ub_if_used] {
        let %5 total: u32 [storage=automatic] = reinterpret<u32, reason=assign, fits=always>(const<i32>(0));
        for %9
            init:
                let %6 i: i32 [storage=automatic] = const<i32>(0);
            condition: lt<i32>(read<i32>(%6), const<i32>(3))
            increment: {
                let %10: i32 [synthetic] = read<i32>(%6);
                let %11: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%10), const<i32>(1));
                write<i32>(%6, read<i32>(%11));
                yield void;
            }
            body:
                let %12: u32 [synthetic] = read<u32>(%5);
                let %13: u32 [synthetic] = add<u32, overflow=wrap>(read<u32>(%12), reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(deref(ptr_offset<ptr<const u16>, subtract=false, element=u16, overflow=ub>(read<ptr<const u16>>(%4), read<i32>(%6))))))));
                write<u32>(%5, read<u32>(%13));
        return truncate<u32, reason=return, fits=unknown>(add<u64, overflow=wrap>(widen<u64, reason=usual_arith>(read<u32>(%5)), const<u64>(2)));
    }
    fn %7 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
        let %8 samples: array<u16, 3> [storage=automatic] = aggregate<array<u16, 3>, zero_fill=true>(index1 = reinterpret<u16, reason=assign, fits=unknown>(truncate<i16, reason=assign, fits=always>(const<i32>(7))), index2 = reinterpret<u16, reason=assign, fits=unknown>(truncate<i16, reason=assign, fits=always>(const<i32>(9))));
        return reinterpret<i32, reason=explicit, fits=unknown>(call<u32, signature=fn(ptr<const u16>) -> u32>(%3, pointer_cast<ptr<const u16>, reason=arg>(array_decay<ptr<u16>, length=Some(3)>(%8))));
    }
}
```

| Source / AST | Resolved IR |
| --- | --- |
| `Sample`, `COUNT = 2 + 1` | `Sample = u16`; enum constant `3` and concrete array length `3`. |
| `const Sample samples[COUNT]` parameter | Adjusted to `ptr<const u16>`, retaining `[array=3]`. |
| Identifiers such as `total` and `samples` | References to explicit bindings (`%5`, `%4`, `%8`); each access is a typed `read`, `write`, or place. |
| `total += samples[i]` | Pointer offset and load, integer promotion to `i32`, then conversion to `u32` for unsigned addition (`overflow=wrap`). |
| `++i` | A sequenced read, signed addition (`overflow=ub`), and write using synthetic temporaries. |
| `total + sizeof(Sample)` | `sizeof` becomes `const<u64>(2)`; addition widens `total` to `u64`, then the return converts to `u32`. |
| `{[1] = 7, [2] = 9}` | Explicit aggregate indices and element conversions, with `zero_fill=true` for the omitted first element. |
| `sum(samples)` and `(int)` | Array decay, pointer qualification conversion, resolved call signature, and explicit result conversion. |

Sema records types, bindings, operand conversions, and initializer plans;
lowering consumes those facts to build IR. Required constants and layout
queries are evaluated, but ordinary arithmetic is preserved for Rust
translation rather than optimized away. `--show-metadata` adds source
C types and other metadata; `--compact-ir` hides some of the semantic
annotations shown above.

The AST data model can represent trees that are not valid C programs.
Parsing alone is not semantic validation: names, operand constraints,
declaration compatibility, and conversions belong to sema. The CLI's
`parse` command does run the checker before printing, but it neither
lowers bodies nor proves complete C validity. Additional resolution and
unimplemented-feature failures can surface during IR generation, and
some invalid C is deliberately accepted under the project's permissive
policy. Use the original compiler to validate the source. See
[sema passes](../../wiki/concepts/sema-passes.md) and
[architecture](../../wiki/concepts/parser-architecture.md) for the checking
and lowering boundary.

## Implementation

| Stage | Code |
| --- | --- |
| Lexing | [`src/lexer.rs`](src/lexer.rs) |
| Preprocessing and files | [`src/pp/`](src/pp/), [`src/files.rs`](src/files.rs) |
| Parsing and AST | [`src/parser/`](src/parser/), [`src/ast.rs`](src/ast.rs) |
| Semantic analysis and lowering | [`src/sema/`](src/sema/), [`src/sema/module.rs`](src/sema/module.rs) |
| Typed IR and printers | [`src/ir/`](src/ir/) |

## Trophy case

C projects that slate-parser can parse with `--flavor=clang` on
`x86_64-unknown-linux-gnu`.

| Project                                              | Revision                         |
| ---------------------------------------------------- | -------------------------------- |
| [musl](https://musl.libc.org)                        | `v1.2.6-20-gf21a9653`            |
| [PostgreSQL](https://www.postgresql.org)             | `REL_19_BETA1-1128-g852fd5b86e1` |
| [CPython](https://www.python.org)                    | `v3.14.8`                        |
| [Mbed TLS](https://github.com/Mbed-TLS/mbedtls)      | `522e2e4`                        |
| [curl](https://curl.se)                              | `5b06840`                        |
| [Redis](https://redis.io)                            | `e1d7d50`                        |
| [libsodium](https://libsodium.org)                   | `1.0.17-RELEASE-1234-g75c6d552`  |
| [libuv](https://libuv.org)                           | `f87c8e4`                        |
| [nginx](https://nginx.org)                           | `5f54125`                        |
| [SQLite](https://sqlite.org)                         | `version-3.53.0-768-g0eaef28cf2` |
| [zstd](https://github.com/facebook/zstd)             | `10da6ba`                        |
| [PCRE2](https://github.com/PCRE2Project/pcre2)       | `a2b146a`                        |
| [Lua](https://www.lua.org)                           | `v5.5.1`                         |
| [libexpat](https://libexpat.github.io)               | `R_2_8_2-51-gdfdbaadf`           |
| [mimalloc](https://github.com/microsoft/mimalloc)    | `v3.5.3-1-g31d034d9`             |
| [libdeflate](https://github.com/ebiggers/libdeflate) | `v1.26`                          |
| [giflib](https://giflib.sourceforge.net)             | `6.1.3-4-ga8e3114`               |
| [libpng](https://www.libpng.org/pub/png/libpng.html) | `d1d0abe`                        |
| [cJSON](https://github.com/DaveGamble/cJSON)         | `fb16e5c`                        |
| [libyaml](https://github.com/yaml/libyaml)           | `0.2.5-16-g893682b`              |
| [zlib](https://zlib.net)                             | `e3dc0a8`                        |
| [QuickJS](https://bellard.org/quickjs/)              | `04be246`                        |
| [LZ4](https://github.com/lz4/lz4)                    | `0774d05`                        |
| [TinyCC](https://bellard.org/tcc/)                   | `2ba12e8`                        |
| [chibicc](https://github.com/rui314/chibicc)         | `90d1f7f`                        |
| [xxHash](https://github.com/Cyan4973/xxHash)         | `v0.7.4-1128-g680bf46`           |
| [utf8proc](https://github.com/JuliaStrings/utf8proc) | `0075ed7`                        |
| [yyjson](https://github.com/ibireme/yyjson)          | `757305b`                        |
| [c-ares](https://c-ares.org)                         | `v1.31.0-404-gf4156c12`          |
| [jq](https://jqlang.org)                             | `1.6rc2-766-gf13c1ef`            |
| [libevent](https://libevent.org)                     | `d82464a2`                       |
| [oniguruma](https://github.com/kkos/oniguruma)       | `v6.9.10-28-gf95747b`            |
| [cglm](https://github.com/recp/cglm)                 | `v0.9.6-66-g58d8c15`             |
| [stb](https://github.com/nothings/stb)               | `2c980bb`                        |
| [lmdb](https://www.symas.com/lmdb)                   | `700e10f`                        |

## Development

Run tools from the workspace root. The standard parser test gate is:

```bash
cargo nextest run --release --profile parser --no-fail-fast
```

FileCheck fixtures live under `crates/slate-parser/tests/fixtures/`. After
changing a fixture or its renderer, regenerate expectations with:

```bash
python3 crates/slate-parser/tools/update_filecheck.py --in-place <fixture>
```

Project work is tracked with `bd`; run `bd ready` for available tasks.
