# Heap Allocations

Heap facts account for what happens between allocation and release. An eligible
local byte pointer can own its allocation when it has no escaping alias, no use
after release, and supported writes such as assignment from its own `realloc`. A
callee that can free the buffer blocks the proof.

## Track extent and initialization

For guarded typed allocations, `guarded_heap::analyze` recognizes a conditional
allocation and a following success check. It tracks each proven offset from the
allocation base. A dereference needs an in-bounds element, a read needs an
initialized element, and a release needs the base pointer. The initialized
prefix can be shorter than the allocated extent.

## Use the result

A byte buffer can lower to `Vec`, with eligible resizing expressed as a vector
operation. A guarded allocation can lower through a fallible allocation helper.
Sparse initialized slots can require `MaybeUninit`. Lowering keeps an explicit
drop for an early release and can use scope-exit drop for a terminal release.

A pointer with an escaping use or an unproven access keeps the C allocation and
pointer operations.
