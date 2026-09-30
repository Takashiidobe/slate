# model MSVC time and file status families

_created 2026-08-24_

The MSVC time and file-status surface follows the pinned xwin UCRT headers,
not the POSIX layouts selected by the target architecture.

- `time.h` owns `__time32_t`/`__time64_t`, the three timespec records,
  `struct tm`, and explicit 32/64-bit function variants.
- `sys/stat.h` owns the four `_stat` layouts, UCRT scalar typedefs, `_S_*`
  constants, and narrow/wide explicit variant declarations.
- `sys/utime.h` and `sys/timeb.h` own their explicit 32/64-bit records and
  declarations. The POSIX-only top-level `utime.h` rejects MSVC targets.
- Secure `_s` functions remain in `slate-wlpp.3.6`; macro aliases and imported
  symbol selection remain in `slate-wlpp.4`.

MSVC aggregate fixtures compile against both the shim and pinned xwin. They do
not enter CIR until `slate-wlpp.7` implements MS-ABI record definitions.
