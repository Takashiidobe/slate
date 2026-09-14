# Compiler flags

_Design notes, 2026-09-14 — proposals for discussion, not an implemented API._

Issue: `slate-parser-ixa.15`. Related: language modes (`ixa.6`) and typed
semantic pragmas (`ixa.17`). Companion documents: [IR Shape](ir-shape.md)
and [IR Spec](ir-spec.md). These notes capture the discussion before changes
to those specifications or implementation.

## Resolution contract

Compiler options are inputs to sema; resolved behavior is its output.
The IR module records the configuration that produced it, but that record
must not supply missing semantics. Rust lowering reads resolved operations,
types, layouts, and symbol properties, never compiler flags.

```text
compiler arguments + compiler-flavor defaults
    → translation-unit configuration
    → sema with scoped pragma/attribute overrides
    → resolved operations, types, layouts, and symbols
    → IR
```

Proposed invariant: removing configuration provenance must not change the
meaning of the remaining IR. Required target representation information
remains part of the module, not removable provenance.

## Configuration ownership

Prefer one owning configuration with separate groups over a flat
`LangOptions` containing unrelated settings.

| Category | Inputs | Resolved effects |
| --- | --- | --- |
| Operation semantics | `-fwrapv`, `-ftrapv`, `-fno-strict-overflow`, `-fno-delete-null-pointer-checks`, `-frounding-math` | Required behavior on relevant operations |
| Type meaning and layout | `-funsigned-char`, `-fshort-enums`, `-fshort-wchar`, `-fpack-struct`, `-mlong-double-*` | Effective `TargetInfo`, concrete types, layouts, and calling contracts |
| Language | `-std`, `-fms-extensions`, `-fdollars-in-identifiers`, `-fgnu89-inline` | Parsing and declaration meaning, including emitted definitions |
| Linkage | `-fcommon`/`-fno-common`, `-fvisibility` | Definition kinds and symbol visibility |
| Codegen settings | `-O`, `-g`, `-fstack-protector` | Applicable predefines and configuration provenance within this translation scope |

Applicable predefines must be generated from the same effective
configuration used by parsing and sema. Examples include `__OPTIMIZE__`,
`__NO_INLINE__`, and `__FAST_MATH__`; not every option defines a macro.
Defaults, option interactions, and supported local overrides depend on
compiler flavor. Preserve argument order when resolving them.

## Operation semantics

Proposed printer examples:

```text
add<i32, overflow=undefined>(a, b)
add<i32, overflow=wrap>(a, b)
add<i32, overflow=trap>(a, b)
float_add<f64, rounding=dynamic, exceptions=strict>(x, y)
```

Prefer an explicit source-level undefined-overflow contract (`undefined`
or the existing `ub` spelling) over `nsw`, which risks implying LLVM poison
semantics. A proof that overflow cannot occur remains a separate analysis
fact, not an integer type property.

Overflow rules are operation-specific. Wrapping addition does not imply
wrapping division or unrestricted shifts. GCC documents interactions among
`-ftrapv`, `-fwrapv`, and their negations; `-fno-strict-overflow` also implies
integer and pointer wrapping policies. Resolve those interactions before
assigning operation contracts. See [GCC code-generation options](https://gcc.gnu.org/onlinedocs/gcc-15.1.0/gcc/Code-Gen-Options.html).

Pointer offset behavior and the assumptions allowed after a memory access
need explicit contracts too. The exact representation for
`-fno-delete-null-pointer-checks` remains open; an integer overflow field
cannot express it.

Floating-point rounding, exception behavior, and contraction are separate
dimensions. Clang describes `-frounding-math` in terms of dynamic rounding;
`float_control` can change precision and exception behavior locally.
See the [Clang manual](https://clang.llvm.org/docs/UsersManual.html) and
[language extensions](https://clang.llvm.org/docs/LanguageExtensions.html).

The issue proposes conservatively retaining strict behavior for permissions
such as fast math, finite-only assumptions, reassociation, and optional
contraction, while still emitting their predefines. This is a translation
policy, not a claim that all floating-point settings are interchangeable.
Required rounding and observable exception behavior must still be honored.

## Local resolution

Sema carries an effective semantic environment through the AST.
Translation-unit defaults initialize it. Typed pragmas and function
attributes modify the applicable scope according to compiler-flavor rules:
`#pragma STDC FP_CONTRACT`, `#pragma float_control`, and
`__attribute__((optimize(...)))` are motivating examples.

Each operation receives its resolved contract under that environment,
including arithmetic synthesized for increments and compound assignments.
Constant evaluation must use the same environment. IR consumers do not
replay pragma push/pop events; local override origins can remain provenance.
This depends on preserving the typed pragma information in `ixa.17`.

Observable floating-point exceptions and dynamic rounding also constrain
sequencing, folding, and hoisting. The current IR spec's statement that
expressions are side-effect-free except calls needs refinement to account
for operations that observe or modify the floating-point environment.

## Layout and symbols

Keep policies distinct from their results. `-fshort-enums` supplies a
selection policy; each enum receives its concrete underlying type.
`-fpack-struct` supplies a default; each record receives concrete field
offsets and alignment after local packing and alignment overrides.
`TargetInfo` exposes effective target properties after ABI options are
applied. Rust consumes the resolved representations and calling contracts.

`-fcommon` requires a resolved definition kind for tentative definitions;
external linkage alone is insufficient. Visibility is a separate symbol
property. GNU inline rules likewise affect which definitions are emitted,
not just the spelling of an inline attribute.

## Module provenance and follow-up specification work

Proposed module header information: normalized translation-unit defaults,
compiler flavor, and optionally the original ordered arguments. Local
overrides are represented by their resolved effects and optional origins.

When the design is agreed, update `ir-shape.md` with configuration ownership
and concrete field proposals. Update `ir-spec.md` with the resolution
contract, operation rules, and sequencing requirements. Required operation
semantics must remain visible when optional metadata is hidden; current
examples that hide overflow behavior need adjustment.

At the time of this discussion, `TargetInfo` contains basic integer and
pointer widths and character signedness; IR and IR sema remain scaffolding.
These notes establish direction without claiming the flag integration exists.

Open choices include the exact operation field shapes, pointer/null-access
contracts, floating-point effect representation, compiler-flavor precedence
rules, and which configuration details belong in the printed module header.
