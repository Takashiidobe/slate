# prototype FileCheck updater

_created 2026-08-29_

`tools/update_filecheck.py` provides a deliberately mechanical baseline
generator for differential fixtures. It invokes `translate-lowered` and
`translate` in the harness's C23 mode, then writes the generated Rust as one
ordered profile-qualified check chain, including blank lines. Checkout-root
paths are matched with a regex. The resulting block is intentionally noisy and
is meant to be pared down by hand.

```bash
tools/update_filecheck.py tests/fixtures/add.c --profile both --in-place
```

Target-qualified output can be generated with repeated mappings:

```bash
tools/update_filecheck.py tests/fixtures/bionic/stdio_locale_conversion.c \
  --target BIONIC-AARCH64=aarch64-linux-android \
  --target BIONIC-X86_64=x86_64-linux-android \
  --profile both --in-place
```

Fixtures directly under `tests/fixtures/bionic`, `macos`, or `msvc` are skipped
unless at least one `--target` mapping is provided. This prevents a host
translation from being recorded as target ABI coverage. If translation for an
explicit target fails, the fixture is left unchanged.

Only blocks between `SLATE-FILECHECK-BEGIN/END` markers are replaced. Existing
hand-written directives remain outside those blocks. The generator does not
attempt to infer semantic expectations or normalize unstable output; those
decisions are made while manually reducing the generated block.

Embedded NUL bytes in generated Rust are emitted as `{{\\x00}}` regexes so
the C fixture remains a text file.
