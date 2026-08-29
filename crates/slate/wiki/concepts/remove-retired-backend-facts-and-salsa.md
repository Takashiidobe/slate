# Remove retired backend facts and Salsa

_created 2026-08-29_

The worklist rewrite engine no longer declares or consumes the old
`backend::facts` module. Delete its collectors and Salsa-derived data model
instead of keeping an unreachable copy of the pre-worklist analyses.

The same retirement applies to the undeclared `idents`, `runtime`, and
`support` helpers. Their only reverse dependencies were in the deleted facts
and query infrastructure, and `support::walk` depended directly on the removed
`facts::PathSegment` type.

Keep `backend::interproc` intact. Its pointer and length facts are live
worklist/lattice analyses and are not part of the retired Salsa subsystem.

Remove the direct `salsa` dependency and its transitive lockfile entries.
