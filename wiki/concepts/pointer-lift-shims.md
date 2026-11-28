# Pointer lift shims for exported and address-taken functions

<!-- toc -->
- [Shape](#shape)
- [What a shim lifts](#what-a-shim-lifts)
- [Soundness rules for outside
  callers](#soundness-rules-for-outside-callers)
- [Routing](#routing)
- [Which functions are shimmable](#which-functions-are-shimmable)
- [Turning shims off](#turning-shims-off)
- [Limits](#limits)
- [Related](#related)
<!-- /toc -->

Design for `slate-8ge7.26.7`, decided 2026-11-27 and 2026-11-28. Nothing here
is implemented yet; the child beads `.2` to `.7` build it in order.

The [pointer capability lattice](pointer-capability-lattice.md) only retypes
candidates: functions whose every caller is visible. An exported function (open
mode, `--export-dynamic`) or an address-taken one keeps raw pointers because
outside code calls it through the C symbol or a function pointer. A shim splits
such a function in two: a lifted Rust function that internal direct calls use,
and an `extern "C-unwind"` wrapper with the original C signature that owns the
symbol and every function-pointer use. External callers see the same symbol,
signature and address as before.

`slate-8ge7.26.6` measured the upper bound (`wiki/log/2026-11-27-18-54.md`):
closed redis 575 -> about 1160 lifted params, open redis 116 -> about 1865,
open lua 41 -> about 170. That count used rules that are unsound for outside
callers; `.2` re-measures under the rules below, and its counts decide whether
the epic continues.

## Shape

For a shimmed `int f(struct s *p, const struct t *q, const char *name)`:

```rust
pub fn f(p: Option<&mut s>, q: Option<&t>, name: *const c_char) -> i32 { /* lifted body */ }

#[unsafe(export_name = "f")]
pub extern "C-unwind" fn __slate_c_f(p: *mut s, q: *const t, name: *const c_char) -> i32 {
    f(unsafe { p.as_mut() }, unsafe { q.as_ref() }, name)
}
```

- The lifted function keeps the Rust name `f`, uses the Rust ABI and carries no
  export attribute. It is `pub` when another unit calls it by Rust path.
- The wrapper is `__slate_c_f`. It carries the C symbol and every attribute the
  plain export carries today: `export_name` (including odr, asm-label and
  long-double entry names), weak linkage, `link_section`, `used`/retain,
  dll storage, constructor/destructor registration. An address-taken function
  with internal linkage gets a wrapper with no `export_name`.
- The wrapper is a safe `extern "C-unwind" fn` with `unsafe` blocks, the same
  convention address-taken functions use today, so existing
  `Option<unsafe extern "C-unwind" fn(..)>` types still accept it.
- A function that is shimmable but lifts no param and no return is emitted
  exactly as today, with no wrapper.

## What a shim lifts

A shimmed function lifts only single-object borrows: `Option<&T>` and
`Option<&mut T>`, which the wrapper builds with `p.as_ref()` / `p.as_mut()`.
Every other param passes through unchanged.

No slices and no `&CStr`. Both claim more memory than C needs:
`from_raw_parts(p, n)` claims `n` elements and `CStr::from_ptr` scans to a NUL,
while C needs only what the body touches on the path it takes. Here an outside
call `find_zero(a, 1000)` with `int a[2] = {1, 0}` is fine in C and undefined
behavior behind a slice wrapper:

```c
size_t find_zero(const int *buf, size_t n) {
    for (size_t i = 0; i < n; i++)
        if (buf[i] == 0)
            return i;
    return n;
}
```

Making these sound needs a proof that every path touches all of `[0, n)` or
scans to the NUL: no early `return`/`break`/`goto`, no conditional access, and
no call that may `longjmp` out of the loop. That is a later bead. A param that
would otherwise be `many` or `c_str` reports `shim.no_slice` or `shim.no_c_str`
and stays raw. The `.26.6` counts put the loss at about 7% of redis open
(`c_str` 90 + slice 22 of 1634) and 10% of redis closed (47 + 4 of 491).

A lifted return converts back to raw: `&T` with `ptr::from_ref`, `&mut T`
  with `ptr::from_mut`, `Option<..>` with `map_or(ptr::null(), ..)`. Return
  lifting keeps its existing conditions (one tracked pointer param, lifetime
  elision only).

## Soundness rules for outside callers

An outside caller is any C code we did not translate, or any call through a
function pointer. The analysis sees none of its arguments, so a shimmed
function is a candidate marked `external` (`Summaries::assume_external_callers`)
and gets these rules:

- **Nullability: every lifted pointer param is `Option`.** `NULLABLE` flows
  only caller -> callee in `flow::solve`, so in `f(p) { g(p); }` where `g`
  null-checks, `f` gets no null source and would lift to `&T`, which an outside
  `f(NULL)` breaks. A must-deref analysis that proves non-null is a later
  bead.
- **Whole-object contract.** A non-null pointer from an outside caller is
  assumed to point at a live, aligned, whole `T`: Rust treats `&T` as
  dereferenceable for `size_of::<T>()` from entry, even on paths that never
  touch it. A dangling pointer the body ignores (`f(p, 0)` with
  `if (flag) use(*p)`) or a smaller object cast to a larger struct type whose
  prefix the body reads is fine in C and undefined behavior after the shim.
  This is the contract every hand-written `ptr::as_ref` FFI binding makes, and
  real callers rarely break it, so shims lift by default and a function that
  does break it is opted out by name (see
  [Turning shims off](#turning-shims-off)).
- **Aliasing.** An outside caller may pass one object in two params, or pass a
  global the body also names. Two pointee types are compatible when they are
  the same type or either is `void` or a character type.
  - `&mut` needs: no other pointer param of a compatible pointee, and a body
    that (transitively, through `crate::effects`) touches no mutable global of
    a compatible type.
  - `&T` needs: no other compatible pointer param that is written, and a body
    that writes no compatible mutable global.
  - A failure reports `shim.aliased_param` or `shim.aliased_global` and keeps
    the param raw.
- **Caller-side rules still apply to internal callers.** `external` does not
  skip `borrow::unproven` or `Flow::propagate_unproven`: an internal call that
  passes an unproven argument (`unproven.argument`, `raw_argument`,
  `macro_call_site`, `part_offset`) still blocks the lift. External only
  changes candidacy and the slice and `c_str` refusal above.
- The wrapper sits on every indirect call, including calls from translated
  code through a function pointer, so these rules cover those callers too.

## Routing

- Every internal direct call names the lifted `f`. Same unit: plain call.
  Other units: link binds the call by Rust path
  (`use crate::__slate_unit_<stem>::f`), as for closed-mode lifts today, and
  `copy_to_declarations` copies the lifted facts only to declarations that link
  binds that way.
- Every address-of use names the wrapper: `Some(__slate_c_f)` in initializers,
  `__slate_c_f as *const ()`, function-pointer locals and arguments, and extern
  declarations another unit uses only for its address (raw C signature). There
  is one address per function, so function-pointer equality holds across units
  (`tests/fixtures.release/cross_unit_address_taken_exports` checks
  `local_add == api.add`).
- Outside callers reach the wrapper through the symbol.
- There is no per-call-site routing: if one internal call fails a caller-side
  rule, the param stays raw for everyone. Per-call-site routing (that caller
  calls the wrapper instead) is a later bead, filed only if `.2` shows a large
  loss.

## Which functions are shimmable

The exclusions are checked on the function's own attributes, not on
`Program::export_pin`. In open mode `Program::new` returns
`ExportPin::Exported` for every export and never runs `fixed_abi` or
`pinned_symbols`, so an open-mode export with a `section` or a `target`
attribute looks plain; checking attributes gives both modes the same answer.

Shimmable: exported (open mode), address-taken, and these closed-mode pins,
whose attribute moves to the wrapper: `section`, `used`, `retain`,
`dll_storage`, `selectany`, `constructor`, `destructor`, `weak_odr`,
`discardable`. `selectany`, `weak_odr` and `discardable` may have one
definition per unit; `--merge` and link must keep one wrapper per symbol
(`.5`, `.6`).

Excluded in v1:

| Excluded                    | Why                                                                           |
| --------------------------- | ----------------------------------------------------------------------------- |
| `main`                      | the C runtime calls it with a fixed signature; nothing internal calls it      |
| `variadic`                  | the wrapper cannot forward `...` to a Rust-ABI function                       |
| `naked`                     | the body is asm; there are no Rust params to lift                             |
| `interrupt`                 | fixed interrupt ABI and frame argument                                        |
| `abi_pass`                  | nontrivial pass (byval, sret, indirect) lowering lives in the export shape; the wrapper would have to redo it |
| `target_feature`, `sme`     | the lifted fn needs the same features or streaming mode, and the wrapper -> lifted call crosses that boundary |
| `alias`, `weakref`, `ifunc`, `fallback` | another symbol or a resolver names this function's address        |
| `global_asm`                | asm text references the symbol and may assume its C body                      |
| `defined_twice`             | two definitions of one symbol: which body internal calls bind is ambiguous    |
| `declared_other_prototype`  | a unit declares it with another arity or pointer-ness, so link cannot bind that unit's calls by Rust path |
| named in a macro template   | `interproc::functions_pinned_by_templates`: the template's call text is not rewritten, so expansions pass raw args |
| unit has `null_pointer_is_valid` | built with `-fno-delete-null-pointer-checks` (the kernel): code written for a compiler that infers nothing from pointer use is the worst fit for the whole-object contract |

`Summaries::assume_external_callers` (the `.26.6` measurement) excludes only
main, variadic, naked and interrupt, so its open-mode counts include every
other row; `.2` narrows it to this table.

## Turning shims off

- `--lift-shims` / `--no-lift-shims` on `translate` and `translate-project`
  turns the whole feature on or off. It is off by default until the corpus
  gate (`.7`), then on.
- `--no-lift-shim <name>`, repeatable, keeps the named C functions unshimmed:
  raw signature, no wrapper, output as without shims. It matches every
  function with that C name, static ones in any unit included. Use it when an
  outside caller breaks the whole-object contract, or when a shim breaks a
  build.

## Limits

- A panic in the lifted body (an `unwrap` on `None` where C would fault)
  unwinds into C frames; `C-unwind` makes that defined, but a C caller without
  unwind tables aborts.

## Related

- [pointer-capability-lattice](pointer-capability-lattice.md) and
  [slate-analysis](slate-analysis.md#pointer-capabilities): candidacy, the
  lattice and the existing rules.
- `wiki/log/2026-11-27-18-54.md`: the `slate-8ge7.26.6` upper-bound
  measurement and the `lift-shim` lines of `SLATE_LIFT_REASONS`.
