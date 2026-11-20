# slate-analysis

<!-- toc -->
- [Boundary](#boundary)
- [Keys](#keys)
- [Call resolution](#call-resolution)
- [Scaling rule](#scaling-rule)
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
  idiomization needs (`counted_loops`, possibly others) may move here
  (`slate-8ge7.8`).

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

## Shadow mode

A port runs next to the Rust-side pass it replaces until every difference is
triaged as an improvement (with a fixture) or a bug (`slate-8ge7.4`). Only then
does lowering read the fact, and the Rust-side pass is deleted.

- `SLATE_SHADOW=<pass>[,<pass>]` (or `all`) turns it on; unset, nothing runs.
- Decisions are compared per binding, keyed by `(unit, Rust fn name, binding
  name)`. Interproc passes run on lowering's output before any per-function
  rule renames a binding, so lowering's names identify the same C entity on
  both sides.
- IR side: lowering pushes `rust_ast::ShadowDecision { pass, function, binding,
  decision }` into `Program.shadow`, rendering each fact with its own name
  tables (`Tables.names`, `Tables.bindings`).
- Rust side: the pass calls `backend::shadow_diff::compare` with its decisions,
  rendered in the same vocabulary. `pointer_lattice` reports `solve`'s
  `Representation` (`Slice`, `RefMut`, `RawConst?` with `?` for nullable), the
  analysis decision before `length_lattice` and plan validation.
- Output is one `slate-shadow pass=.. kind=differ|rust_only|ir_only ..` line per
  difference and a `summary` line. `tools/shadow_report.py` sweeps fixture
  directories or a `--project` compile database and groups differences by kind
  and decision with example sites.

Until a pass has an IR side, every Rust decision is `rust_only`.

## Related

- [pointer-capability-lattice](pointer-capability-lattice.md): the lattice the
  first port reimplements on the IR.
- [ir/pipeline](ir/pipeline.md#node-identity-and-metadata): how `NodeId`s are
  allocated and preserved.
- [entity-model](entity-model.md): `BindingId`s and redeclaration merging.
