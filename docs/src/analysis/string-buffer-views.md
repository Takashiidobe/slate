# String and Buffer Views

A pointer to bytes can be used as a C string, a buffer, or an ordinary pointer.
Analysis checks its origin and uses before offering a view to lowering.

- **Constant strings.** `constant_strings::analyze` follows a local's
  initializer to a string literal through casts and other eligible locals. The
  local must be written once, and at least one recognized string call must have
  views for all its string arguments. Bytes beyond an interior NUL can block the
  view when another use could observe them.
- **C string arrays.** `c_string_arrays::analyze` checks that a local `char[N]`
  starts with a terminator and that each allowed write leaves one. A write such
  as a nonzero element assignment, or passing the array to an unknown function,
  can block the fact.
- **Byte buffer aliases.** `byte_buffers::analyze` can associate a local pointer
  with a byte array and a constant offset when the pointer is initialized once
  and used only as a buffer argument to recognized library calls. An ordinary
  read through the pointer or an escape blocks that alias fact.

Lowering uses the matching fact to select a string view or a byte slice. For a
memory copy it also checks the accessed range and whether the source and
destination share a root. An overlapping move within the same slice can use
`copy_within`.
