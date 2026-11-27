# Interprocedural Lifts

Slate can change a function's Rust signature when it can prove that every
translated caller can supply the new argument type. Analysis computes the proof
on C IR for one target. Lowering uses the same result at the definition and each
call site, including direct calls resolved to another translation unit. The
project linker binds lifted cross-unit calls through Rust paths.

Analysis resolves direct calls by definition or external link name. It tracks
argument-to-parameter and return-to-result flow. Requirements such as write,
offset, free, or escape propagate back toward the pointer source. Nullability
flows from sources toward their uses. An unresolved call can escape its pointer
arguments. Fixed C ABI boundaries, address-taken functions, and variadic
functions retain their signatures.

## Pointer to Reference

**C input**

```c
static void bump(int *p) { *p = *p + 1; }
static int peek(int *p) { return *p + 1; }

/* In the caller */
int a = 1;
int b = 10;
bump(&a);
int peeked = peek(&b);
```

**Emitted Rust**

```rust
fn bump(p: &mut i32) {
    *p += 1;
}

fn peek(p: &i32) -> i32 {
    *p + 1
}

// In the caller
let mut a: i32 = 1;
let b: i32 = 10;
bump(&mut a);
let peeked: i32 = peek(&b);
```

A read-only single-object parameter can become `&T`. A write through the
parameter requires an exclusive borrow, so the caller must prove `&mut T`. When
a borrow is otherwise valid and a caller can supply null, lowering can use
`Option<&T>` or `Option<&mut T>`.

Keep the raw pointer when the object can escape, the access requires pointer
operations outside the borrow, or alignment cannot be proved. For example,
`update(&x, &x)` cannot supply two independent mutable references. If either
aliased parameter writes, both stay raw.

## Pointer and Length to Slice

**C input**

```c
static int sum(int *items, int len) {
    int total = 0;
    for (int i = 0; i < len; i++)
        total += items[i];
    return total;
}

/* At the call site */
sum(values, 4);
```

**Emitted Rust**

```rust
fn sum(items: &[i32]) -> i32 {
    let len: i32 = items.len() as i32;
    let mut total: i32 = 0;
    for i in 0..len {
        total += items[i as usize];
    }
    total
}

// At the call site
sum(values.0.as_slice());
```

The fixture stores the four-element array in an alignment wrapper, so the caller
borrows `values.0`. The signature rule is:

```text
f(T* p, integer n) + proof that n matches the slice extent
    → f(p: &[T]) with n reconstructed from p.len()
    → f(p: &mut [T]) if the callee writes and callers prove exclusivity
```

Every caller must supply a compatible extent. A pointer into unknown storage or
a count that differs from the proposed slice length blocks that proof. For
example, passing 2 alongside a four-element array cannot be rewritten as a
full-array borrow with a reconstructed count of 4. The pointer can still lift in
cases where Slate can prove an appropriate view or keep the count separate.

## C String Parameter to CStr

**C input**

```c
static int is_keyword(const char *word) {
    return strcmp(word, "if") == 0 || strcmp(word, "while") == 0;
}
```

**Emitted Rust**

```rust
fn is_keyword(word: &core::ffi::CStr) -> i32 {
    (word == c"if" || word == c"while") as i32
}
```

The caller passes a C string literal such as `c"if"` or forwards a parameter
with the same proof. Forwarding is checked across the call graph, so
`keyword_length(word)` can accept `&CStr` and pass it on to `is_keyword(word)`.
The borrow includes the NUL terminator.

The parameter must be used as a string and may not be written, freed, returned,
nullable, or escaped. An unknown caller, an interior NUL in a literal, or
advancing the pointer to a different address can keep the parameter raw.

## Test the Caller and Callee Together

Use a Slate differential fixture that checks both the generated signature and
the rewritten calls. Add a caller that blocks the proof when changing an
eligibility rule. For cross-unit changes, put the declaration and definition in
different files in a project fixture. See [Testing](../testing.md).
