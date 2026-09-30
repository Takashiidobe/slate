# MSVC oracle

<!-- toc -->
- [Install](#install)
- [What it can answer](#what-it-can-answer)
- [Corpus sweep](#corpus-sweep)
- [clang/test MS-mode sweep](#clangtest-ms-mode-sweep)
- [Gotchas](#gotchas)
<!-- /toc -->

`tools/cl.exe` wraps real `cl.exe` under Wine via
[mstorsjo/msvc-wine](https://github.com/mstorsjo/msvc-wine) (`bin/x64/cl`).

## Install

Outside the repo, ~5.5 GB, needs `wine` and `msitools`; accepts
Microsoft's license.

```
git clone --depth 1 https://github.com/mstorsjo/msvc-wine ~/.local/share/msvc-wine
cd ~/.local/share/msvc-wine
python3 vsdownload.py --accept-license --dest ~/.local/share/msvc-wine/opt/msvc
WINEDEBUG=-all ./install.sh ~/.local/share/msvc-wine/opt/msvc
```

`MSVC_WINE_ROOT` and `MSVC_ARCH` (default `x64`) override location and
architecture.

## What it can answer

- `/E`, `/P`: preprocessed output.
- `/Zs`: syntax check with MSVC error codes.
- `/d1reportAllClassLayout`: record layout, C++ only (compile as `.cpp`).
- `_Static_assert` probes on sizeof/alignof/offsetof.
- No AST dump.

## Corpus sweep

```
python3 tools/corpus_sweep.py --tool msvc --fixtures tests/fixtures/clang/linux/x86_64/add.c
```

- Runs `/Zs`. Fixtures with identical defines, include paths, and mode go
  in batches of up to 32; a failing batch is split to find the culprits.
- C89, C99, C23 fixtures run as C17 (`cl.exe` lacks those modes).

## clang/test MS-mode sweep

`tools/llvm_lit_sweep.py <out.jsonl>` runs every Windows-triple or
`%clang_cl` RUN line in `~/llvm-project/clang/test/**/*.c` through slate
(both flavors), `clang --target=<triple> -fsyntax-only` with the slate
sysroot as `-isystem`, and `tools/cl.exe /Zs`.

- Skips driver, PCH, CIR, `-E`-only runs, and `-verify` runs expecting
  errors.
- Pins slate to the oracle's standard (gnu17 for clang, the mapped `/std:`
  for msvc).
- `--migrate tests/fixtures/suites/clang-test` writes new fixtures (never
  overwriting) as `<flavor>/windows/<arch>/<Dir>__<stem>.c`.

## Gotchas

- C mode defaults to pre-C11; pass `/std:c11` or `/std:c17` for
  `_Static_assert` or get cascading C2143 errors.
- Use `/Z7`, not `/Zi` (`mspdbsrv` is flaky under Wine).
- Wine startup is slow per call; use `--fixtures <file.c>` for a targeted
  check.
