# Control flow and side effects

<!-- toc -->
- [Function bodies](#function-bodies)
- [Structured control flow](#structured-control-flow)
  - [Structured exception handling](#structured-exception-handling)
  - [OpenMP regions](#openmp-regions)
  - [Statement attributes](#statement-attributes)
  - [Materialized function ends](#materialized-function-ends)
- [Side-effect hoisting](#side-effect-hoisting)
  - [Statement expressions](#statement-expressions)
- [Unsequenced effects follow clang](#unsequenced-effects-follow-clang)
- [Resolved away before the IR](#resolved-away-before-the-ir)
- [Pragmas](#pragmas)
<!-- /toc -->

Part of the [IR spec](../ir-spec.md). Statement forms are in the
[grammar](../ir-grammar.md#statements).

## Function bodies

- A body is a list of statements holding typed expression trees. A list
  (not a scope) so a multi-declarator declaration needs no invented C scope.
- Expressions are side-effect-free except calls. Every node has a `NodeId`
  and a concrete type.
- Locals and globals are referenced by `BindingId`, fields by index. Names
  like `c` and `c#1` are printer spellings, not identity.

## Structured control flow

Control flow keeps its source form: `if`, `while`, `do`/`while`, `for`,
`switch`, labels, `goto`, computed goto, and blocks lower as themselves, not
as `loop` + `break`. The source shape tells the Rust rewriter what the ideal
form was.

- Controlled bodies keep explicit `Block`s; `Null` differs from an empty
  block. `for` keeps its initializer list, optional condition (omitted means
  unconditional), and optional increment.
- Conditions become explicit `bool` values.
- Loops and switches have ids. `break` names the nearest loop or switch;
  `continue` the nearest loop, through a switch. C2y `break label;` and
  `continue label;` name the labeled statement's id, which may be any
  enclosing loop or switch. Rust lowering consumes these ids and never
  re-searches parents or label names.
- Continuations: `for` → increment, then condition; `while` → condition;
  `do`/`while` → trailing condition.
- Case labels keep their switch id even nested in loops (Duff's device)
  and stay nested statements, not flattened arms. Statement order and
  absent `break`s encode fallthrough. Case endpoints (and GNU ranges) fold
  to the promoted switch type.
- Labels, `goto`, and `label_addr<ptr<void>>` use label `BindingId`s,
  distinct for GNU local labels. Computed goto keeps its pointer operand.
- Fixtures: `ir_control_*.c`.

### Structured exception handling

- `Statement::SehTry { id, body, handler }` keeps MS `__try` as written;
  `SehHandler` is `Except { filter, body }` or `Finally(body)`.
  `SehLeave(id)` names the innermost `__try` whose body encloses it.
- The filter is an `evaluation` like a loop condition: its effects stay
  attached (`__except (r = f())`), since it runs once per exception.
  Its value is integer-typed (enums lowered, comparisons `bool`);
  consumers narrow it to 32 bits as clang does (`long` on Windows).
- `_exception_code`, `_exception_info`, `_abnormal_termination` and their
  `__` spellings lower to `ValueKind::Seh(SehIntrinsic)`, effect-free reads
  of the active exception (`u32`, `ptr<void>`, `i32`), not calls.
- Checker (`sema/assertion.rs`, `sema/seh_scopes.rs`): `goto` into a
  `__try` body, `__except` block or `__finally` block is rejected with
  clang's "cannot jump from this goto statement to its label" (each of the
  three is its own protected scope; `seh_scopes::label_scopes` records each
  label's scope path per function). Leaving the innermost `__finally` by
  `return`, `break`/`continue` (named or not), `__leave` or `goto` warns
  `-Wjump-seh-finally`.
- Slate has no Rust lowering; a function with SEH is a lowering barrier.
- Fixtures: `clang/windows/x86_64/ir_seh.c`,
  `error/clang/windows/x86_64/seh_misuse.c`, `suites/clang-test/*/windows/*/CodeGen__exceptions-seh*.c`.

### OpenMP regions

- `Statement::OmpParallel(OmpParallel)` is `#pragma omp parallel`.
  `num_threads` is converted to `unsigned int` (libgomp's parameter) and
  `condition` (the `if` clause) to a truth value; both are hoisted before
  the region, so they are evaluated once by the encountering thread.
- `private` lists one `OmpPrivate { original, copy, firstprivate }` per
  `private`/`firstprivate` variable. `copy` is a fresh automatic
  `Variable` with the original's type and no initializer; the body refers
  to `copy` only (name resolution declares it in a scope around the body).
  A `firstprivate` copy starts as the original's value at region entry.
  Globals can be privatized too.
- `shared` lists the automatic variables and parameters declared outside
  the region that the body references, explicit `shared` or implicit,
  in first-reference order. Globals and static locals are shared but not
  listed: they need no capture.
- Checker (`sema/assertion.rs`): clause variables must be variables, at
  most once across data-sharing clauses; a `private` variable cannot be
  const; privatized variables must be complete and not variably modified
  (VLA copies are unsupported). `num_threads` must be an integer, `if`
  scalar. `default(none)` requires every outer variable the body
  references, globals and static locals included, to be listed.
  `return`, `break`/`continue` (named or not) and `goto` that leave or
  enter a region are "invalid branch to/from OpenMP structured block"
  (gcc's wording; region scopes join the SEH scope paths for `goto`).
- Slate has no Rust lowering yet (slate-3amj.24.3); the statement is a
  lowering barrier.
- Fixtures: `{gcc,clang}/linux/x86_64/openmp_parallel.c`,
  `msvc/windows/x86_64/openmp_parallel.c`, `error/*/*/x86_64/openmp-*.c`.

### Statement attributes

- `[[fallthrough]];` is a null statement with source metadata; an
  attribute statement without it is a null statement. Outside a switch it
  is an error except under gcc, which accepts it (a pedwarn) and gets no
  metadata.
- `[[x]] stmt` lowers to `stmt`, dropping the attributes (clang and gcc only
  warn). `fallthrough` on a non-empty statement is an error except under
  gcc, which ignores it.
- A nested function definition is an error under clang and MSVC and
  `Unimplemented` under gcc (`slate-parser-dyd.56`).

### Materialized function ends

| Case                                          | IR                                                                         |
| --------------------------------------------- | -------------------------------------------------------------------------- |
| C99+ `main` falling off the end               | `fallthrough=ret_zero`                                                     |
| `void` function falling off the end           | `fallthrough=ret_void`                                                     |
| non-void function reaching the end            | `fallthrough=ub_if_used`                                                   |
| `noreturn` or naked function reaching the end | `fallthrough=ub`                                                           |
| valueless `return` in a non-void function     | `return`, same meaning; accepted only under gcc C89/gnu89 and MSVC (C4033) |

## Side-effect hoisting

Rust has no compact form for C's embedded effects, so a final sema pass
(`src/sema/effects.rs`) pulls them into statements:

- `a = b`, compound assignment, and `++`/`--` become `Statement::Write` on
  a place. As a statement only the write remains. Where the result is used,
  a non-constant stored value is bound to a `[synthetic]` temporary first,
  so `x = y = f()` calls `f` once and `b = (i = i + 1)` does not recompute
  `i + 1` after the write; `x = y = 0` chains the constant without re-reading
  `y`. A store to a bit-field yields the value cut to the field's width
  (C11 6.5.16p3): `widen<u32>(truncate<u8b>(v))` for `unsigned bf : 8`,
  also for prefix `++`/`--` and compound assignment.
- Postfix forms snapshot the old value in a `[synthetic]` temporary; the
  write targets the once-evaluated place. `f(i++)` always snapshots;
  removing unneeded temporaries is the analysis pass's job.
- Calls stay values. Arguments evaluate left to right.
- `&&`, `||`, `?:` with effectful operands become `Statement::If` writing a
  per-branch result temporary (`void` operands discard). A direct call to a
  `[memory=none]`/`[memory=read]` function is not an effect, so
  `a && square(b)` stays `logical_and` (`ir_const_pure_calls.c`).
- Comma lowers left to right; the last operand is the value.
- Effects in a loop condition or `for` increment stay attached as an
  `evaluation` block (statements, then the value), rerun per iteration:
  `while { ch = getc(f); ch != EOF } {}` is valid Rust. The increment yields
  `void`; a `do`/`while` condition runs after the body.
- Synthetic temporaries print `let %id: ty [synthetic] = ...` and never
  enter name resolution.
- Fixtures: `ir_side_effects_hoisted.c`, `ir_effects_sequencing.c`.

### Statement expressions

`({ stmts; e; })` lowers to `ValueKind::StatementExpression(Evaluation)`,
always effectful and always removed by hoisting:

- value used: a synthetic `let %t: T;` then a block ending in a write to
  `%t`, and the expression reads `%t`;
- value discarded or `void`: the block alone.

The block keeps its scope; `return`, `break`, and `goto` inside it act on
the enclosing function and loops, as in gcc. Under `&&`/`||`/`?:` it goes
through the same `if` lowering (`ir_statement_expressions.c`).

## Unsequenced effects follow clang

Where C leaves two effects on one object unsequenced (C11 6.5p2) there is
no order to reproduce and the oracles disagree. With `i = 1`, `f(i++, i++)`
where `f(a, b) = a * 10 + b`:

| Compiler   | Result | Arguments |
| ---------- | ------ | --------- |
| clang 22   | 12     | `f(1, 2)` |
| gcc 16     | 21     | `f(2, 1)` |
| MSVC 19.51 | 11     | `f(1, 1)` |
| slate      | 12     | `f(1, 2)` |

- slate hoists left to right, like clang and like Rust's argument order.
  Emulating each flavor would pin codegen accidents, not semantics; the
  flavor mechanism is for defined meanings that differ.
- Statements committing to the arbitrary order are marked, as other UB is:
  `let %18: i32 [synthetic, unsequenced] = ...`,
  `write<i32, unsequenced>(..)`.
- `sema/sequencing.rs` decides it. A location is a binding (or the object a
  pointer binding points at) plus the path of members and constant
  subscripts, so `x.a++`/`x.b++` and `b[0]`/`b[1]` don't conflict; computed
  subscripts compare equal only to each other. A conflict is one location a
  prefix of the other with at least one write. Groups: a call's callee and
  arguments, both operands of an arithmetic or comparison operator, and an
  assignment's target against a second effect on it in the value
  (`i = i + 1` is defined by 6.5.16p3).
- gcc `-Wsequence-point` agrees on all 1963 corpus configurations; clang's
  `-Wunsequenced` tracks scalars only, so gcc is the detection oracle and
  clang the order oracle (`ir_unsequenced.c`).

## Resolved away before the IR

- `_Generic`: the controlling operand is typed (unevaluated: temporary
  bindings and string globals roll back) after lvalue conversion but not
  promotion; only the selected association is lowered. A selection is a
  place when the selected expression is (`_Generic(..) = v`,
  `&_Generic(..)`) (`ir_generic_selection.c`).
- `__builtin_choose_expr(c, a, b)`: `c` must be an integer constant; the
  selected operand replaces the call and supplies the type. The other is
  name-resolved and type-checked but not evaluated.

## Pragmas

Pragmas are never IR nodes. `sema::pragmas` resolves them in one ordered walk
(including `StmtKind::Pragma` in bodies) into lookups keyed by `TagId`
(packs) or symbol name, because tag bodies are laid out on demand rather than
in source order. Owners consume them: `pack` and `ms_struct` → record layout
([types](types.md#pragma-pack-and-ms_struct)); `weak`, `visibility`,
`redefine_extname` → `SymbolAttributes`; floating pragmas →
[operations](operations.md#floating-pragmas). An explicit attribute always
wins, so `apply` fills only fields the declaration left open.
