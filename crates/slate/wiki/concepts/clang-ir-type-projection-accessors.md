# clang-ir type projection accessors

_created 2026-08-24_

`clang-ir-types-gen` owns structural projections on generated `Type` values.
The generated API provides `as_array`, `as_vector`, `as_integer`, `pointee`,
`as_function`, and `record_name`; the hand-maintained clang-ir parser tests
exercise those methods against parsed CIR syntax.

Slate must resolve `Type::Named` through its type-alias table before applying
structural projections. It must not recover integer signedness or width by
decoding alias spellings such as `s32i`. Slate's `slate_record_name` remains
local because stripping the `rec_` alias prefix is a Slate naming policy, while
the generated `record_name` accessor only projects concrete struct/union data.
