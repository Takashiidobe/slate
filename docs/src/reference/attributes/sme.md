# AArch64 SME attributes

Slate records these on AArch64 function declarations and definitions. GCC also
accepts corresponding `[[arm::...]]` spellings such as `[[arm::streaming]]`.
State arguments are `"za"` or `"zt0"`. Translation to Rust stops when it
requires an SME function state.

- `__arm_streaming` requests streaming mode at the function boundary.
- `__arm_streaming_compatible` marks a function callable in either mode.
- `__arm_locally_streaming` requests streaming mode for the function body.
- `__arm_new(STATE)` requests a new state local to the function.
- `__arm_in(STATE)` marks state as input.
- `__arm_out(STATE)` marks state as output.
- `__arm_inout(STATE)` marks state as input and output.
- `__arm_preserves(STATE)` marks state that the function preserves.

## `__arm_streaming`

Translate with `--target=aarch64-unknown-linux-gnu`.

```c
int inc(int x) __arm_streaming { return x + 1; }
```

Slate records the state in IR. Rust translation stops with this diagnostic:

```text
function inc: SME function state `Streaming`
```
