# Calling convention attributes

- `cdecl`, `stdcall`, `fastcall`, `thiscall`, `vectorcall`, `regcall`, `ms_abi`,
  and `sysv_abi` are covered individually in
  [Calling Conventions](../calling-conventions.md), including their keyword
  spellings.
- `regparm(N)` parses an integer register-count request. Slate does not apply it
  to an individual function. See `-mregparm=N` in
  [Calling Conventions](../calling-conventions.md#flags-that-change-the-default).
- `pcs("aapcs")` parses an Arm base calling convention request. It does not
  override the target ABI in IR.
- `pcs("aapcs-vfp")` parses an Arm hard-float convention request. It does not
  override the target ABI in IR.
- `preserve_all` and `preserve_none` parse when registered for the flavor and
  target. Slate does not apply them to the function ABI.
- `preserve_most` and `swiftcall` produce an unsupported convention diagnostic
  when registered.
- `nomips16` has a parser representation. The registry disables the GNU spelling
  because Slate has no MIPS target.

## `ms_abi`

```c
__attribute__((ms_abi)) int inc(int x) { return x + 1; }
```

For `--target=x86_64-unknown-linux-gnu`, Slate gives the Rust function an
explicit Windows x64 ABI.

```rust
extern "win64-unwind" fn inc(x: i32) -> i32 {
    x + 1
}
```

## `preserve_most`

```c
__attribute__((preserve_most)) int inc(int x) { return x + 1; }
```

Clang mode recognizes the spelling and stops with
`` unsupported calling convention `preserve_most` ``.
