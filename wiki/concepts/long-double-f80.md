# How Slate represents C long double

Slate chooses the Rust representation of C `long double` from the target ABI. It
emits `f64` for IEEE binary64, `f128` for IEEE binary128, and a generated
`LongDouble` type for x87 extended precision.

## Formats by target

| C format                     | Rust representation | Representative targets                              |
| ---------------------------- | ------------------- | --------------------------------------------------- |
| IEEE binary64                | `f64`               | Windows MSVC, Apple silicon macOS, 32-bit ARM Linux |
| x87 extended precision (f80) | `LongDouble`        | x86-64 Linux, i686 Linux, Intel macOS               |
| IEEE binary128               | `f128`              | AArch64 Linux and Android, x86-64 Android           |

These are default formats from Slate's
[target definitions](../../crates/slate-parser/src/target_info.rs). Runtime
coverage for a target and call shape comes from the
[differential fixtures](../../crates/slate/tests/).

## Why x87 needs a separate type

Rust has no primitive for x87 extended precision. Converting to `f64` would
narrow the value and can change arithmetic and comparisons. Slate stores the
80-bit payload in a `[u8; 10]` inside `LongDouble` and gives the type the
alignment required by the target. On x86-64 Linux, the value occupies 16 bytes
in memory, with 16-byte alignment. Slate delegates arithmetic, comparisons, and
casts to C helpers.

Storage layout and calling convention are separate. Even with the right size and
alignment, Rust passes `LongDouble` as a struct. Native C can use a different
convention for x87 values, such as returning a scalar `long double` in the x87
register stack. Slate generates C bridges for supported signatures that pass or
return x87 values by value. The bridges convert between the Rust wrapper and
native C values. This also matters for callbacks and variadic calls.

Project translation writes the required bridge code to `src/slate_long_double.c`
and builds it from `build.rs`. If you compile standalone Rust output that uses
x87 helpers or bridges, compile and link the C shim as part of your build.

See the [Rust wrapper](../../crates/slate/src/frontend/long_double.rs),
[C bridge generator](../../crates/slate/src/frontend/c_shim.rs), and
[native ABI fixture](../../crates/slate/tests/fixtures.dylink/long_double_abi/)
for implementation details and tested call shapes.
