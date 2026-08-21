# intrinsicgen

[Intrinsicgen](https://github.com/Takashiidobe/intrinsicgen) is a crate
that slate uses to generate intrinsics headers for x86, arm, and riscv.

It works by parsing the sources that generate intrinsics for each ISA.
Intel has an xml file, arm has a json file, and riscv uses a python
program (`rvv_intrinsic_gen`) to programatically generate intrinsics.

Intrinsicgen has an
[IR](https://github.com/Takashiidobe/intrinsicgen/blob/main/src/ir.rs)
that is used to normalize each of the different architecture's
intrinsics into one form.

Any backend can iterate through all of the items in the IR and then
generate what it wants, for example, generating C headers:

[Header
Generation](https://github.com/Takashiidobe/intrinsicgen/blob/main/src/ir.rs#L417-L453)

Slate will extend this mechanism to generate a mapping from C intrinsic
to rust intrinsics, since intrinsics can't be ported by translating the
underlying C code.
