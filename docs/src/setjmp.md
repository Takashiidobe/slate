# `setjmp` and `longjmp`

`setjmp` saves an execution context in a C `jmp_buf` and returns zero on the
first pass. `longjmp` resumes at that saved call, which then returns the
supplied value, or 1 if the supplied value was zero. Slate lowers supported
calls using the target's C runtime and the target layout of `jmp_buf`.

```c
if (setjmp(env)) {
    record_failure("case");
    return;
}
inner_check(ok);
```

On x86-64 Linux with glibc, the header expansion selects `_setjmp`. The emitted
Rust calls it at the original branch, using a raw pointer to the buffer.
`inner_check` can call `longjmp` to resume at the branch. This excerpt gives the
emitted buffer pointer the name `env_ptr`.

```rust
if unsafe { _setjmp(env_ptr) } != 0 {
    record_failure(c"case".as_ptr());
    return;
}
inner_check(ok);
```

GNU `__builtin_setjmp` uses a compiler-specific buffer layout. Slate reserves
native jump-buffer storage in the translated caller's frame and stores a pointer
to it in the GNU buffer. The corresponding `__builtin_longjmp` follows that
pointer. Keep the saving function active until the jump and use
`lowering-barriers` to check support for the target and call shape.
[Nonlocal jump fixture](https://github.com/takashiidobe/slate/blob/main/crates/slate/tests/fixtures/setjmp_recovery_guard.c)
