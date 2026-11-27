# Call Effects

Pointer analysis uses effect summaries to check whether a callee can reach
storage through another path. For example, passing a mutable static array as a
slice can create a Rust borrow while the callee still has access to the array by
name.

## Build a summary

The pointer walk records each function's reads and writes of mutable internal
statics and its direct callees. Indirect calls, unknown external calls, and
opaque operations such as nonempty assembly can make the function opaque.
Opacity propagates to callers. For a queried static, a reverse walk from the
functions that name it finds callers that can reach it.

## Use a summary

A call can supply a borrow of a mutable static array only if the address stays
private, the callee is visible to the analysis, and the callee cannot reach the
array by name. Another argument that uses the same array can also block the
proof. If the array's address is stored in another global, Slate keeps the
pointer form.

Start with `Effects` in `crates/slate-analysis/src/effects.rs`. These summaries
support pointer facts during analysis.
