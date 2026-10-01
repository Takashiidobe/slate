# Vendored crates

Generated Cargo crates depend on adapted support crates under
`crates/slate/vendor/`:

| Crate | Purpose |
| --- | --- |
| `aligned` | Explicitly aligned storage |
| `bitint` | Arbitrary-width integer representation |
| `num-complex` | Complex number representation |
| `bitfields` | Bit-field support |

Target facts come from slate-parser's `TargetInfo` registry. Target C headers
are installed by slate-sysroots. C runtime bridges live under
`crates/slate/src/frontend/shims/` and are compiled into generated crates when
needed.
