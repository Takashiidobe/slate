# Long double

slate-parser's target information determines the C long-double format. Slate
lowers binary64 values as `f64` and x87 values using a `LongDouble` byte payload
and C runtime bridges.

The prelude and bridge declarations live in `frontend/long_double.rs`;
name-based bridge rendering lives in `frontend/c_shim.rs`. Runtime code is
under `frontend/shims/`. Project translation writes and builds required bridge
code; standalone Rust output may require additional build setup.

The [long-double guide](https://github.com/takashiidobe/slate/blob/main/wiki/concepts/long-double-f80.md)
records representation and ABI details. Target and call-shape coverage still
requires differential fixtures.
