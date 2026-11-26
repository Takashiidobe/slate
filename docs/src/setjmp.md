# setjmp and longjmp

Nonlocal jumps need special care across Rust frames and compiler optimizations.
Coverage must be established with differential fixtures using the target ABI.
Inspect `lowering-barriers` for the input rather than assuming parser acceptance
establishes translation support.

The [historical index](history.md) preserves earlier lowering and recovery
experiments. Current behavior is defined by the supported fixture buckets.
