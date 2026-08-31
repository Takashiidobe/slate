# FreeBSD libc oracle

Slate uses FreeBSD 15.1-RELEASE as the initial libc and ABI oracle. It is the
current production release and provides official amd64 and arm64 base-system
distributions. The corresponding Rust targets are `x86_64-unknown-freebsd` and
`aarch64-unknown-freebsd`; Clang probes use release-qualified targets so the
major and minor FreeBSD version remain explicit.

Materialize the two release sysroots under `target/` with:

```bash
tools/bootstrap-freebsd-oracle.sh
```

The bootstrap downloads the official amd64 and arm64 `base.txz` archives,
checks their byte sizes and SHA-256 hashes, and extracts `/usr/include`,
`/usr/lib`, and `/lib`. This provides release-built public headers, startup and
linker inputs, and the architecture-specific `libc.so.7` without unpacking an
entire FreeBSD installation. No license-acceptance flag is required.

The release distributions are the ABI authority. The matching implementation
reference is the FreeBSD source tag `release/15.1.0`, peeled to commit
`96841ea08dcfa84b954a32dc5ae1a26c28966cf4`. The bootstrap records the complete
contract in `target/freebsd-oracle/oracle.json` and keeps downloaded and
derived data ignored under `target/`.

Existing sysroots for the same release may be used without copying them:

```bash
SLATE_FREEBSD_AMD64_SYSROOT=/path/to/amd64/sysroot \
SLATE_FREEBSD_ARM64_SYSROOT=/path/to/arm64/sysroot \
  tools/bootstrap-freebsd-oracle.sh
```

Both variables are required together. External sysroots must contain the
15.1-RELEASE `sys/param.h` version and a `lib/libc.so.7` for the expected ELF
machine.

## Header comparison

The bootstrap runs `tools/compare-freebsd-libc.sh`. It inventories both
release header trees, maps every current `libc-shim` header to its amd64 and
arm64 counterpart, and writes raw same-path diffs under
`target/freebsd-oracle/comparison`. Start with `header-map.tsv` and
`summary.txt`. A same-path match only identifies declarations to investigate;
it does not prove that FreeBSD shares another platform's ABI.

## Probes

Run one probe by architecture and mode:

```bash
tools/probe-freebsd-libc.sh --arch aarch64 --mode layouts
tools/probe-freebsd-libc.sh --arch x86_64 --mode symbols
```

Available modes are `predefined`, `header-macros`, `preprocess`, `ast`,
`layouts`, `assembly`, `availability`, and `symbols`. Results are written under
`target/freebsd-oracle/probes/<architecture>/15.1-RELEASE`. The symbol mode
records both the representative object's undefined imports and the selected
architecture's `libc.so.7` dynamic symbols. Probes require Clang,
`llvm-readobj`, and `llvm-readelf`; their paths can be overridden with
`SLATE_FREEBSD_CLANG`, `SLATE_FREEBSD_LLVM_READOBJ`, and
`SLATE_FREEBSD_LLVM_READELF`.

Run the complete bootstrap and probe smoke test with:

```bash
tools/test-freebsd-oracle.sh
```

This oracle does not yet select a FreeBSD `libc-shim` profile, translate a
FreeBSD fixture, link generated Rust, or execute FreeBSD binaries. Those are
separate implementation steps. Updating the baseline requires changing the
release, source commit, both archive identities, checksums, targets, tests, and
documentation together.

## Target selection

`SLATE_TARGET=x86_64-unknown-freebsd` or `aarch64-unknown-freebsd` select the
FreeBSD kernel/libc profile in `src/frontend/toolchain.rs`: `__SLATE_KERNEL_FREEBSD`,
`__SLATE_LIBC_FREEBSD`, `__SLATE_PLATFORM_FREEBSD`, `__SLATE_OBJ_ELF`, LP64,
little endian. Rust's FreeBSD triples carry no release information, so the
release baseline is a separate input: `SLATE_FREEBSD_RELEASE`, defaulting to
`15.1` (the pinned oracle release above). Any other value is diagnosed as
unsupported rather than silently accepted, since only 15.1's layouts are
modeled. The resolved release also selects the Clang target
(`x86_64-unknown-freebsd15.1` / `aarch64-unknown-freebsd15.1`, matching
`oracle.json`'s `clang_target` fields) and defines
`__SLATE_FREEBSD_VERSION__` using FreeBSD's own `__FreeBSD_version`
MMmmmXXX encoding (`1501000` for 15.1-RELEASE).

`libc-shim/freebsd-basic-headers.txt` is the initial supported-header
manifest: the ISO C headers only (assert/ctype/errno/float/limits/math/
stddef/stdint/stdio/stdlib/string/time and friends). `bits/types.h` and
`features.h` gain a `__SLATE_LIBC_FREEBSD` branch for the fundamental
integer/pointer-width types, sourced from the real 15.1-RELEASE
`sys/_types.h`/`x86/_types.h`/`arm64/_types.h` in the bootstrapped sysroots
under `target/freebsd-oracle/sysroots/`, not guessed. Notable divergences
from the generic LP64 branch: `mode_t` is `uint16_t` (not `uint32_t`),
`st_blksize`'s `__blksize_t` is `int32_t` even though `off_t`/`blkcnt_t` are
64-bit, and `wctrans_t` is a plain `int` rather than a pointer type. POSIX
headers beyond the ISO set (`dirent.h`, `pthread.h`, `signal.h`,
`sys/socket.h`, `sys/stat.h`, `unistd.h`, ...) are deliberately excluded from
this manifest until their layouts are checked against the oracle in later
tickets (`slate-sfzn.10.4`–`.10.13`).
