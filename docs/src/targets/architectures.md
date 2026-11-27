# Architectures

The architecture and operating system together determine type sizes, alignment,
and how arguments and return values cross the C ABI. Preserve architecture flags
from the original build because they can also affect predefined macros and which
intrinsics the source can use.

| Architecture        | Pointer width | Target-specific details                                                                                                                                                       |
| ------------------- | ------------- | ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| x86-64 (`x86_64`)   | 64 bits       | Linux uses the System V ABI by default. Windows uses Win64. Linux `long double` defaults to x87 extended precision.                                                           |
| 32-bit x86 (`i686`) | 32 bits       | Layout and argument passing differ from x86-64. On 32-bit Windows, conventions such as `stdcall` and `fastcall` affect where arguments go and who cleans up the stack.        |
| AArch64 (`aarch64`) | 64 bits       | Slate handles NEON builtins and selected SVE types and intrinsics. On Linux, `long double` defaults to binary128. On Apple silicon, it uses binary64.                         |
| 32-bit Arm (Arm32)  | 32 bits       | Linux Armv7 has soft-float (`gnueabi`) and hard-float (`gnueabihf`) calling conventions. The Windows `thumbv7a` target uses Thumb instructions and the hard-float convention. |

On Arm32 Linux, keep the original `-mfloat-abi`, `-mfpu`, and `-marm` or
`-mthumb` options when present. In particular, the float ABI determines how
floating-point arguments are passed across function boundaries.

See [Calls and ABI](../parsing/ir/calls.md) and [Long Double](../long-double.md)
for details on how Slate represents these differences.
