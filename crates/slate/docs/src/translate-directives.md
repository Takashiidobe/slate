# Translate Directives

`translate` translates one preprocessing configuration: the host target, or the
one passed with `--target=`. `#if` branches that the configuration does not
select are dropped, not turned into Rust cfgs.

`--targets=<triple>,<triple>` translates the file once per target and merges the
results with the same engine as `translate-project --merge`. Items that differ
between targets get `#[cfg(target_arch = ...)]`-style gates, and a
`compile_error!` guards every target that was not translated. Feature macros are
not explored; use `translate-project --merge --matrix` for those.

```bash
cargo run --release -p slate-c2rust -- translate \
  --targets=x86_64-unknown-linux-gnu,aarch64-unknown-linux-gnu input.c
```

Target headers come from slate-sysroots.
