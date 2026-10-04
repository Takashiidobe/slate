# Open IR design

<!-- toc -->
- [Information ownership](#information-ownership)
- [Macro provenance](#macro-provenance)
- [String literal views](#string-literal-views)
- [Attributes as open occurrences](#attributes-as-open-occurrences)
- [Functions](#functions)
- [Module invocation record](#module-invocation-record)
- [Open questions](#open-questions)
- [References](#references)
<!-- /toc -->

Part of the [IR spec](../ir-spec.md). Agreed direction that is not
implemented yet, and open questions. This replaces the old `ir-shape.md`
proposal; its implemented parts were folded into the other subpages. When a
piece here lands, move it to the subpage for its area.

## Information ownership

The rule that decides where a fact lives. A side table does not by itself
make information optional.

| Information | Owner | Required? |
| --- | --- | --- |
| numeric value domain | the numeric type | yes |
| size, alignment, representation | type storage, with object/field overrides | yes when stored |
| storage duration, linkage | the declaration | yes |
| calling convention and ABI | the signature and each call | yes at calls |
| overflow and conversion behavior | the operation | yes |
| volatile/atomic access | the memory operation | yes |
| original C type, typedef chain | metadata on the type use | no |
| spelling, radix, suffix, macro/header origin | node metadata | no |
| proven ranges, known bits, no-overflow | analysis facts keyed by node | no |

Optional context may improve Rust names and idioms but must never be needed
to reconstruct arithmetic, storage, or calling semantics.

## Macro provenance

Planned: every value's origin carries an optional `ExpansionId` chain
(`INT_MAX` → `__INT_MAX__`, object- or function-like, definition site), so
Rust lowering can emit `c_int::MAX` for `<limits.h>` names or a `const` for
user macros, and turn a function-like expansion's nodes back into a `fn`.
Needs the preprocessor to record an expansion id per token and the parser to
propagate it into every expression node, `ConstExpr` included. Today only
atomic builtins record `c_macro`.

## String literal views

Planned: a string literal object carries three views of the same contents so
Rust picks the right one without re-encoding:

- `code_units`: decoded units including the terminator; the array length
  counts these. Embedded zeros are kept.
- `bytes`: the exact target representation in target byte order.
- `text`: lossless Unicode text without the implicit terminator, or `None`
  if not decodable (never replacement characters).

The literal object is `writable: false` as an access contract; the C element
type is not rewritten to `const char`. Object identity is independent of
content equality. Today only `code_units` exists.

## Attributes as open occurrences

Planned: attribute storage stays extensible. Every node that can carry
attributes (functions, variables, parameters, results, types, fields,
statements) keeps an ordered occurrence list: namespace, name, a token-tree
argument fallback, typed arguments when a handler exists (constant, string,
type, symbol, `ParamIndex`, identifier, list), and a resolution state
(`Uninterpreted`, `Resolved`, `IgnoredByCompiler`). Duplicates are kept.
Resolved effects move to their owner (signature, symbol, contracts) and keep
the occurrence id.

| Attribute information | Resolved owner |
| --- | --- |
| calling convention | function type (done) |
| symbol name, visibility, weak, import/export, section | symbol attributes (done) |
| alias, ifunc resolver | function implementation |
| noreturn (done), returns_twice | control-flow facts |
| constructor/destructor priority, target features, interrupt | execution requirements |
| nonnull, access, alloc_size, alloc_align, malloc (deallocator done) | parameter/result contracts (`alloc_size(1, 2)` → `Product(Argument(0), Argument(1))`) |
| pure/const | function contracts (done as `memory=`) |
| inline, hot/cold, optimize | hints |
| deprecated, nodiscard, format | hints/metadata |

Rules: declared contracts stay distinct from proven facts (nonnull does not
prove a valid borrow); function-pointer attributes stay visible at indirect
calls; lacking a handler never makes an attribute ignorable; a known
behavior-affecting attribute is either resolved or reported unsupported,
never silently dropped. Today functions keep unresolved attributes as
`c_attributes` metadata.

## Functions

- Implementation kinds beyond declaration and definition: `Alias` (symbol
  pointing at another function, not a wrapper body) and `Resolver` (ifunc).
- Old-style (K&R) definitions: keep promoted incoming types separate from
  the local binding types and convert at entry. An unprototyped declaration
  is neither zero parameters nor variadic.
- GNU nested functions: captures and static chains
  (`slate-parser-dyd.56`).
- Nonlocal exits: `returns_twice`, `setjmp`/`longjmp`, and unwinding need a
  shape; no default `nothrow` flag.
- A `musttail` requirement belongs on the call.
- `regparm(N)` is parsed onto the AST but not yet applied to the ABI; only
  the `-mregparm=N` default reaches `AbiSignature.regparm`.

## Module invocation record

Open: whether the module header should keep the argv, standard, and default
semantics beside the target. Today the ordered arguments stay on
`CompilerOptions`, and only resolved effects reach the IR.

## Open questions

- Function "type parameters": needs a concrete C use (`_Generic`,
  `<tgmath.h>`, type-generic macros) before it gets a slot.
- Whether the derived-fact analysis pass runs on this IR or stays on lowered
  Rust in Slate.

## References

The [historical index](../../historical/index.md) preserves earlier IR design
comparisons. This IR uses typed ids and concrete source-level signatures;
regions, logical fields versus storage units, and symbol/callable/body
separation are defined by the current specification.
