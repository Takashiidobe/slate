# Function Identity

A call to a function named `strlen` may target a system declaration or a C
function defined by the program. Analysis classifies the declaration before
other passes use library behavior as evidence.

`identify` checks the symbol, whether the unit is hosted, and the declaration's
header provenance. The catalog in `crates/slate-analysis/src/identity.rs`
records the known symbols and their headers. A recognized identity lets pointer
analysis classify a call to `free` as a release, for example. The same identity
lets lowering select a library-specific implementation.

`UnitFacts::identity` looks up the result by function binding. An unknown
identity keeps ordinary call handling. Plain libc names in freestanding units
have no libc identity.
