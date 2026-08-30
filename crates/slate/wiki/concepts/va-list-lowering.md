# va_list lowering

Two representations, chosen per translation unit. `module_requires_native_va_list`
(`src/frontend/lowerer/analysis.rs`) decides **once per file**, not per
function: every va_list use in the file can be the boxed `__SlateVaArgs`
(`Rc<Vec<__SlateVaArg>>` + cursor index, `Clone` = independent cursor over
shared storage) unless something in the file needs genuine C ABI — a
pub-exported/`c_abi_functions` function touching va_list, or a call to a
function _not defined in this module_ with a va_list argument (real
`vprintf`/`vfprintf`). Native mode falls back to real `core::ffi::VaList`.
No fixture in the corpus mixes both needs in one file, so the whole-file
simplification is unverified against that case.

## Type and place recognition

`va_list_shaped_type`/`c_va_list_shaped_type` (`types.rs`, `lowerer.rs`)
recognize the bare record, `ptr<record>` (decayed by-value `va_list`),
`array<record,N>` (storage, including real C arrays of `va_list`), and
`ptr<array<record,N>>` (a genuine `va_list*`) uniformly, recursing through
`cir.ptr`/`cir.array` layers.

`va_target_place` (`memory.rs`) plus propagation through `cir.cast`/
`cir.load` is the place-side counterpart: va_start/va_arg/va_copy operands
are always the result of an intervening cast (array decay) or load
(parameter shadow-copy), so the underlying storage place has to be threaded
through those ops. `va_places: BTreeMap<String, Expr>` on `FunctionLowerer`
records this propagation.

`__SlateVaArgs.args` is `Option<Rc<Vec<__SlateVaArg>>>` with a `const fn
empty()`, not `Rc::new(vec![])` directly — `Rc::new` isn't `const`, and
va_list-shaped globals/uninitialized locals need a const-evaluable default.
It's the first non-`Copy` field type ever embedded in a record/array, which
required `default_value_expr`/`record_fields_are_copy` (`codegen.rs`)
changes: array defaults use `core::array::from_fn` instead of `[x; N]`, and
records containing a `__SlateVaArgs`-shaped field skip `#[derive(Copy)]`
(keep `Clone`). This check is direct-field-only — a struct containing another
struct that embeds a `va_list` isn't detected; would need a module-wide
fixed-point pass if it comes up.

`__SlateVaArg`/`__SlateVaArgs` are emitted as a raw string literal in
`SupportModule` rather than proper AST — deliberate, matching how the
`LongDouble` prelude was originally built before conversion; don't build the
proper-AST version unless asked.

## Known gap: `va_list*` pointer places

`lower_load`'s va-list guard fires whenever the _loaded type_ is
va-list-shaped, without distinguishing "loading a by-value `va_list`" (where
propagating the underlying place is correct — the loaded value _is_ the same
storage) from "loading a genuine `va_list*` pointer value" (where the load
produces a pointer, and using its place directly skips a level of
indirection). Symptom: `va_arg(*ap, int)` inside a function taking
`va_list *ap` lowers to `ap.next_arg::<i32>()` instead of
`(*ap).next_arg::<i32>()`.

Recommended fix (not yet applied): narrow the guard to only fire for the bare
value shape (`Type::VaList` / `is_boxed_va_args_type`), letting the pointer
case fall through to an ordinary pointer load; then have `va_target_place`
fall back to the existing `place_or_deref_expr` (already used elsewhere for
"no direct place → deref the pointer operand") instead of bare `place_expr`.

## The hard case: malloc'd va_list arrays with a real `vprintf` call

Needs a design decision, not just a lowering fix. `va_list *ap_array[3]`
populated via `malloc` with pointer arithmetic, feeding a **real**
`vprintf(s, **ap_ptr)` call. This forces native `core::ffi::VaList` (per the
whole-file native-ABI rule) _and_ needs the va_list to live in raw
`malloc`'d memory — but `core::ffi::VaList` has no defined layout Slate can
reconstruct from an arbitrary buffer safely, and the boxed `__SlateVaArgs`
representation can't be consumed by a real libc `vprintf` call.

Two directions: (1) shim `vprintf` itself against `__SlateVaArgs` — a
from-scratch format-string interpreter, letting anything forcing native
_solely_ because of a vprintf/vfprintf call box like everything else instead;
broadly reusable. (2) reinterpret `malloc`'d bytes as `core::ffi::VaListImpl`
and hope the byte layout matches `__va_list_tag` on the target ABI — a
one-off, "morally UB, works in practice" hack for a single test case. Option
1 fits the project's general "solutions should scale" preference better, but
is meaningfully more work and needs a decision before starting since it
changes the native-va_list calculus project-wide.
