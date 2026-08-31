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

## stdio, locale, and text conversion (`slate-sfzn.10.3`)

`libc-shim/freebsd-stdio-locale-headers.txt` extends the manifest with
stdio.h, locale.h, xlocale.h, wchar.h, wctype.h, uchar.h, and langinfo.h.

`struct __sFILE`, defined in `libc-shim/bits/freebsd/stdio.h`, is the real
15.1-RELEASE layout (312 bytes, 8-byte aligned), verified field-for-field
against the oracle sysroot with
`clang -Xclang -fdump-record-layouts -c -o /dev/null`. `fpos_t` is a direct
`__off_t` alias (no wrapper struct, unlike Darwin's `__fpos_t`). `stdin` /
`stdout` / `stderr` route through `__stdinp` / `__stdoutp` / `__stderrp`,
same as Darwin. `__mbstate_t` (`bits/types.h`) is a union of a 128-byte
buffer and a `long long`, embedded inline in `__sFILE` at offset 176 — a
plain byte array would have the wrong alignment, since the real type
requires 8-byte alignment for its `long long` member. `_fl_mutex` /
`_fl_owner` are modeled as `void *` rather than the real
`struct pthread_mutex *` / `struct pthread *`, since only pointer
size/alignment matters for this layout and the pointee types aren't
otherwise modeled.

`locale.h`: `LC_*` category numbering (`LC_ALL=0` .. `LC_MESSAGES=6`) and
`lconv`'s `int_p_cs_precedes`/`int_n_cs_precedes` field order match
Darwin's. `LC_*_MASK` bit assignment does not — FreeBSD orders
`COLLATE,CTYPE,MONETARY,NUMERIC,TIME,MESSAGES`, Darwin orders
`COLLATE,CTYPE,MESSAGES,MONETARY,NUMERIC,TIME` — so FreeBSD has its own
mask block rather than reusing Darwin's. `freelocale()` returns `void` on
FreeBSD (`int` on Darwin), so FreeBSD stays on the shared default branch.

`langinfo.h`: `nl_item` numbering (`CODESET=0` .. `D_MD_ORDER=57`) is
numerically identical to Darwin's, but Darwin gates `YESSTR`/`NOSTR`/
`D_MD_ORDER` behind `__DARWIN_C_LEVEL`; FreeBSD defines them
unconditionally, so it has its own block instead of reusing Darwin's.

`xlocale.h`: FreeBSD gets only `localeconv_l`, matching its real surface.
Darwin's `LC_C_LOCALE`/`_c_locale` globals and the wider `_l`-suffixed
function family (`strtol_l` etc.) have no FreeBSD equivalent in this header
and are out of scope here.

`wchar.h`/`wctype.h` gained `__SLATE_LIBC_FREEBSD` in the existing
POSIX/Bionic/Darwin visibility conditions for `locale_t` and `_l`-suffixed
wide-char functions — visibility only, no ABI divergence.

Verified end-to-end via `slate translate` + `cargo check --target
{x86_64,aarch64}-unknown-freebsd` against the generated Rust using the real
`libc` crate, for a stdio-using fixture, on both architectures.

## Filesystem and directory ABI (`slate-sfzn.10.4`)

`libc-shim/freebsd-filesystem-headers.txt` extends the manifest with
`sys/types.h`, `sys/stat.h`, `sys/statvfs.h`, `dirent.h`, `fcntl.h`, and
`unistd.h`.

`struct stat` (`libc-shim/bits/freebsd/stat.h`) is FreeBSD 15.1's modern
(non-`freebsd11`) layout — 224 bytes, oracle-verified field offset by
field offset via `offsetof` static asserts under the real sysroot. It is
**not** POSIX-generic: field order is
`st_dev, st_ino, st_nlink, st_mode, st_bsdflags, st_uid, st_gid,
st_padding1, st_rdev, st_atim, st_mtim, st_ctim, st_birthtim, st_size,
st_blocks, st_blksize, st_flags, st_gen, st_filerev, st_spare[9]`, with a
16-bit `st_bsdflags` immediately after `st_mode` and a trailing
`st_gen`/`st_filerev`/`st_spare` reservation block that POSIX/glibc structs
don't have. `st_flags`/`st_gen` (the BSD file-flags and generation-number
fields with no glibc equivalent) and `st_birthtim` (file creation time,
exposed as `st_birthtime`/`st_birthtimensec` macros) are present, matching
the ticket's acceptance criteria. `st_blksize` stays `int32_t` per the
divergence already noted in 10.2/10.3.

`struct dirent` (`libc-shim/bits/freebsd/dirent.h`) is 280 bytes:
`d_fileno` (not `d_ino` — FreeBSD's field is already named `d_fileno`, so
the shared `#define d_fileno d_ino` compat macro is suppressed for
FreeBSD), `d_off`, `d_reclen`, `d_type`, `d_pad0`, `d_namlen`, `d_pad1`,
`d_name[256]`. `DT_*` file-type constants are numerically identical to the
existing shared block, so no override was needed there. FreeBSD's
`getdents(2)` has a different signature than Linux's
(`ssize_t getdents(int, char *, size_t)` vs. `int getdents(int, struct
dirent *, size_t)`), so it gets its own declaration rather than reusing
the glibc one.

`fcntl.h` (`libc-shim/bits/freebsd/fcntl.h`): `O_*`, `F_*`, and `AT_*`
values are all BSD-style bit assignments, distinct from both the
glibc-style octal values and Darwin's own BSD-style values (e.g.
`AT_FDCWD` is `-100` on FreeBSD vs. `-2` on Darwin; `O_CLOEXEC` is
`0x00100000` on FreeBSD vs. `0x01000000` on Darwin; `F_DUPFD_CLOEXEC` is
`17` on FreeBSD vs. `67` on Darwin; `F_GETLK`/`F_SETLK`/`F_SETLKW` are
`11`/`12`/`13` on FreeBSD vs. `7`/`8`/`9` on Darwin). All existing
glibc-specific octal `O_*`/`AT_*`/`F_*` blocks in the shared `fcntl.h` were
additionally gated off for FreeBSD (they were previously gated off only
for Darwin).

`struct statvfs` (`libc-shim/bits/freebsd/statvfs.h`) uses FreeBSD's real
field order (`f_bavail, f_bfree, f_blocks, f_favail, f_ffree, f_files,
f_bsize, f_flag, f_frsize, f_fsid, f_namemax`), which differs from both
the generic branch's and Darwin's `f_bsize`/`f_frsize`-first ordering.
`ST_RDONLY`/`ST_NOSUID` values match Darwin's, so FreeBSD reuses that
branch.

`unistd.h` filesystem calls (`link`, `unlink`, `symlink`, `readlink`,
`truncate`, `chown`, `access`, ...) needed no changes: their prototypes
were already libc-agnostic. `_PC_*` `pathconf` constants are glibc-numbered
with no per-libc gating for any libc, Darwin included — that's a
pre-existing gap, deliberately left alone here and deferred to the
feature-selection/symbol-versioning ticket (`slate-sfzn.10.10`).

Verified end-to-end via `slate translate` + `cargo check --target
{x86_64,aarch64}-unknown-freebsd` against the generated Rust using the real
`libc` crate, for a fixture exercising `open`/`fstat`/`opendir`/`readdir`/
`statvfs`, on both architectures.
