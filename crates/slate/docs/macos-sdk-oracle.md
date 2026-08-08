# macOS SDK and open-source oracle

Slate's AArch64 macOS support starts at macOS 11.0. The Rust target remains
`aarch64-apple-darwin`; SDK probes use Clang's deployment-qualified
`arm64-apple-macos11.0` target so API availability does not silently follow the
host's deployment setting.

The installed macOS SDK is the ABI authority. On macOS it is discovered with
`xcrun --sdk macosx --show-sdk-path`. On other hosts, set `SLATE_MACOS_SDK` to
an existing `MacOSX.sdk` directory. The repository does not acquire, copy, or
commit an Apple SDK.

Apple's public source releases are a supporting declaration and implementation
reference. Download the Big Sur 11.0.1 distribution manifest and its pinned
Libc and XNU revisions under `target/` with:

```bash
tools/bootstrap-macos-oracle.sh
```

The bootstrap uses the official `apple-oss-distributions` repositories and
pins the release tag, project tags, and peeled commits in the script. It writes
the resolved contract to `target/macos-oracle/oracle.json`. The downloaded
source is not a substitute SDK: generated headers, SDK-only declarations,
availability metadata, module overlays, and some public LibSystem interfaces
are absent from the open-source trees.

`tools/compare-macos-libc.sh` can be rerun independently. It inventories the
Apple Libc and XNU BSD headers, maps them against `libc-shim/include`, and puts
raw same-path diffs under `target/macos-oracle/comparison/diffs`. With
`SLATE_MACOS_SDK` set, it adds installed-SDK comparisons. Start with
`header-map.tsv` to distinguish declarations owned by Libc, XNU, or the SDK.

The initial source comparison identifies these Darwin-specific overlays:

- AArch64 macOS is LP64, but `long double` is 64-bit binary64 rather than the
  Linux AArch64 representation.
- `clock_t` is unsigned long, `blksize_t` is signed 32-bit, `mode_t` is
  unsigned 16-bit, `sigset_t` is unsigned 32-bit, and `mbstate_t` is a
  128-byte union aligned as `long long`.
- `fpos_t` is the signed 64-bit Darwin offset type. `FILE` is the public
  `struct __sFILE` layout rather than Slate's current opaque `struct FILE`.
- `struct stat` includes birth time, flags, generation, and reserved fields in
  a Darwin-specific order. `struct dirent` has `d_seekoff`, a 16-bit
  `d_namlen`, and a 1024-byte name buffer for the 64-bit inode profile.
- Darwin socket addresses begin with an 8-bit length and an 8-bit family;
  Slate's current Linux layout begins with a 16-bit family.
- public declarations use Darwin alias families such as inode-64, UNIX 2003,
  and cancellation variants, so source names cannot be assumed to equal
  Mach-O symbol names.

The source-only comparison scopes likely work but cannot prove ABI identity.
Macro sets, preprocessed declarations, JSON AST, record layouts, availability,
assembler labels, and Mach-O symbols must be captured from the installed SDK
before the corresponding shim declarations are treated as correct.
