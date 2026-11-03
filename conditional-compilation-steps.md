# Conditional compilation steps

Next priority: move macro variant dispatch into definitions so function bodies
keep one invocation per C call site. Start with the attempt below; Beads owns
task status.

## Completed foundation

Macro epic `slate-f2zu.9.5.12`:

- `9.5.7` and `9.5.10`: lift writes through arguments and nested macros.
- `.21`: write `merge-report.txt` on every `--merge` run.
- `.18`: drop hole types from the template key.
- `9.5.6`: pass caller locals as hidden arguments, then regenerate zlib.
- `.10`: allow `return`, `break` and `continue` in templates.
- `.20`: keep `sizeof` unfolded in templates.
- `.24`: lower `alignof` to `align_of::<T>()` where usable.
- `.25`: lower `offsetof` to `offset_of!` where usable.
- `.11` and `.13`: keep integer casts and conversions in sema.
- `.19`: initial audit left caller-directed `goto` expansions inline; see `9.15`
  below for the remaining label-passing work.
- `.14`: audit zlib, with all-features `validate_merge` and both full profiles.

Deferred: `.17` (remeasure with caller-local arguments lifted), `.16`.

## Maintainable output

Goal: any rewrite slate can prove keeps behavior and that a person maintaining
the port would make anyway. Readable call sites beat line count, so lifting a
macro is worth it even when the crate grows. Fixes stay generic, with no zlib
knowledge. Beads are `slate-f2zu.9.*` unless they carry another prefix.

Keep the checks passing throughout: `--check-projection`, the merge fixtures and
`tools/corpus/validate_merge.py`. Regenerate `slate-zlib` and review it after
each group.

### Next attempt: move macro dispatch into definitions

Prioritize this before the remaining naming and control-flow work. In zlib's
`deflate_huff`, most of the extra Rust is repeated cfg dispatch around macro
calls. Keep one invocation at each C call site and move feature and target
selection into cfg-selected definitions of the same Rust macro name. This
removes repeated branches and gives the largest immediate readability gain.

Target output inside `deflate_huff`:

```rust
Tracevv!(stream, format, value);
_tr_tally_lit!(s, value, bflush);
FLUSH_BLOCK!(s, 0i32);
```

The arguments above are schematic. Preserve the translated arguments and C
semantics; do not pre-evaluate arguments to make call sites match.

Attempt in this order:

1. `9.5.15`: give compatible invocations of one originating C macro a stable
   family identity before function merging. Keep body-specific template
   identities for correctness checks. Select different nonempty bodies with
   mutually exclusive cfg definitions under one invocation name. Start with
   `_tr_tally_lit` and `FLUSH_BLOCK`.
2. `9.5.15`, then `9.5.12.28`: normalize nested calls, starting with
   `FLUSH_BLOCK_ONLY` inside `FLUSH_BLOCK`. Deduplicate identical final
   definitions and union their cfg predicates. Do this before assigning display
   names; renaming `__2` suffixes after function merging cannot remove call-site
   branches.
3. `9.5.4`: retain empty invocation sites so the merger can align them with
   nonempty sites. Reuse argument spelling from a nonempty configuration and
   emit an empty token-consuming definition in disabled configurations.
   `Tracevv!` must neither evaluate arguments nor resolve their names when
   disabled.
4. Make projection comparison expand the selected macro definitions, or apply
   the same normalization to both sides. Add C differential fixtures for
   argument side effects, writable arguments, nested calls and caller-returning
   macros. `FLUSH_BLOCK!` must still return from its caller.
5. `9.5.12.29`: add verbose trace comparison and feature coverage to
   `validate_merge` so validation exercises enabled trace bodies. Run this
   before accepting the dispatch changes; trace calls at `z_verbose = 0` do not
   exercise those bodies.
6. Regenerate and review `slate-zlib`. Require one unconditional `Tracevv!`, one
   `_tr_tally_lit!` and three `FLUSH_BLOCK!` invocations in `deflate_huff`, with
   macro variant dispatch confined to definitions. Keep projection, merge
   fixtures and runtime parity passing across the covered features and targets.

Only share a name when invocation interfaces are compatible and cfg selection is
unambiguous. Preserve separate families or matcher arms for variants that
coexist in one configuration. Keep hidden arguments compatible, and keep
prerequisite computations inside the selected expansion. Stay inline where
equivalence cannot be proved.

`9.13` and `9.14` remain the follow-up for shared types, path-bound project
calls and placing header macros in `__slate_macros`. They remove duplicate
definitions across units, but do not block moving dispatch out of
`deflate_huff`.

### Remaining work

Resume these groups after the macro dispatch attempt. Beads own task status.

Recover names, with no control flow change:

- `9.7`: enumerators as consts (`BAD`, not `16209`).
- `9.8`: object-like constant macros as consts (`Z_STREAM_ERROR`, `GZ_READ`).
- `9.9`: anonymous records take their typedef or field name (`code`,
  `gz_state`).
- `9.10`: keep the written literal radix (`0xff`).
- `9.12`: `char[]` from a string prints as a string.
- `9.11`: C `const` objects become non-mut statics.

Stop spelling differences from splitting code:

- `9.5.12.27` and `slate-nfq8.14`: spell `size_t` and `ptrdiff_t`. Reassess the
  remaining target splits in `longest_match`.
- `9.19`: fold compound-assignment temporaries, removing `#[cfg] let` pairs.
- `9.5.5`, `9.5.8.7`, `9.5.8.8`, `9.5.8.9`: libc spellings. Then reassess `9.3`
  and close it if nothing is left.

Give each definition one home:

- `9.5.12.30`: attach C macro definition comments to emitted Rust macros,
  preserving ownership through cfg variants and header relocation. zlib's
  `CLEAR_HASH` comment survives above `slide_hash` instead of its macro;
  `UPDATE_HASH` and `INSERT_STRING` comments drift there too. Keep definition
  documentation separate from invocation-site comments.
- `9.13`: one module per C header for types, and path-bound calls in library
  output.
- `9.14`: header macros in `__slate_macros`, one template per C macro.

Recover control flow:

- `9.17`: `do {} while (0)` becomes a block, and constant conditions fold.
- `9.18` and `slate-o3gw.24`: `else if`, and no `else` after a diverging branch.
- `9.16`: switch dispatch in functions with `goto` becomes a `match`.
- `slate-nfq8.4`: cfg on match arms, and hoisting differing `if` conditions.
- `9.5.12.22`: lift macros that declare locals in those functions (`DROPBITS`,
  `BYTEBITS`).
- `9.5.12.23`, then `9.15`: pass labels so `NEEDBITS`, `PULLBYTE`, `PULL` and
  `ROOM` lift. Reverses the `.19` decision.
- `slate-o3gw.10` and `slate-o3gw.25`: structure the remaining `__join` blocks.

Clean up what remains:

- `slate-o3gw.21` and `slate-o3gw.18`: redundant `unsafe`.
- `slate-o3gw.20`: sink zeroed locals, with `null_mut()` for pointers that stay.
- `slate-o3gw.22` and `slate-o3gw.23`: post-increment temporaries and redundant
  casts.
- `9.5.13`: re-measure cfg groups and trace why splice leaves the mostly shared
  ones.

Only if a project needs it: `9.5.12.12` (variadic and `#`/`##` macros).

Not on this path: `9.5.8.5`, `9.5.8.6` and `9.5.12.26` are minor or do not show
up in zlib.
