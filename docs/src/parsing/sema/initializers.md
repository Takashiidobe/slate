# Initializers

The parser keeps initializer braces and designators. The checker walks the
destination type, selects the subobject for each initializer, and records the
conversion that the value needs.

**C input**

```c
struct row { int values[3]; };
struct row r = { .values[1] = 4, 5 };
```

**Typed IR excerpt**

```text
type @type0 row = struct {
    field0 values: array<i32, 3>;
} [size=12, align=4, offsets=[0]];
global %1 r: @type0 [storage=static] =
    aggregate<@type0, zero_fill=false>(
        field0 = aggregate<array<i32, 3>, zero_fill=true>(
            index1 = const<i32>(4),
            index2 = const<i32>(5)
        )
    ) [linkage=external];
```

The nested aggregate names `index1` and `index2`. Its `zero_fill=true` accounts
for the omitted element.

The designator starts at `values[1]`. The following value initializes
`values[2]`, and the omitted `values[0]` is zero-initialized. The checker
records this as an `InitializerPlan` keyed by the declarator or compound
literal. The emitter applies the plan in order and builds structured IR with the
field and element selections resolved.

This matters when rewriting C declarations into Rust literals or initialization
code. Rust lowering receives the selected destinations, conversions, and
required zero fill. A local declared without an initializer has different
semantics from a partially initialized aggregate. Slate also records flag-driven
automatic variable fills separately.

Start with `record_initializer` and `InitializerPlan` in `sema/initializer.rs`.
