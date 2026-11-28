# Diagnostics and the Checker Contract

The checker decides how the selected C flavor interprets a construct. It records
decisions such as operand conversions, merged declaration types, and initializer
plans. IR lowering reads those decisions. A conversion rule belongs in the
checker so validation and emission use the same result.

## Check a control flow target

**C input**

```c
void stop(void) {
    while (1)
        break;
}
```

**IR excerpt**

```ir
fn %0 @stop() -> void [linkage=external] [fallthrough=ret_void] {
    while %1 ne<i32>(const<i32>(1), const<i32>(0))
        break %1;
}
```

The checker accepts the `break` inside a loop. The IR names the loop directly
with `break %1`. Moving the same statement outside the loop leaves it without a
target:

```c
void stop(void) { break; }
```

Slate reports `break outside loop or switch` and emits no IR.

## Route a failure to the stage that owns it

- **Rejected input.** The checker reports an invalid construct with its source
  location. For example, a `break` outside any loop or switch gets a C
  diagnostic.
- **Unimplemented semantics.** A construct can be valid C and still lack a Slate
  model. `ResolveError::Unimplemented` identifies that coverage gap.
- **Missing checker fact.** If the emitter needs a fact that the checker should
  have supplied, `ResolveError::Internal` identifies a Slate bug.

`Sema::analyze` runs the checker and structural validation before `Sema::lower`
emits a module. Start with `sema/assertion.rs` for checker rules,
`sema/validate.rs` for structural checks, and `sema/numeric.rs` for the error
variants.

See [Error Handling](../../reference/errors.md) for how these errors propagate
through Slate and become diagnostic reports.
