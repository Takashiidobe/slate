# Dead Temporaries

<div class="transform-pass">

## Purpose

Use `dead-temporaries` to select this pass. Remove unread synthetic result slots
while keeping the expressions that produced their values.

## Example

**C input**

```c
int f(void);
int g(void);

void discard(int c) {
    c ? f() : g();
}
```

**Emitted Rust**

```rust
unsafe extern "C" {
    fn f() -> i32;
    fn g() -> i32;
}

fn discard(c: i32) {
    if c != 0 {
        unsafe { f() };
    } else {
        unsafe { g() };
    }
}
```

## Rewrite rules

```text
temp t; if C { write(t, E) } else { write(t, F) }
    → if C { eval(E) } else { eval(F) }
temp t = E → eval(E)
```

The result slot can disappear when it is a synthetic scalar with zero reads and
all references are direct writes. Every initializer or written value is still
evaluated at the original point. The example must call exactly one of `f()` and
`g()`, even though their results are discarded.

## Caveats

Changing the C statement to `return c ? f() : g();` adds a read of the result
slot. A VLA extent can also refer to a temporary through its type, even when the
statement list has no read. Both uses keep the slot. Source declarations and
storage in kept expansions stay in place.

</div>
