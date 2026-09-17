# Compiler flags

_Design notes, 2026-09-14 — agreed direction with illustrative shapes,
not an implemented API._

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

Floating-point rounding and exception behavior are separate required
dimensions. Clang describes `-frounding-math` in terms of dynamic rounding;
`float_control` can change precision and exception behavior locally.
See the [Clang manual](https://clang.llvm.org/docs/UsersManual.html) and
[language extensions](https://clang.llvm.org/docs/LanguageExtensions.html).

**Agreed:** drop optional transformation permissions from IR operations.
This is not an optimizing IR. `nnan`, `ninf`, `nsz`, `arcp`, `contract`,
`reassoc`, and `afn` need no operation fields. Their originating compiler
arguments may remain invocation provenance, and applicable predefines must
still be emitted. Ordinary arithmetic is a permitted implementation without
exercising these permissions; it need not reproduce a particular optimized
C binary's numerical results.

`nnan` does not change float representation or require a non-NaN wrapper,
runtime check, or unsafe assumption in Rust. Optional contraction does not
require fusion. An explicit fused operation does require it:

```text
C arithmetic with optional contraction: a * b + c
IR: float_add(float_mul(a, b), c)
Rust: a * b + c

explicit fused multiply-add:
IR: float_fma(a, b, c)
Rust: a.mul_add(b, c)
```

These Rust examples assume ordinary rounding and no observable FP
environment effects. Required rounding and exception behavior must still
be honored. Dropping fast-math permissions does not justify ignoring driver
or startup effects such as changes to denormal handling.

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

## Extensible semantic categories

Group resolved properties by meaning and attach them to their actual owner.
The categories are orthogonal; individual compiler flags need not be. One
flag can affect several categories, and several flags can affect one field.

| Category | Resolved properties | Owner |
| --- | --- | --- |
| Integer arithmetic | Overflow policy, division exceptional cases, shift rules | Arithmetic operation |
| Floating-point behavior | Rounding source, exception observability, evaluation precision | Arithmetic or conversion operation |
| Pointer semantics | Offset overflow, bounds/provenance requirements, null-access contract | Pointer operation or memory access |
| Memory access | Volatile, atomic ordering, access width, alignment | Load/store/copy operation |
| Type representation and ABI | Signedness, numeric format, size/alignment, field layout, calling convention | Types, objects, function signatures |
| Symbol and definition semantics | Linkage, visibility, common definitions, interposition, inline-definition behavior | Global/function declaration |
| Execution and instrumentation | Required initialization, runtime checks, trap/report behavior | Explicit operations and function properties |
| Language interpretation | Standard, extensions, builtin recognition | Frontend configuration; resolved effects flow into IR |
| Invocation provenance | Original arguments, normalized defaults, compiler flavor | Module header |

These groups describe semantic ownership, not a universal property bag
attached to every node. Language options are mostly consumed before IR.
Layout options change the resolved layout rather than merely adding a flag
to an unchanged type. Explicit source constructs, such as volatile and
atomic accesses, use the same semantic categories as option-derived effects.

### Extension rules

1. Resolve command-line order, flavor defaults, pragmas, and attributes into
   an effective context before assigning operation properties.
2. Keep mutually exclusive choices within one category. An applicable
   integer operation has one overflow policy: `undefined | wrap | trap`.
   `nuw`, `nsw`, and `trap` are not independent additive bits; LLVM no-wrap
   contracts and trapping behavior are different semantics.
3. Required semantic metadata cannot be silently ignored or hidden as
   optional provenance. An emitter that cannot implement a required
   property reports an unsupported case instead of emitting ordinary
   arithmetic as a fallback.
4. Operations carry their resolved contracts. Function metadata can supply
   the context used by sema, but consumers do not combine module, function,
   and operation defaults again.
5. Add only applicable fields to each operation family, and validate their
   combinations. Orthogonal categories do not make every combination valid.

Illustrative shapes:

```text
add<i32>(a, b) {
    integer: { overflow: trap }
}

float_add<f64>(x, y) {
    floating: {
        rounding: environment,
        exceptions: observable
    }
}
```

For a translation unit compiled with `-fwrapv`, a supported local override
may change the effective context for one function. Sema materializes the
difference, including synthesized increment arithmetic:

```text
function f, effective overflow policy = wrap:
    add<i32, overflow=wrap>(a, b)

function g, effective overflow policy = trap:
    add<i32, overflow=trap>(a, b)
    add<i32, overflow=trap>(i, 1)
```

This illustrates resolved contexts, not a promise that every compiler
flavor accepts the same function-attribute spelling or override combination.
Wrapping addition can emit `a.wrapping_add(b)`. Trapping addition needs a
checked operation and the selected trap behavior; an ordinary Rust `+`
whose overflow behavior depends on build settings is not that contract.

### Additional flag families to accommodate

These are extension candidates, not a commitment to implement all of them
in the first change:

- Builtin/library handling: `-fno-builtin`, individual builtin exclusions,
  `-ffreestanding`, and math `errno` requirements affect call recognition
  and whether replacing a call with a Rust operation is valid.
- Floating-point representation: excess precision, denormal handling, and
  defined float-to-integer overflow behavior need explicit resolved rules.
- Memory/layout: volatile bit-field access rules, Microsoft bit-field
  layout, scalar storage order, target ABI, and calling-convention options.
- Instrumentation: automatic variable initialization and sanitizer
  trap/recover modes. Decide which instrumentation is preserved and which
  is outside translation scope rather than silently treating it as metadata.
- Symbol resolution: interposition and weak/common definitions can require
  preserving an externally replaceable call or definition.

## FP environment and the existing Rust rewrite

**Agreed:** do not duplicate the existing Rust-to-Rust analysis in IR.
With the same preserved calls and semantic information, IR has no inherent
advantage in determining the runtime FP environment.

| Stage | Responsibility |
| --- | --- |
| Sema/IR | Record required rounding and exception behavior from flags and scoped pragmas |
| Initial Rust emission | Preserve calls through the existing `extern C` path and carry required operation contracts to the rewriter |
| Existing Rust rewrite | Prove lifting to higher-level Rust is valid; dynamic environment changes prevent lifting under the current conservative policy |

Recognizable environment functions can remain resolved calls. Dedicated
IR environment operations are unnecessary unless emission benefits from
them. Macros have expanded before this point; resolved calls and arguments
carry their effects, while macro names remain provenance.

Knowing the compiler option does not mean knowing the runtime mode.
`-frounding-math` requires observing the environment; `FENV_ACCESS`
controls environment-sensitive semantics rather than selecting a direction.
Distinguish a resolved operation contract from a proven environment value:

```text
float_add<f64, rounding=nearest_even, exceptions=ignore>(a, b)
float_add<f64, rounding=environment, exceptions=observable>(a, b)
```

The second contract is complete even though the runtime mode is unknown.
For example, source calls remain calls through the pipeline:

```text
call fesetround(FE_TOWARDZERO)
call external_function()
x = float_add<f64, rounding=environment, exceptions=observable>(a, b)
```

The rewriter must not infer the mode at `x` merely from the first call:
the external call may change it, and setting the mode can fail. Likewise,
no changes inside a function alone do not establish its incoming environment.
The existing lifting guarantee must cover that boundary.

Preserving `fesetround` through `extern C` does not by itself make an emitted
Rust `a + b` honor dynamic rounding. Initial emission must implement the
operation contract through a suitable helper, emulation, or another valid
mechanism until lifting is justified. Evaluating ordinary arithmetic and
adjusting afterward can lose necessary rounding information and exception
effects. The transport of required semantic properties into the existing
Rust rewrite remains an implementation choice.

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

The first implementation seed now lives in `src/sema/`, which resolves AST
numeric literals, scalar casts, promotions, mixed-type arithmetic, bitwise
and shift operators, unary `-`/`~`, and comparisons and logical operators
producing `bool` directly into typed IR. `src/ir/`
owns nodes, spans, printing, and separate integer-overflow and floating-point
properties. There is no intermediate semantic AST. `CompilerOptions` now
groups operation settings, layout overrides, and ordered argument provenance.
The parser retains it on `TranslationUnit`; sema materializes its operation
contracts without making IR consumers interpret options. See the
[numeric seed](ir-spec.md#implemented-numeric-seed) for supported scope.

The first flag hookup accepts positive/negative wrapv, trapv, strict-overflow,
rounding-math, and trapping-math options for Clang/GCC flavors. GCC uses the
last active wrap/trap setting (negation can reveal an earlier active option);
Clang gives active trapv precedence over wrapv, checked against Clang 22.
Clang defaults to ignored FP exceptions; GCC defaults to observable ones.
Rounding and exceptions remain independent. GCC's `-frounding-math` defines
`__ROUNDING_MATH__=1`; Clang does not. Other supported operation flags add
no predefines.
MSVC flavor rejects these spellings rather than claiming compatible behavior.

`-mlong-double-64/80/128` changes the effective `TargetInfo.long_double` and
the selected x86 Linux target's long-double predefines together, before
user `-D` definitions. The option is rejected for the supported AArch64 and
ARM32 Linux targets. Sema parses the original literal directly at the
selected precision. f80 has 80 value bits with 16-byte storage on x86_64 and
12-byte storage on x86.
The target layout is selected by the supported target triple and is printed in
the IR module header. GCC's `-mpreferred-stack-boundary` and Clang's
`-mstack-alignment` update the target's stack ABI policy after validating their
compiler-specific value rules; they do not change ordinary scalar object
storage alignment. Local `aligned` and `_Alignas` attributes remain layout
overrides on individual objects, fields, and aggregate types.

```sh
cargo run -- parse tests/fixtures/sema/numeric_seed.c --dump-ir-expressions -fwrapv -frounding-math -ftrapping-math
cargo run -- parse tests/fixtures/sema/long_double_128.c --dump-ir-expressions -mlong-double-128
```

Pointer wrapping implied by strict-overflow flags is retained in the
configuration for future pointer operations. Other flag families, local
pragma/attribute overrides, and the eventual IR module provenance header
remain unimplemented; this does not complete `ixa.15`.

Open choices include the exact operation field shapes, pointer/null-access
contracts, floating-point effect representation, compiler-flavor precedence
rules, and which configuration details belong in the printed module header.
