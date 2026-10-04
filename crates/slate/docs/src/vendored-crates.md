# Vendored crates

Generated Cargo crates depend on adapted support crates under
`crates/slate/vendor/`:

Embedded support manifests use `Cargo.toml.template`: Cargo excludes nested
packages containing `Cargo.toml` from published archives. The translator writes
these templates as `Cargo.toml` in generated projects.

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
