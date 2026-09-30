# Fix four differential failures exposed by FileCheck generation

_created 2026-08-29_

Full FileCheck generation made four previously unselected fixtures enter the
differential batch and exposed translator correctness failures.

- enum-pointer record fields now cast to CIR's underlying pointer type on load
- function-pointer arithmetic transmutes from its actual byte-pointer type
- typed CIR `f128` constants retain the full decimal spelling instead of
  round-tripping through `f64`
- long-double/BitInt casts bridge through signed or unsigned 128-bit shims

The four fixtures now carry regenerated lowering and rewrites checks. Both full
release differential profiles pass.
