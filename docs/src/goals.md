# Goals

## Support all of C

Support every C standard from C89 to C2y and all extensions in GCC, Clang, and
MSVC.

## Translate across platforms

Translate the same project for different operating systems and architectures,
preserving each target's data layout and calling conventions. Where a project
supports multiple targets, generate one Rust crate that builds for them.

## Generate Rust people can maintain

Use pointer analysis to turn raw pointers into references and slices where the
program permits. Keep source comments with the code they describe and use macro
definitions and expansions to recover named constants and other abstractions in
the generated Rust.

## Preserve observable behavior

Preserve all observable behavior of the C program for the same target and
compiler settings, including output, exit status, and interactions with other
code. Every rewrite must preserve that behavior.
