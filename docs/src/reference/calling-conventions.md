# Calling Conventions

A calling convention determines how a caller passes arguments and receives a
result. Slate selects the default from the target and compiler options. An
explicit convention is part of the function type, including when the function is
called through a pointer.

Save this as `callback.c` to translate a 32-bit Windows callback.

```c
typedef int (__stdcall *callback)(int);

int __stdcall increment(int x) { return x + 1; }
int invoke(callback f, int x) { return f(x); }
```

```sh
slate translate --flavor=msvc --target=i686-pc-windows-msvc callback.c
```

Slate emits `extern "stdcall-unwind"` for the definition and the callback type.
Use the same convention on the function and the pointer used to call it. See
[Calls and ABI](../parsing/ir/calls.md) for how to inspect a convention in IR.

## Default C convention

- `__cdecl` selects the ordinary C convention for the target in Slate's GCC,
  Clang, and MSVC modes. GCC and Clang modes also accept
  `__attribute__((cdecl))` on x86.
- On 32-bit x86, cdecl leaves stack cleanup to the caller. The target determines
  the ABI on other architectures. For example, x86-64 Linux defaults to System V
  and x86-64 Windows defaults to Win64.

## x86 calling conventions

Slate accepts the keyword spellings below in GCC, Clang, and MSVC modes. These
entries describe the spellings Slate recognizes. The original compiler may
accept a different set of spellings. Variadic signatures use the ordinary C
convention. On x86-64, only vectorcall and regcall remain distinct from the
ordinary C convention.

### `__stdcall`

- GCC and Clang modes also accept `__attribute__((stdcall))`.
- On i686, the callee cleans up stack arguments. Slate emits
  `extern "stdcall-unwind"`.

### `__fastcall`

- GCC and Clang modes also accept `__attribute__((fastcall))`.
- On i686, eligible integer arguments use registers. Slate emits
  `extern "fastcall-unwind"` for signatures that rustc passes the same way as
  the C compiler.
- Slate rejects known register assignment differences. For example, an integer
  after a `long long` can go in a register in Clang and on the stack in rustc.

### `__thiscall`

- GCC and Clang modes also accept `__attribute__((thiscall))`.
- On i686, this convention can pass the first argument in `ecx`. Slate emits
  `extern "thiscall-unwind"`.

### `__vectorcall`

- Clang mode also accepts `__attribute__((vectorcall))`.
- On i686 and x86-64, eligible floating-point and vector arguments use vector
  registers. Slate emits `extern "vectorcall-unwind"` when rustc matches the C
  ABI for the signature.
- Slate rejects record or vector signatures when Rust and C pass them
  differently. The i686 fastcall register assignment limits also apply.

### `__regcall`

- Clang mode also accepts `__attribute__((regcall))`.
- Slate records regcall in x86 and x86-64 function types and reports a lowering
  error when Rust output requires that convention.

## Microsoft and System V ABI attributes

An effective ABI override stays on variadic signatures. Variadic definitions and
some signatures involving `long double` can fail to lower.

### `__attribute__((ms_abi))`

- GCC and Clang modes support this attribute on x86-64. On a non-Windows target,
  it selects the Microsoft ABI and lowers to `extern "win64-unwind"` for
  supported signatures. On Windows it selects the ordinary C convention.
- Clang mode also records the Windows ARM64 ABI on non-Windows AArch64. Slate
  reports a lowering error when Rust output needs that ABI override.

### `__attribute__((sysv_abi))`

- GCC and Clang modes recognize this attribute on x86-64. In Clang mode on
  Windows, it selects System V and lowers to `extern "sysv64-unwind"` for
  supported signatures.
- On x86-64 Linux it selects System V, including on individual declarations when
  GCC's `-mabi=ms` changes the default.

## Flags that change the default

- `/Gd` selects cdecl in MSVC mode.
- `/Gr` selects fastcall in MSVC mode on i686.
- `/Gz` selects stdcall in MSVC mode on i686.
- `/Gv` selects vectorcall in MSVC mode on i686 and x86-64.
- `-mrtd` selects stdcall for eligible non-variadic functions in GCC and Clang
  modes on i686. Clang keeps `main` cdecl.
- `-mabi=ms` selects the Microsoft ABI by default in GCC mode on x86-64 Linux.
- `-mabi=sysv` selects System V again on x86-64 after GCC's `-mabi=ms`.
- `-mregparm=N` changes integer argument register use for eligible functions in
  GCC and Clang modes on i686. The generated project uses the matching rustc
  setting. Slate ignores the per-function `regparm(N)` attribute.
- `-msseregparm` changes floating-point argument or result passing in GCC mode
  on i686. Slate records the convention in IR and reports a lowering error for
  signatures that require it.

MSVC's variadic functions, `main`, and `wmain` keep cdecl under the `/G`
defaults. Windows entry points named `WinMain`, `wWinMain`, or `DllMain` default
to stdcall on i686 and cdecl on x86-64.

## Unsupported Clang conventions

- `__attribute__((preserve_most))` produces an unsupported calling convention
  diagnostic.
- `__attribute__((swiftcall))` produces the same diagnostic for Swift's calling
  convention.
