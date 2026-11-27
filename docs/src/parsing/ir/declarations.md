# Declarations and Initialization

Declarations give later stages the information needed to emit storage and link
symbols. Initializers keep their aggregate shape so Rust can build a value from
resolved fields and elements.

## Read a static aggregate

**C input**

```c
struct row { int values[3]; };
static struct row r = { .values[1] = 4, 5 };
int get(void) { return r.values[0]; }
```

**Typed IR excerpt**

```text
global %1 r: @type0 [storage=static] =
    aggregate<@type0, zero_fill=false>(
        field0 = aggregate<array<i32, 3>, zero_fill=true>(
            index1 = const<i32>(4),
            index2 = const<i32>(5)
        )
    ) [linkage=internal];
```

The global has static storage and internal linkage. Storage duration describes
how long the object exists. Linkage describes which translation units can name
it. The project translator uses that distinction when deciding which
declarations can resolve to this symbol.

The initializer names `field0`, then array elements `index1` and `index2`. The
nested `zero_fill=true` accounts for the omitted first element. Sema has already
resolved the designator and the following positional value. The Rust lowerer can
build the aggregate without walking C's initializer syntax. Required zero
initialization also stays distinct from an uninitialized automatic binding.
