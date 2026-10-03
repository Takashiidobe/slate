# Inline asm parsing

<!-- toc -->
- [Recognition](#recognition)
- [GNU asm: parser](#gnu-asm-parser)
- [GNU asm: sema](#gnu-asm-sema)
  - [x86_64 clang coverage](#x86_64-clang-coverage)
- [MS `__asm`](#ms-__asm)
<!-- /toc -->

slate-parser reads two inline asm syntaxes: GNU `asm` (gcc and clang
flavors) and MS `__asm` blocks (msvc flavor, and clang with MS extensions).
Both lower to one `ir::InlineAsm`, specified in [ir/asm](ir/asm.md). MS block
syntax, effects, and implicit returns are in [msvc-asm](msvc-asm.md). This
page covers what each flavor accepts and where the flavors diverge. Slate's
lowering of the IR is tracked under epic `slate-3f8g.4.17`.

## Recognition

| Form | gcc | clang | msvc |
| --- | --- | --- | --- |
| `asm`/`__asm`/`__asm__`, qualifiers, `(` | GNU asm | GNU asm | accepted, but cl.exe rejects it (`slate-parser-asmmsv`) |
| `__asm`/`_asm` not followed by `(` or a qualifier | never | MS block on x86/x86_64 with `microsoft_extensions` | MS block, i686 only (x64 is C4235) |
| `asm {` | no | not modeled | not a keyword |

- Plain clang, not only clang-cl, accepts MS blocks: `-fms-extensions` or
  `-fasm-blocks` on any x86 target. The parser gates on
  `microsoft_extensions`, so `-fms-extensions` works and `-fasm-blocks` is
  still an unknown option (`slate-parser-asmblk`).
- `asm` stays a keyword under strict `-std=cNN` in gcc and clang.
- File scope: basic GNU asm only in clang (`expected ')'` on operands); gcc
  accepts operands. Qualifiers are rejected in both. Lowers to `Module::asm`.
- `T x asm("sym")` is an asm label (`asm_name`). With `register` it binds a
  register: `Variable::register`. clang on x86_64 accepts global register
  variables only for `rsp`/`rbp` at matching size; Slate lowers reads to an
  `asm!` register read, and writes are a barrier.

## GNU asm: parser

`src/parser/asm.rs`. The template is the concatenation of adjacent narrow
string literals; wide and unicode literals are rejected.

- Template pieces: `%%`, `%=`, `%N`, `%[name]`, `%<letter>N` (letter kept as
  a modifier), and labels. Label operands number after all operands, so
  `%lN` and `%l[label]` resolve to `Label(n)`. `%l` on a non-label, an
  operand number past the end, and an unknown name are errors.
- x86 only: `{att|intel}` alternation, nested alternation rejected; `%{`,
  `%|`, `%}` are literal. Off x86 the braces are plain text.
- Constraints split on `,` into alternatives. Modifiers `=`, `+`, `&`, `%`,
  `-`; `{reg}` is a hard register, digits or `[name]` a match, anything else
  a letter string resolved in sema.

| Check | gcc | clang |
| --- | --- | --- |
| `{reg}` hard-register constraint | accepted (gcc 15) | invalid constraint |
| input matching different outputs across alternatives | accepted; ties only when every alternative matches | rejected; any matching alternative ties |
| `-` (PIC) modifier | file scope only | file scope only |
| two inputs tied to one output, a tie to a `+` output or past the outputs, `=`/`+`/`&` on an input, output without `=`/`+` | rejected | rejected |

## GNU asm: sema

`src/sema/asm.rs` resolves letters per target family, picks one alternative
for the whole asm, folds ties, and computes Rust options; the rules are in
[ir/asm](ir/asm.md#alternative-selection). Flavor splits:

| Rule | gcc | clang |
| --- | --- | --- |
| memory-only operand naming a register variable | error | accepted |
| bit-field or vector lane under a memory-capable constraint | spills | rejects (not followed) |
| thread-local as a symbol operand | prints the symbol | not a link-time address |
| const tentative definition in an immediate | folds as zero | unfolded |
| `memory` option of volatile extended asm without memory operands | `nomem` | `Any` |

### x86_64 clang coverage

Audited 2026-10-03 against clang 23.1.1 `-O0 -c`, one probe per feature.

- Resolved: `r q Q R a b c d S D`, `x` (widened to ymm/zmm by width), `v`,
  `k`, `Yz`, `m o g rm imr`, `i n I J K L M N O e Z`, ties, `%`, hints
  `?!*#`. Modifiers `b h w k q x t g` become register views; `a c n P p V H
  A` are kept for lowering. Clobbers decode to canonical registers plus
  `cc`, `memory`, `unwind`.
- Misresolved: every `=@cc<cond>` flag output parses as letters, and
  `@ccg`/`@ccge` select `reg` (`slate-parser-asmcc`).
- Unresolved but accepted by clang: `X`, `p`, `s`, `Ws`, `V`, `l`, `A`,
  `Yi`, `Yt`, `Y2`, `Ym`, `Yk` (`slate-parser-asmlet`). `<`, `>`, `E`, `F`,
  `C`, `G`, `Y0` stay unresolved on purpose: clang rejects the operands or
  its backend fails.
- A register-variable operand resolves to plain `reg`; clang pins the
  variable's register (`slate-parser-asmrv`).
- Looser than clang: out-of-range `L`/`M` immediates, clobbers `eflags`,
  `es`, `fs`, `redzone`, and `=f` outputs are accepted; `-mapxf` is unknown
  (`slate-parser-asmlax`).
- x87 (`t`, `u`, `f`) and MMX (`y`) operands resolve but are clobber-only in
  Rust, so selection rejects them.

## MS `__asm`

`src/parser/ms_asm.rs` and `src/sema/ms_asm*.rs`; see
[msvc-asm](msvc-asm.md). For one block with C operands, byte displacements,
a local label, and EAX and EDX:EAX implicit returns, the clang flavor on
`i686-unknown-linux-gnu -fms-extensions` gives the same IR as
`i686-pc-windows-msvc`.

- Differential oracle: plain `clang --target=i686-linux-gnu -fms-extensions`
  runs natively. `MSVC_ARCH=x86 tools/cl.exe` under Wine is the msvc-flavor
  cross-check ([msvc-oracle](msvc-oracle.md)). Both print the same output for
  that block.
- x86_64 clang accepts blocks but has no implicit return, so those tests are
  i686-only.
