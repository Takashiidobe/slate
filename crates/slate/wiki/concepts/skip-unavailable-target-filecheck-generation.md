# Skip unavailable target FileCheck generation

_created 2026-08-29_

`tools/update_filecheck.py` does not generate host-profile checks for fixtures
directly under `tests/fixtures/bionic`, `macos`, or `msvc`. Those fixtures need
an explicit `--target PREFIX=TRIPLE` mapping so their generated checks describe
the intended target ABI.

Target-specific generated blocks are kept only when the corresponding target
differential path passes. Unsupported MSVC generation and failing Bionic/macOS
fixtures remain unasserted instead of being covered by host output or weakened
checks.
