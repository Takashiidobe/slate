# Goals

1. **Support all of C.** Support every C standard from C89 to C2y and all
   extensions in GCC, Clang, and MSVC.
2. **Translate across platforms.** Target x86_64, i686, aarch64, and arm32
   across Linux, macOS, Windows, FreeBSD, and Android. Generate one Rust crate
   for a project’s selected targets while preserving each target’s data layout
   and calling conventions.
3. **Generate Rust people can maintain.** Keep comments with the code they
   describe and recover abstractions from C macros. Use pointer analysis and
   rewrites to express the program in idiomatic Rust.
4. **Preserve observable behavior.** The original C program and the generated
   Rust should show no observable difference in execution on the same target
   with the same inputs.
