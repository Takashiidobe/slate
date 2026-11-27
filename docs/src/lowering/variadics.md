# Variadic Calls and `va_arg`

Slate passes C variadic arguments using the target ABI and reads them in the
order requested by `va_arg`. The call site applies C's default promotions, such
as passing a variadic `float` as `double`. At the receiving end, `va_arg`
specifies the promoted type the callee expects.

## Read a mixed aggregate on x86-64 System V

```c
#include <stdarg.h>

struct mixed { double d; long l; };

long read_mixed(int count, ...) {
    va_list ap;
    va_start(ap, count);
    struct mixed m = va_arg(ap, struct mixed);
    va_end(ap);
    return (long)m.d + m.l;
}
```

A caller can pass `read_mixed(1, (struct mixed){4.0, 5})`. On x86-64 Linux, the
first eight bytes of the struct have the SSE class and the second eight have the
integer class. When both register classes have space, the pieces arrive in
separate registers. An ordinary scalar `va_arg`, such as `va_arg(ap, long)`, can
use Rust's `VaList::next_arg` on this target. The aggregate needs a helper that
can reassemble the two pieces.

```rust
let m: mixed = unsafe {
    __slate_sysv_va::arg::<_, mixed>(core::ptr::addr_of_mut!(ap), 6u32)
};
```

`6` encodes the SSE class followed by the integer class. The helper uses the
floating-point and general-purpose offsets in the `va_list` to find both pieces
in the register save area, copies them into the struct in field order, and
advances both offsets. If either class lacks room, the whole argument comes from
the aligned stack overflow area instead. In that case only the stack cursor
advances.

`va_copy` gives the recipient a cursor at the same position as the original. The
fixture copies a list and reads the same argument from each copy, alongside
tests for register and stack arguments.
[Variadic aggregate fixture](https://github.com/takashiidobe/slate/blob/main/crates/slate/tests/fixtures/x86_64/va_arg_aggregates.c)
