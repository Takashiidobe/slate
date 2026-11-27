# Printf Formats

Format analysis starts with a known library call and a literal format. It
recognizes supported directives and checks the matching argument types and
places. The result describes the format at that call, so lowering can choose the
Rust formatting and preserve the argument values.

For example, a recognized `printf("%d\n", n)` call can lower to
`println!("{}", n)` where the target and use allow it. A runtime format, an
unsupported directive, or an argument that would require borrowing an
underaligned field can keep the C call. Target and stream behavior can also
limit lowering, including Windows text-mode newline translation.

`printf::analyze` produces the format facts. Lowering looks up a `PrintfFormat`
with `UnitFacts::printf_format` using the call's node ID.
