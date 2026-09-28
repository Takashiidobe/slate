# MSVC oracle

Real `cl.exe` runs on Linux under native Wine via
[mstorsjo/msvc-wine](https://github.com/mstorsjo/msvc-wine). `tools/cl.exe`
is a thin wrapper over its generated `bin/x64/cl`. The optional corpus sweep
runs it as a syntax oracle.

## Install

Outside the repo (about 5.5 GB). Needs `wine` and `msitools`. Downloading
means accepting Microsoft's license.

```
git clone --depth 1 https://github.com/mstorsjo/msvc-wine ~/.local/share/msvc-wine
cd ~/.local/share/msvc-wine
python3 vsdownload.py --accept-license --dest ~/.local/share/msvc-wine/opt/msvc
WINEDEBUG=-all ./install.sh ~/.local/share/msvc-wine/opt/msvc
```

`MSVC_WINE_ROOT` and `MSVC_ARCH` (default `x64`) override the location and
target architecture for `tools/cl.exe`.

## What it can answer

- `/E`, `/P`: preprocessed output.
- `/Zs`: syntax check only, with MSVC error codes.
- `/d1reportAllClassLayout`: record layout, C++ only (compile as `.cpp`).
- `_Static_assert` probes on sizeof/alignof/offsetof.

There is no AST dump, so it cannot replace `clang -ast-dump`.

## Corpus sweep

Check one fixture without compiling or linking an executable:

```
python3 tools/corpus_sweep.py --tool msvc --fixtures tests/fixtures/clang/linux/x86_64/add.c
```

`/Zs` stops after syntax checking. Fixtures with identical defines, include paths,
and language mode are sent to `cl.exe` in batches of up to 32. If a batch fails,
the sweep splits it to identify which fixtures failed. C89, C99, and C23 fixture
modes use MSVC's C17 mode because those modes are unavailable in `cl.exe`.

## clang/test MS-mode sweep

`tools/llvm_lit_sweep.py <out.jsonl>` runs every Windows-triple or
`%clang_cl` RUN line in `~/llvm-project/clang/test/**/*.c` through slate
(both flavors), `clang --target=<triple> -fsyntax-only` with the slate
sysroot as `-isystem`, and `tools/cl.exe /Zs`. Driver, PCH, CIR and `-E`-only
runs are skipped, and so are `-verify` runs whose prefixes expect errors.
The sweep pins slate to the oracle's effective standard (gnu17 for clang,
the mapped `/std:` for msvc) because slate's gnu23 default would mask other
gaps. `--migrate tests/fixtures/suites/clang-test` writes fixtures for newly
accepted files (never overwriting) as
`<flavor>/windows/<arch>/<Dir>__<stem>.c`, so the directory carries the
compiler and target.

## Gotchas

- C mode defaults to a pre-C11 dialect. Pass `/std:c11` or `/std:c17`
  for `_Static_assert`, or you get a cascade of C2143 syntax errors.
- Avoid `/Zi`: it needs `mspdbsrv`, which is flaky under Wine. Use `/Z7`.
- Each invocation has Wine startup overhead, so the corpus sweep batches compatible
  fixtures. Use `--fixtures <path/to/file.c>` for a fast targeted check.
