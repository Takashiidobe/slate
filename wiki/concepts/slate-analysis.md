# slate-analysis

<!-- toc -->
- [Boundary](#boundary)
- [Where a rewrite belongs](#where-a-rewrite-belongs)
- [Transform stage](#transform-stage)
  - [Pass scopes](#pass-scopes)
  - [Const-fold](#const-fold)
  - [Truth canonicalization](#truth-canonicalization)
  - [Truth locals](#truth-locals)
  - [Labeled blocks](#labeled-blocks)
  - [Casts are lowering, not a
    transform](#casts-are-lowering-not-a-transform)
- [Keys](#keys)
- [Call resolution](#call-resolution)
- [Scaling rule](#scaling-rule)
- [Pointer capabilities](#pointer-capabilities)
- [String views and libc calls](#string-views-and-libc-calls)
- [Byte buffers and mem\* calls](#byte-buffers-and-mem-calls)
- [C-string arrays](#c-string-arrays)
- [Effect summaries](#effect-summaries)
- [Def-use](#def-use)
  - [Counted loops](#counted-loops)
  - [Inline temporaries](#inline-temporaries)
- [Shadow mode](#shadow-mode)
- [Related](#related)
<!-- /toc -->

`crates/slate-analysis` runs whole-program analyses over slate-parser's
`ir::Module`s and hands their results to `crates/slate`'s lowering as read-only
facts. It replaces analyses that today re-derive C meaning from Rust spellings
after lowering (`backend/interproc`, `backend/engine/rules`). Tracked by
`slate-8ge7`.

```text
ir::Module per unit -> Program (one target) -> analyze -> Facts
  -> lowerer::lower(module, ..., facts.unit(id)) per unit, in parallel
```

## Boundary

- Input is one target's program: every unit of one configuration, after
  `project::merge::without_definitions` has dropped definitions owned by other
  units. Multi-target `--merge` joins per-target translations after lowering;
  the analysis never sees two targets at once.
- Analyses read the IR only. They never read `rust_ast`, spellings, or another
  analysis's output from the Rust side.
- A fact is a statement about a C entity that holds for the whole program and
  that lowering needs to choose a Rust form: a binding's pointer capability, a
  value's proven length. A fact never names a Rust type or rewrite; lowering
  decides the representation from it.
- Facts are computed once by `analyze` and are immutable afterwards. Nothing
  mutates the store during lowering, and lowering never feeds facts back.
- A missing fact means "unknown" and must lower to the transliterated form.
  An analysis that cannot see a callee or a use gives up for that entity, not
  for the program.
- C semantics the checker needs stay in slate-parser's sema. Facts that only
  idiomization needs move here (`slate-8ge7.8`): `counted_loops` was the first.

## Where a rewrite belongs

The transform stage (`slate-8ge7.24`) puts IR -> IR canonicalizations ahead of
the facts: analyses the transforms need, then transforms, then the fact
analyses once, then lowering. The goal is a simpler lowerer, not optimization:
every transform leaves IR a C programmer could have written. Decide by what
the rewrite's precondition reads:

| Precondition | Place | Examples |
| --- | --- | --- |
| C meaning only, and the result is still C | IR transform | const-fold, truth canonicalization, dead code, decl-init |
| C meaning, but the result is a Rust form | fact; lowering picks the form | pointer capability, counted loops, `mut`, libc callees |
| One IR node already decides the spelling | lowering emits it directly | `+=`, `while`, `.add()`, `.is_null()`, `c""` |
| The Rust form the lowerer chose | Rust rule after lowering | `unsafe` scoping, `MaybeUninit` slots, Rust scopes |

A rule never sits half in each layer: port it whole or leave it. The
2026-11-21 triage of `backend/engine/rules` (`slate-8ge7.24.3`) assigns each
rule a place and a port bead.

## Transform stage

`slate_analysis::transform` (`slate-8ge7.24.1`) holds the IR -> IR passes.
`PASSES` lists each pass with its name and the slots it runs in, and
`Passes::run(module, slot)` runs the selected passes of one slot in list
order. `frontend::lower_parsed` calls two slots per unit:

- `Slot::BeforeMacroKeep`, after `sema.lower` and before
  `macro_keep::kept_expansions`. Every `Statement::MacroExpansion` and wrapped
  value (`Module::macro_values`) is still present and a pass must not move
  anything into, out of, or across one. Passes here change what macro_keep
  surveys (temporaries are gone before its `local_used_after_macro` and
  `argument_reads_macro_local` checks).
- `Slot::AfterMacroStrip`, after `strip_macro_structure` and
  `pass_caller_locals`. Only kept expansions remain; the code of dropped ones
  is ordinary statements now.

A pass that needs callee summaries from other units would run at a
whole-target slot after `project::merge::without_definitions`; none does yet.

### Pass scopes

A `Pass` declares `decls: fn(&mut Module)` for globals and types, `function:
fn(&mut Function, &mut PassContext)`, or both (`slate-8ge7.24.17.1`). The
runner iterates; a pass never loops over `module.functions` itself.

- Each top-level function runs first, then each inner function (a `^{}` block
  literal or a GNU nested function) runs as a function of its own, with its
  parameters, and its captures as names it does not own. The runner recurses,
  so a block inside a block runs after its enclosing block.
- Inside a function, passes walk with `walk_function_body` /
  `walk_function_value` (and the `_with_macros` forms). These stop at an inner
  function and call `Visitor::capture(binding, kind)` once per captured outer
  binding at the literal: `CaptureKind::Escape` for a `__block` capture and
  every GNU nested-function capture, `CaptureKind::Read` for a by-value block
  capture. The plain `walk_body` still descends into inner bodies, because
  sema, macro strip and the lowerer need that.
- `PassContext` carries the module (with `functions` taken out), `captured`
  (bindings a record field type or a macro hidden argument names), `visible`
  (names visible from enclosing scopes: globals, function names, and for an
  inner function its enclosing functions' parameters and locals), and
  `retire(nodes)` for side tables.
- Passes treat a capture conservatively: truth-locals keeps any captured local
  `int`, because `BlockCapture::ty` and the inner body keep its type.
  inline-temps rejects a captured temporary. Nested-function captures are
  always `Escape` until nested functions lower (`slate-8ge7.24.17`); then a
  capture never written after the definition could become `Read`.

Fixtures: `fixtures.transforms/{truth-locals,decl-init,structure-goto}/block.c`.

Invariants: a pass moves nodes and never clones or rebuilds them, so every
surviving node keeps its `NodeId` (side tables such as `macro_written_casts`
stay valid); a new node gets a fresh id; slate-parser's printed IR is
unchanged, since the passes run in `crates/slate`'s pipeline only. Fact
analyses read the transformed IR and never see a removed node.

`slate emit-slate-ir --passes=<a,b|all> <file.c>` prints the IR after the named
passes (no flag: none, which is sema's output after the macro strip). Fixtures
live in `crates/slate/tests/fixtures.transforms/<passes>/*.c`: the directory
names the passes, the C is the input, and the generated block checks every
function body with `CHECK-LABEL`. Test: `transform_filecheck` in the `slate`
profile; regenerate with `crates/slate/tools/update_filecheck.py`, which turns
binding and type numbers into FileCheck variables.

### Const-fold

`const-fold` (`transform/const_fold.rs`, `slate-8ge7.24.4`) runs in
`Slot::AfterMacroStrip` only. It folds the conversions the C source does not
spell and nothing else:

- `widen`, `truncate` or `reinterpret` with any reason but `explicit`, over an
  integer constant, becomes `const<T>` with C's conversion (wrap to `T`'s width,
  two's complement for signed `T`); `char c = 200` is `const<i8>(-56)`.
- `neg` of a constant with a signed type becomes a negative constant, since C
  has no negative literal; `-1` is `const<i32>(-1)`.
- A comparison of two integer constants becomes `const<bool>` unless it is an
  explicit `(_Bool)` conversion (`slate-8ge7.24.6`); `_Bool f = 1` is
  `const<bool>(true)`, and lowering spells a bool constant without a cast.
- Arithmetic and shifts on constants stay as written (`60 * 60`, `1 << 18`,
  `FE_ALL_EXCEPT`'s `|` chain): rustc folds them, and the output keeps the C's
  meaning. Explicit casts stay, no-op ones included (see
  [Casts are lowering](#casts-are-lowering-not-a-transform)).
- Values inside a kept expansion or a `Module::macro_values` region, and
  conversions of a macro-valued constant, are left alone.

The folded node keeps the conversion's `NodeId`. Lowering spells
`const<T>` as `Cast(literal, T)`; codegen prints a literal cast to a plain
integer type it fits as a suffixed literal (`97i8`), `literal_casts` drops the
suffix where the context types it (`let c: i8 = 97`), and
`literal_casts::bare_initializers` does the same for a static whose declared
type is `T`. A constant array index or lane is written bare by
`FunctionLowerer::lower_index` when it fits the target's `usize`.

### Truth canonicalization

`truth-canonicalize` (`transform/truth_canonicalize.rs`, `slate-8ge7.24.6`)
runs first in `Slot::AfterMacroStrip`, before `inline-temps`, and replaced the
Rust rules `bool_simplify` and `bool_ternary`. Sema already types every
condition, `&&`, `||` and `!` operand as `bool` and spells C's int round trips
as `from_bool` and `ne(_, 0)`, so each rewrite keeps the value in any context,
not only a truth context:

- `ne(from_bool(b), 0)` is `b`; `eq(from_bool(b), 0)` is `not(b)`.
- `not(not(b))` is `b`; `not(const<bool>)` folds.
- `not` of `eq`/`ne` inverts the comparison for any operand type (exact under
  NaN); `not` of `lt`/`le`/`gt`/`ge` inverts only for integer, bool or pointer
  operands, so `!(a < b)` on doubles stays.
- `conditional<int>(c, 1, 0)` is `from_bool(c)`, `(c, 0, 1)` is
  `from_bool(not(c))`, `(c, 1, from_bool(y))` is `from_bool(c || y)` and
  `(c, from_bool(y), 0)` is `from_bool(c && y)`; the same shapes with `bool`
  arms drop the `from_bool`.
- Sema lowers `a || f()` as `let t: bool; if a { t = true } else { t = f() != 0 }`.
  When both arms write one `Temporary` of type `bool` this becomes
  `let t = a || f() != 0`, which `inline-temps` then moves into its use. A
  user variable written that way stays an `if`.

Nothing is rewritten that removes an `explicit` conversion or comparison, a
`Module::macro_values` node, or a node inside a kept expansion.

Lowering uses the same rules through `transform::negation` for loop exit tests
(`if !c { break }`) of a `do`/`while` and of a `while`/`for` whose condition
has hoisted statements; a head test with none lowers to `while c` directly. A
`break` the C source wrote at the loop head stays `if c { break }`: no rule
folds it into the condition. Negations that the backend's goto structuring builds
(`if !(i < 5) { break }` from `if (i < 5) goto top`) are not canonicalized.

### Truth locals

`truth-locals` (`transform/truth_locals.rs`, `slate-8ge7.24.15`) runs last in
`Slot::AfterMacroStrip`, after `decl-init`, and retypes an automatic integer
`let` to `bool` when C only ever stores 0 or 1 in it (`int found = 0; ...
found = 1;`, `int lt = a < b;`). The retyped local is declared `bool`, each
`ne(read, 0)` becomes `read<bool>`, each `eq(read, 0)` becomes `not`, and any
other read becomes `from_bool<int>(read<bool>)`.

- Every definition (initializer, `auto_init` zero, or a plain `write`) must be
  `const` 0 or 1, a `from_bool`, a `conditional` whose arms all qualify, or a
  read of another truth local. Locals that read each other are solved with an
  optimistic fixpoint. Anything else keeps the local `int`, so calls
  (`int r = rand()`), arithmetic, `++`, a stored 2, or a missing initializer
  all do.
- Every reference has to be a plain read or write. Address-taken, `Store`
  (`if ((x = a < b))`), `Update`, atomic access, `volatile`, `register`,
  `cleanup`, `__block`, statics, a capture by a block literal or nested
  function, and any reference inside a macro region or an OpenMP region keep
  the local `int`. A definition whose
  node is a `Module::macro_values` key does not count as proven.
- Truth uses must strictly outnumber integer reads, because a tie only moves
  the cast. A copy into another truth local (`int same = lt;`) counts as a
  truth use.

Fixtures: `fixtures.transforms/truth-locals/`, `fixtures/int_truth_locals.c`.

### Labeled blocks

`labeled-blocks` (`transform/labeled_blocks.rs`, `slate-8ge7.24.10.7`) runs in
`Slot::AfterMacroStrip` after `do-while-zero`. It recovers if/else and `match`
arms from the labeled blocks that `structure-goto` and `do-while-zero` build:

- sink: a `block L { ... }` whose body ends in a jump and has exactly one
  `break L` takes the statements after it, up to the first jump (or up to the
  end of an enclosing block or loop body, plus that exit's jump), in place of
  that break. The block itself disappears. It is skipped when the block has a
  cleanup or VLA `Let` or a `__try`, or declares a C name that is also used
  outside the block, since lowering keeps C names and the moved code could
  read the wrong one.
- else arm: `if c { A; break X } B...` becomes `if c { A } else { B... }` when
  falling off the list also reaches `X`, or when the list ends in `break X`.
  A then arm left empty is swapped with the else arm under
  `transform::negation`. `continue` exits are left alone, so `if (c) continue;`
  keeps its early `continue`.
- a trailing `break` to the enclosing block is dropped, a block with no breaks
  left is removed, a switch's case siblings move into the `Case` body, and
  after sinking finishes, case arms that end in `break L` with the switch at
  `L`'s end switch to `break switch`.

The tail moves and is never duplicated, so a label reached by two gotos stays
a block, and a `--merge` variant with a different goto count gets a different
shape (`fixtures.merge/splice_statements.c`). Fixture:
`fixtures.transforms/structure-goto,labeled-blocks/diamonds.c`.

### Casts are lowering, not a transform

`slate-8ge7.24.5` planned a `cast-canonicalize` transform to replace the Rust
rule `peel_casts`. Measured on the 3067-fixture corpus it has nothing to do:

- An implicit `pointer_cast` of an implicit `pointer_cast` never occurs; sema
  emits one conversion per C conversion. 5.7k of `peel_casts`' ~7k fires were
  casts the lowerer itself added (`b"x".as_ptr() as *mut i8` under a C
  `const char *` conversion, `buf.as_mut_ptr() as *mut i8`).
- Whether an implicit same-type conversion can go depends on its Rust
  spelling. `long f(int v) { return v; }` is `i32 -> i32` on LLP64, but under
  `--merge` the result lowers as `c_long`, and dropping the cast on one target
  splits the function per target. Only the lowerer sees that, so the
  implicit same-type `reinterpret` arm in `lower_value_node` stays, keyed on
  the lowered types.

Lowering emits each cast once instead: array decay is `buf.as_mut_ptr()` with
no `as *mut T` when the method already returns the C pointer type; a C pointer
conversion of an array decay retargets the decay's cast (`b"x".as_ptr() as
*const i8`); an integer conversion of a layout query retargets the query's cast
(`size_of::<T>() as i32`, exact for any `usize`), and a query of type `usize`
has no cast. Explicit conversions stay, same-type ones included: `(size_t)n` is
`u64 -> u64` on LP64 but not on i686 or LLP64, and `--merge` lifts one spelling
across targets. `(int *)(char *)v` keeps both casts, and float or
narrow-then-widen pairs were never collapsed.

## Keys

| Entity                          | Key                                    | Why                                                                                |
| ------------------------------- | -------------------------------------- | ---------------------------------------------------------------------------------- |
| Parameter, local, global, temp  | `ProgramBinding { unit, binding }`     | `BindingId` is a dense counter per unit's resolver, so it repeats across units     |
| Value or statement node         | `NodeId`                               | allocated from one process-wide counter, unique across units and targets           |
| Place                           | the enclosing value or statement's `NodeId` plus its root binding | places carry no id; locals and params are keyed by their root binding  |
| Defined function                | `FunctionId`                           | dense index into `Program::functions`, for per-function tables in `Vec`s           |

`UnitFacts` is one unit's view of the store, so lowering looks facts up by the
`BindingId`s and `NodeId`s it already holds.

## Call resolution

`Program::callee(unit, &Callee)` returns the `FunctionId` whose body the call
runs, or `None` when the callee is opaque:

- A direct call to a function defined in the same unit resolves to it.
- A direct call to an external declaration resolves to the external
  definition with the same link name (`asm_name`, else the C name) in another
  unit.
- Weak, `alias`, `weakref`, `ifunc` and `inline_only` definitions are opaque:
  the linker or another unit may replace the body the analysis would see.
- Indirect calls are opaque. Address-taken functions are an analysis input,
  not a resolution.

`None` is always sound: the analysis treats the call as escaping its pointer
arguments.

A declaration's libc identity (`Known`, `FunctionIdentity`) is also a fact:
`analyze` classifies each function once from its declaration provenance (the
system header it came from, resolved through the unit's `Files`) and
`module.hosted`, and lowering reads `UnitFacts::identity` instead of
classifying. The catalog lives in `slate-analysis/src/identity.rs`; slate
re-exports it from `function_identity`, where `CallBinding` (a Rust-AST
concept) stays.

## Scaling rule

Analyses here must stay near-linear (`n` or `n log n` in IR size) on
`tools/benchmark/interproc_scaling.py`, including the adversarial shapes:

- Per-function summaries, computed once and in parallel.
- Propagation over the call graph or over alias classes (union-find), with
  finite-height lattices; no per-call-site re-analysis of a callee.
- One summary per function, not per calling context: a C function gets one
  Rust signature, so context sensitivity buys nothing for representation.
- Flow-sensitive facts (`&mut` exclusivity, nullness, lengths at a point) come
  from a per-function CFG that is built and dropped per function.

Precision is measured by the benchmark's `output #lifted` column and the
give-up fixtures `pointer_lift_local_copy_chain.c` and
`pointer_lift_reassigned_local.c`.

## Pointer capabilities

`pointer::analyze` (`slate-8ge7.6`) gives every pointer parameter of a
candidate function and every named pointer local a `PointerCapability`
(write, unique, free, offset, escape, nullable, string, raw, byte elements).
The capabilities are `pointer-capability-lattice`'s flags and stay internal to
the analysis. `BindingFacts::pointer` carries the representation lowering
emits: `borrow` (`&T`, `&mut T`, or a slice when `many`) and `length` (a
companion integer parameter that is dropped and rebuilt from `len()`).
Lowering always reads them; the Rust-side `pointer_lattice`, `length_lattice`
and `string_params` passes and the `SLATE_IR_POINTERS` gate were deleted in
`slate-8ge7.7`.

- Per-function summaries, built in parallel (rayon): one node per pointer
  parameter, local, temporary, call result and return value. Statement order
  is ignored: a C binding becomes one Rust binding, so its type must fit every
  use.
- Inside a function, copies unify nodes into classes (union-find). A class
  collects use flags from its members: write through `*p`, `p[i]` and
  `p->f = ..`; offset from `p[i]`, `p + n`, `p - q`; escape from stores into
  memory or globals, aggregates, pointer-to-integer conversions, `&p`, opaque
  calls, intrinsics, atomics, anything inside OpenMP, and every capture of a
  block literal or nested function (a by-value block capture copies the
  pointer into the block, which can write through it later); nullable from
  comparisons with null and null assignments.
- An `asm` statement is an opaque call over its operands, the bound gcc, clang
  and Rust's `asm!` all assume. A pointer value as an operand (`"r"(p)`,
  `"D"(dst)`) escapes, unless the template is empty (the barrier idiom
  `"" : : "r"(p) : "memory"` executes nothing, so it only reads `p`). A pointee
  lvalue operand (`"+m"(*p)`, `"=b"(*out)`, `"r"(*p)`) is a place use: input
  reads, output writes, in-out does both. An operand whose object is itself a
  tracked pointer escapes when read and becomes `raw` when written. The
  `"memory"` clobber, symbol operands and goto labels change no parameter that
  is not an operand. Lowering passes a borrowed param to a memory operand as
  `p as *mut T` (or `as_ptr()` for a slice).
- A call-site proof from `&x` or a decayed automatic array holds only if no
  other pointer to `x` can be live: `x`'s address must not leak anywhere in the
  caller (a copy into a pointer variable, a store, an escaping use, an asm
  operand, or another call whose parameter escapes or is returned). Leaks are
  flow-insensitive, so `int *q = &x;` anywhere in the caller pins every
  `f(&x)`. A static array is a length proof only when it is `const`; a mutable
  static can be reached by name during the call (`slate-8ge7.16`).
- `&x.a.b` and `&__real__ x` prove a borrow like `&x` when every field on the
  path is a non-bit-field member that the IR layout does not mark
  `underaligned` ([ir/types](ir/types.md#records)); the leak and alias checks
  use the root object `x`, so `f(&s.a, &s.b)` aliases. A `char` in a packed
  record lifts; an `int` under `packed` or `#pragma pack(2)` stays raw. A
  part proof also needs the parameter's class to have no `offset`: C lets a
  field pointer reach its enclosing object (`container_of`), which a Rust
  borrow of the field cannot. Union fields borrow inside `unsafe`.
- `raw` is per binding and flows forward along copies: a binding assigned a
  moved pointer (`p + 1`, `&p->f`), an opaque value (call to an unresolved
  function, a load from memory), or a non-candidate parameter cannot be a
  reference, but its source can still be.
- Across functions, values flow from arguments to parameters and from return
  values to call results. Use flags (write, offset, free, string, shared,
  escape) propagate against the flow, so a caller learns what its callees do
  through its pointer; nullable propagates with the flow. Each flag is one
  reachability pass, so the join is linear. Unifying across calls instead
  merged every caller of a helper such as `sqlite3Malloc` into one class.
- Candidates (functions whose signature may change): defined, not `main`, not
  variadic, address never taken, and not fixed. A function is fixed when
  `lowerer::exports_symbol` (passed to `Program::new`) exports it, except in a
  closed program (an executable project, `liftable_root`), where an export is
  fixed only if its symbol is pinned: defined twice, the target of an
  `alias`/`weakref`/`ifunc`, named in global asm, declared elsewhere with a
  different name, arity or pointer-ness, or carrying `section`, `used`,
  constructor/destructor, target features or a non-C calling convention.
  Arguments to any other callee escape.
- Cross-unit callers are ordinary call sites: a declaration resolved to a
  candidate gets the definition's parameter facts, so its extern declaration
  and its call sites lower with the same borrow and dropped length. Link must
  then bind those calls through a Rust path; an extern declaration left with a
  slice or `&CStr` parameter is a panic, because neither has a C ABI.
- Two arguments of one call that share a class or the same `&local` object
  alias; if either parameter is written, both escape.
- Libc calls use the declaration's identity: `free` frees, `mem*`/`str*cpy`
  write argument 0, `str*` reads mark string and offset, other known
  functions escape.
- `c_string` (`slate-8ge7.7`, replacing the Rust-side `string_params`): a
  candidate's byte-pointer parameter becomes `&core::ffi::CStr` when its class
  is string-marked, never written, freed, escaped, nullable, returned, raw or
  in a macro, and every call site (none pinned) passes a NUL-terminated
  literal with no interior NUL or an unwritten `c_string` parameter of the
  caller (a shrinking fixpoint). Reads lower to `s.as_ptr()`, so `strlen`,
  `p[i]` and `memcpy` stay inside the `CStr`'s provenance, terminator
  included; call sites pass `c"..."` or the caller's `&CStr`. Not `&str`: a
  Rust `"abc"` has no NUL, so `strlen(s.as_ptr())` would read past it.

Measured (`SLATE_INTERPROC_TIMING`, pass `ir_pointer`): N=800 on every
`interproc_scaling.py` shape at most 1 ms (Rust-side `pointer_lattice`
extract 2-4 ms); sqlite 10.9 ms for 17,957 nodes (Rust-side extract 144 ms,
validate 245 ms).

## String views and libc calls

Libc idioms lift to Rust methods on a string view; they never fold to the
result. `strlen(s)` becomes `s.len()`, not `7u64`. The plan for byte buffers in
general is `slate-8ge7.24.19`; stage 1 (`slate-8ge7.24.19.1`) covers borrowed,
static strings.

`constant_strings::analyze` gives a pointer local
`BindingFacts::constant_string` when it is a plain automatic local, DefUse sees
exactly one write (its initializer) and only reads besides, and the initializer
resolves through pointer casts and other such locals to a decayed string
literal, with no macro wrap on the way. The fact carries:

- `bytes`: the literal, terminator dropped (interior NULs kept).
- `readers`: the string functions (`Known::string_arguments`: `strlen`,
  `strnlen`, `strcmp`, `strncmp`, `strspn`, `strcspn`, `ato*`, `atof`) the local
  is passed to outside a macro expansion, where every other string argument of
  that call also has a view (a literal, a constant-string local, or a
  `c_string` parameter). A local with no such reader gets no fact.
- `only_read_as_string`: every read is one of those arguments.

A local whose literal has an interior NUL keeps the fact only when it is only
read as a string, since nothing else could observe the bytes after the NUL;
partners settle by fixpoint.

Lowering (`frontend/lowerer/string_views.rs`) picks the local's type from the
fact and declares it from the bytes:

- `&str` when only read as a string, the content (up to the first NUL) is
  UTF-8, and every `ato*` reader's content parses cleanly as that type.
- `&CStr` when some reader is not `ato*` and the literal has no interior NUL
  (or the local is only read as a string). Every other read lowers to
  `s.as_ptr()`.
- otherwise the raw pointer, as before.

A call to a string function whose string arguments all have views lowers by
mode: all `&str` or UTF-8 literals use `str` methods, all `&CStr` or literals
use `CStr` (`count_bytes`, `c"..."`), a mix compares bytes
(`as_bytes`/`to_bytes`). `strcmp(a, b) <op> 0` fuses to `a <op> b`; a bare
`strcmp` is `a.cmp(b) as i32`. `strncmp(a, lit, n) == 0` with a constant `n`
becomes `a.starts_with(&lit[..n])` (or `a == lit` when `n` passes the
terminator), else `a.bytes().take(n).eq(..)`. `strspn`/`strcspn` call
`slate_support::__slate_strspn`/`__slate_strcspn`, and `ato*` on a clean
literal or `&str` is `s.parse::<T>().unwrap()`.

Other libc lowering stays in `frontend/lowerer/libc_calls.rs`: `exit`/`abort`
become `std::process` calls, remaining `ato*` call
`slate_support::__slate_ato*` (core-only, ungated in `slate_support`; the
per-file prelude is gone), and `mem*` calls lower on byte buffers (below).

## Byte buffers and mem* calls

`libc_calls::buffer` resolves a `mem*` pointer argument to a buffer: a root
binding, its Rust base, element type, a constant byte offset and the known
length. Roots are a decayed local byte array (not static, not a literal), a
lifted `&[u8]`/`&mut [u8]` parameter (`borrow.many`, length unknown), or an
`ArrayAlias` local. Constant `PointerOffset`s on 8-bit elements add to the
offset.

The overlap fact is the root: `memcpy`/`memmove` with one root become
`base.copy_within(s..s+n, d)`, with two roots `d[..].copy_from_slice(&s[..])`
(two lifted references are already disjoint), and anything else stays
`ptr::copy`/`copy_nonoverlapping`. `memset` becomes `fill`; `memcmp` becomes a
slice `cmp` only on `u8` elements (C compares unsigned bytes). A constant `n`
must fit the known length; a runtime `n` is allowed, evaluated once or
duplicable, and a slice index panics where C was undefined.

`byte_buffers::ArrayAlias { array, offset }` (`BindingFacts::array_alias`) marks
a plain, written-once, non-escaping pointer local whose initializer is a
decayed local byte array plus a constant offset, when every read of it is a
`mem*` buffer argument outside a macro expansion and the array's name is not
redeclared in the function. Lowering drops its `let`, and any read lowers to
the initializer, so no raw pointer from `as_mut_ptr()` outlives a slice borrow
of the array (that would be UB under Stacked and Tree Borrows). A pointer local
with other reads (`char *p = buf; memset(p, 0, 8); return p[0];`) gets no fact
and stays raw.

Freestanding units (`slate-3amj.56`) give plain libc names no identity, so only
`__builtin_*` forms reach these lowerings; every form above needs only `core`.

## C-string arrays

`c_string_arrays::analyze` sets `BindingFacts::c_string_array` on a plain local
`char buf[N]` (unique name in the function) when it holds a terminator after
every write, in any order (no CFG). Its initializer is none (lowering zeroes
it), a string literal shorter than `N` or containing a NUL, or a zero-filled
aggregate. Every place naming it, outside macro expansions, is modeled:

- writes that terminate: `strcpy`/`strcat` destination, `fgets`, `snprintf`
  destination, and an element store of constant `0`;
- reads: `Known::string_arguments` positions, `strcpy`/`strcat` sources,
  `printf`/`fprintf`/`snprintf`/`puts`/`fputs` arguments, element reads.

Anything else (memcpy into it, a non-zero element store, `&buf`, an opaque
callee) leaves it raw. The fact also makes the array a viewed partner for
`constant_string` readers.

Lowering (`string_views.rs`) gives such an array, when its element is the
target's `c_char`, a `&CStr` view `slate_support::__slate_c_str(&buf)`, so the
string peepholes above apply (`.count_bytes()`, `== c"x"`, `<`, `.cmp()`). A
discarded `strcpy`/`strcat` into it becomes
`slate_support::__slate_strcpy`/`__slate_strcat(&mut buf, src)`, with `src`
as a `&str` or byte slice from its view; they panic where C would overflow.
Storage stays `[c_char; N]`: retyping to `[u8; N]` would cast every other use.

## Effect summaries

`effects::Effects` (`slate-8ge7.16`) answers "can this call reach object S
while it runs?" for the pointer analysis, and is the place to extend for later
questions of the same shape (`slate-8ge7.17`, purity for `slate-8ge7.9`).

- Per function, from the pointer summary walk: the mutable internal statics it
  names (`Summary.reads` and `written`), its direct callees, and `opaque`.
  `opaque` is set by an indirect call, a call to an undefined function without
  a libc identity, an undefined call passing a function pointer (a callback),
  asm with a non-empty template, and OpenMP bodies. A nested function or block
  literal only marks its captures; calling it is an unresolved call, which is
  opaque.
- `opaque` closes over callers in one reverse pass. Who reaches S is a reverse
  search from S's direct namers, run once per static that appears as a proof,
  so cost is linear per queried static.
- Consumer: a mutable `static` array (internal linkage, not volatile or atomic)
  decayed at a call with its full extent proves a pointer+length pair when S's
  address never leaks, the callee is not opaque, and the callee does not reach
  S. S leaks when a global initializer names it, an escaping or unknown use
  takes its address, an escaping or returned callee parameter receives it, or
  a local copy of its address escapes, is returned or is passed to any call. A
  local copy that stays local (the `ptr_offset` temporary of `s[0] += 1`) does
  not leak. Another argument of the same call naming S or containing a call
  also blocks the proof, because Rust evaluates it while the slice is live.

Limits: a static whose address is stored anywhere (a `static int *tail =
&values[2]` initializer) never proves, even when the callee cannot load that
pointer; that needs points-to for the holders.

## Def-use

`def_use::DefUse::of(body)` (`slate-8ge7.5`) is one function's def-use, built
in one walk over the IR. It is the base for local facts that need "who reads or
writes this binding, and where", in place of each analysis keeping its own
walker. No SSA: one C variable keeps one Rust binding, and the `slate-8ge7.1`
benchmark showed no analysis that SSA would make asymptotically cheaper.

- Positions number statements in pre-order, so a statement's subtree is a
  contiguous range. Each use records its binding, its position, and its access:
  `Read`, `ReadProjection`, `Initialize`, `Write` (assignment or update),
  `Argument` (direct address or fixed-array argument), `Escape` (other address
  uses, place operand, asm operand, OpenMP clause, nested-function or
  `__block` capture, `cleanup`), or `Capture` (a VLA extent named by a type, or
  a by-value block capture: a read with no value site, so never a
  `single_read`). DefUse stops at inner functions; it sees their captures at
  the literal, not their bodies. An update (`x += 1`,
  compare-exchange) is both a read and a write. `single_read(b)` is true when
  `b`'s only use is one `Read`.
- Each loop and switch records its body's range, so "inside this loop" is a
  range test. Labels, asm, SEH `__try` and OpenMP regions, and every `case` with
  its switch's position, are recorded to reject unstructured regions.
- Position order is execution order only within a range with no label, case,
  or jump in it. There is no function-wide order or CFG (`slate-8ge7.22`): an
  ordering query proves its range is straight-line (`structured`, or a sibling
  scan that rejects labels, cases and jumps), so gotos elsewhere in the
  function cannot enter or leave it.

### Binding mutability

- `BindingFacts::mutability` records writes after initialization, opaque
  place uses, and direct address or array arguments that may become borrows.
  Lowering resolves those arguments against the selected parameter
  representation. A shared borrow can use an immutable binding, and a write
  through a pointer can leave the pointer binding immutable.
- Mutability collection uses `for_each_function_ref` to build one `DefUse`
  per scope. By-value block captures get facts scoped to their invoke function.
  A by-reference capture loads an immutable pointer binding to writable storage.
  Aligned array field reads are projections. Packed or underaligned fields
  retain the conservative address-use classification.
- Lowering marks Rust-created writes where it emits them, including aggregate
  fills, dispatch slots, and OpenMP firstprivate copies. Kept macro places
  retain conservative mutability.
- `decl-init` also folds an adjacent uninitialized scalar declaration and
  two branches ending in a single write into a conditional initializer.
  The condition and the branch prefixes must leave the binding untouched.
  Branch prefixes stay inside their arm's statement expression.

### Counted loops

`counted_loops::analyze` is the first consumer. It produces
`UnitFacts::counted_loop(for_id) -> CountedLoop { index, start, end, inclusive }`
for a `for` whose index provably runs over `start..end`. The conditions are in
[ir/control-flow](ir/control-flow.md#counted-loops). It used to be a sema pass
that wrote `counted:` into the IR; the IR no longer carries it.

Lowering reads `start` and `end` from the fact, not from the `for`. They are
clones of IR values, so a macro body keeps its `macro_arg` holes; the
`SLATE_CHECK_MACRO_TEMPLATES` self-check strips them before lowering.

### Inline temporaries

The `inline-temps` transform (`transform/inline_temporaries.rs`,
`slate-8ge7.9.1`, a transform since `slate-8ge7.24.1`) deletes an IR
`Statement::Temporary` (printed `let %n: T [synthetic]`, lowered as `__v<n>`)
and moves its initializer, same `NodeId`, into its one read. It runs in both
slots: before macro_keep, so macro_keep surveys IR without those temporaries,
and after the strip, for temporaries inside expansions macro_keep dropped. A
temporary qualifies when all of these hold:

- the temporary has an initializer, is not
  `unsequenced`, has the initializer's type, has exactly one `Read`, and no
  record field type captures it;
- the read is in one of the next 32 sibling statements (or the list's tail
  value), in that sibling's own values (not a nested body), and not inside a
  macro argument, statement expression, or address-of;
- no sibling in between is control flow or opaque (labels, cases,
  break/continue/goto, asm, SEH, OpenMP, fences), and the producer's effect
  summary does not conflict with what runs between it and the read, or with
  what runs before the read inside the consuming statement. A call, intrinsic
  or compare-exchange that contains the read counts as running after it.
- an effectful producer is never moved under a `&&`/`||` rhs or a `?:` arm.

Each statement list is processed in order and each move happens before the
next temporary is decided, so a chain (`t1 = a; t2 = t1 + b; use(t2)`)
collapses in one pass and `t2`'s producer summary already includes `a`. The
lowerer sees no inlinable temporary, so the goto state machine needs no special
case. The lowering-side gates (`element_view`, `borrow_of`, `c_string_of`)
were dropped: across 3067 fixtures none ever blocked a candidate. Temporaries
the lowerer itself makes (`__t<n>`) are not covered (`slate-8ge7.24.2`); the
engine's `inline_temps::late` still folds those.

## Shadow mode

A port runs next to the Rust-side pass it replaces until every difference is
triaged as an improvement (with a fixture) or a bug (`slate-8ge7.4`). Only then
does lowering read the fact, and the Rust-side pass is deleted.

The harness (`SLATE_SHADOW`, `backend::shadow_diff`, `Program.shadow`,
`tools/shadow_report.py`) was deleted with `pointer_lattice`, its only user,
in `slate-8ge7.7`. It compared decisions per binding keyed by `(unit, Rust fn
name, binding name)` and printed one `slate-shadow` line per difference. The
next port restores it from the commit before that deletion.

## Related

- [pointer-capability-lattice](pointer-capability-lattice.md): the lattice the
  first port reimplements on the IR.
- [ir/pipeline](ir/pipeline.md#node-identity-and-metadata): how `NodeId`s are
  allocated and preserved.
- [entity-model](entity-model.md): `BindingId`s and redeclaration merging.
