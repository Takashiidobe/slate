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

## Time and signal ABI (`slate-sfzn.10.5`)

`libc-shim/freebsd-time-signal-headers.txt` extends the manifest with
`time.h`, `sys/time.h`, `signal.h`, and `sys/signal.h`.

`CLOCKS_PER_SEC` is `128` on FreeBSD, not `1000000` like glibc/Darwin — a
genuine surprise caught from the oracle header rather than assumed.
`libc-shim/bits/freebsd/time.h` models the full real clock-ID set
(`CLOCK_REALTIME=0`, `CLOCK_VIRTUAL=1`, `CLOCK_PROF=2`, `CLOCK_MONOTONIC=4`,
`CLOCK_UPTIME=5`, `CLOCK_UPTIME_PRECISE=7`, `CLOCK_UPTIME_FAST=8`,
`CLOCK_REALTIME_PRECISE=9`, `CLOCK_REALTIME_FAST=10`,
`CLOCK_MONOTONIC_PRECISE=11`, `CLOCK_MONOTONIC_FAST=12`,
`CLOCK_SECOND=13`, `CLOCK_THREAD_CPUTIME_ID=14`,
`CLOCK_PROCESS_CPUTIME_ID=15`, `CLOCK_TAI=16`), including the FreeBSD-only
IDs named in the ticket (`CLOCK_UPTIME`, `CLOCK_UPTIME_PRECISE`,
`CLOCK_UPTIME_FAST`, `CLOCK_SECOND`) plus the Linux-compat aliases
(`CLOCK_BOOTTIME`, `CLOCK_REALTIME_COARSE`, `CLOCK_MONOTONIC_COARSE`).
`struct tm` matches Darwin's shape (`long tm_gmtoff`, `char *tm_zone`
fields, not glibc's `__tm_gmtoff`/`__tm_zone` naming convention). `time.h`
and `sys/time.h` are both self-contained overlays (matching the existing
Darwin precedent) rather than gated branches inside the shared headers,
since FreeBSD's visible surface differs enough from glibc's to make
per-line gating unreadable.

`sigset_t` (`libc-shim/bits/types.h`) is FreeBSD's real
`struct { unsigned int __bits[4]; }` — 16 bytes, 4 32-bit words for 128
signal numbers — genuinely distinct from both glibc's 128-byte/1024-bit
mask and Darwin's plain 4-byte `unsigned int` mask. This was the previous
gap: FreeBSD fell through to the generic glibc-shaped `__sigset_t` branch
before this ticket.

Signal numbers 1–33 match Darwin's assignment exactly (`SIGHUP=1` ..
`SIGUSR2=31`), but FreeBSD additionally defines `SIGTHR=32`/`SIGLIBRT=33`
and a realtime range `SIGRTMIN=65`/`SIGRTMAX=126` that Darwin has neither
of. `struct sigaction` is 32 bytes: the handler union at offset 0,
`sa_flags` at offset 8, then `sigset_t sa_mask` at offset 12 — note the
field order is `sa_flags` *before* `sa_mask`, the reverse of Darwin's
`sa_mask`-before-`sa_flags` layout, both verified via `offsetof` against
the oracle. `siginfo_t` is 80 bytes: `si_signo`, `si_errno`, `si_code`,
`si_pid`, `si_uid`, `si_status`, `si_addr`, `si_value` (union sigval, 8
bytes) at offsets 0/4/8/12/16/20/24/32, followed by a 40-byte reserved
`_reason` union modeled as `long __spare1__; int __spare2__[7];` to match
the real trailing size without modeling every one of FreeBSD's
signal-specific sub-unions (`_fault`/`_timer`/`_mesgq`/`_poll`/`_capsicum`)
that the ticket's scope doesn't require. `ILL_*` code numbering diverges
from Darwin's own reordering of the same names (FreeBSD:
`ILL_ILLOPC=1,ILL_ILLOPN=2,ILL_ILLADR=3,ILL_ILLTRP=4,...`; Darwin permutes
`ILL_ILLTRP`/`ILL_PRVOPC` earlier) — caught by diffing against the oracle
rather than reusing Darwin's block. `FPE_*`, `SEGV_*`, `BUS_*`, `TRAP_*`,
`CLD_*`, `POLL_*`, and `SI_*` code numbering all matched Darwin's values
exactly. `MINSIGSTKSZ` is architecture-dependent (`512*4=2048` on x86_64,
`1024*4=4096` on aarch64, both read from the real per-arch
`machine/_limits.h`), unlike Darwin's single fixed `32768`.

`stack_t`/`sigaltstack` field layout (`void *ss_sp; size_t ss_size; int
ss_flags;`) matches Darwin's shape exactly, so no divergence there beyond
the `SS_ONSTACK=0x0001`/`SS_DISABLE=0x0004` values (also identical to
Darwin's).

Verified end-to-end via `slate translate` + `cargo check --target
{x86_64,aarch64}-unknown-freebsd` against the generated Rust using the real
`libc` crate, for a fixture exercising `clock_gettime`, `gettimeofday`,
`sigaction`/`sigemptyset`/`raise` with a `SA_SIGINFO` handler, on both
architectures — clean with zero warnings.

## pthread (libthr) and synchronization ABI (`slate-sfzn.10.6`)

`libc-shim/freebsd-pthread-headers.txt` extends the manifest with
`pthread.h`, `semaphore.h`, and `sched.h`.

The headline finding: unlike glibc/NPTL (fixed-size opaque byte arrays,
e.g. 40-byte `pthread_mutex_t`) and Darwin/libpthread (fixed-size opaque
structs with a `long __sig` signature field, e.g. 56-byte
`pthread_mutex_t`), **every FreeBSD libthr pthread type except
`pthread_once_t` and `pthread_key_t` is a plain pointer to an incomplete
struct** (`typedef struct pthread_mutex *pthread_mutex_t;`, confirmed for
`pthread_t`/`pthread_attr_t`/`pthread_mutex_t`/`pthread_mutexattr_t`/
`pthread_cond_t`/`pthread_condattr_t`/`pthread_rwlock_t`/
`pthread_rwlockattr_t`/`pthread_barrier_t`/`pthread_barrierattr_t`/
`pthread_spinlock_t` — all 8 bytes on LP64, oracle-verified for both
architectures). The kernel allocates and owns the pointee; user code never
sees its layout. This makes the storage-size/alignment problem the ticket
raised almost moot for FreeBSD specifically — the design note's "opaque
storage as ABI" concern is real for glibc and Darwin but resolves to "it's
a pointer" here. `pthread_key_t` is a plain `int`. `pthread_once_t` is the
one real aggregate: `struct { int state; pthread_mutex_t mutex; }`, 16
bytes (4-byte `state` padded to 8, then an 8-byte pointer), oracle-verified
via `sizeof`.

Because every lock type is a pointer, the static initializer macros are
correspondingly simple: `PTHREAD_MUTEX_INITIALIZER`/
`PTHREAD_COND_INITIALIZER`/`PTHREAD_RWLOCK_INITIALIZER` are all just
`NULL` (the kernel lazily allocates real storage on first use), and
`PTHREAD_ONCE_INIT` is `{PTHREAD_NEEDS_INIT, NULL}` i.e. `{0, NULL}`.
`PTHREAD_ADAPTIVE_MUTEX_INITIALIZER_NP` is the sentinel `(pthread_mutex_t)1`
(distinguishing "not yet allocated" from "use the adaptive variant").

Mutex-type enum values start at 1, not 0:
`PTHREAD_MUTEX_ERRORCHECK=1, PTHREAD_MUTEX_RECURSIVE=2,
PTHREAD_MUTEX_NORMAL=3, PTHREAD_MUTEX_ADAPTIVE_NP=4`, with
`PTHREAD_MUTEX_DEFAULT` aliasing `PTHREAD_MUTEX_ERRORCHECK` (not
`PTHREAD_MUTEX_NORMAL` like Darwin's `PTHREAD_MUTEX_DEFAULT`). Robust-mutex
support (`PTHREAD_MUTEX_STALLED=0`/`PTHREAD_MUTEX_ROBUST=1`,
`pthread_mutex_consistent`, `pthread_mutexattr_get/setrobust`) is present
and matches the shared shim's existing values. `PTHREAD_STACK_MIN` is
architecture-dependent — `2048` on x86_64, `4096` on aarch64 — reusing the
same per-arch `machine/_limits.h` `__MINSIGSTKSZ` derivation established
for `MINSIGSTKSZ` in 10.5, not Darwin's fixed value.

`sem_t` (`libc-shim/bits/freebsd/semaphore.h`) is a real 16-byte value
type (not a pointer, and not glibc's 32-byte struct or Darwin's 4-byte
`int`): `struct { uint32_t _magic; struct { volatile uint32_t _count;
uint32_t _flags; } _kern; uint32_t _padding; }`, oracle-verified via
`sizeof`. `SEM_VALUE_MAX` is `INT_MAX` (`0x7fffffff`), unlike Darwin's
`32767`.

`sched.h`: FreeBSD's real `sched.h` is nearly empty (just CPU-affinity
declarations) and pulls `SCHED_*`/`struct sched_param` from
`sys/sched.h`. Scheduling-policy numbering starts at 1, distinct from
both Linux's 0-based numbering and Darwin's own values:
`SCHED_FIFO=1, SCHED_OTHER=2, SCHED_RR=3`. `struct sched_param` is just
`{ int sched_priority; }` — 4 bytes, no reserved padding fields like the
shared shim's glibc-shaped struct.

Verified end-to-end via `slate translate` + `cargo check --target
{x86_64,aarch64}-unknown-freebsd` against the generated Rust using the real
`libc` crate, for a fixture exercising `pthread_create`/`pthread_join`,
`pthread_mutex_lock`/`unlock` on a `PTHREAD_MUTEX_INITIALIZER` mutex,
`pthread_once`, and `sem_init`/`sem_wait`/`sem_post`/`sem_destroy`, on both
architectures — compiles clean modulo the same benign opaque-struct
`improper_ctypes` warnings already present for Darwin's `__dirstream`.

## Sockets and network ABI (`slate-sfzn.10.7`)

`libc-shim/freebsd-net-headers.txt` covers `sys/socket.h`, `netinet/in.h`,
`sys/un.h`, `netdb.h`, `arpa/inet.h`, `ifaddrs.h`. Unlike pthread/sem/sched
(10.6), which had no Darwin overlay precedent to draw structure from,
sockets also had none — the shared shim's `sys/socket.h` etc. were purely
Linux-shaped with no libc branching at all, so FreeBSD needed the same
"full self-contained overlay" pattern used for Darwin's own divergent
subsystems, gated by a new `#if defined(__SLATE_LIBC_FREEBSD)` branch ahead
of the existing Linux-shaped `#else`.

The headline finding, confirmed by the ticket's own design note: **every
FreeBSD sockaddr variant carries a leading `unsigned char sa_len` byte**
before `sa_family_t sa_family` — the classic BSD length-prefixed sockaddr
convention glibc dropped decades ago. This shifts every subsequent field's
offset relative to glibc, and the specific padding differs from Darwin's
own sockaddr layout too (independently oracle-verified, not assumed
identical): `struct sockaddr` is `{sa_len:1, sa_family:1, sa_data:14}` = 16
bytes; `sockaddr_in` is `{sin_len:1, sin_family:1, sin_port:2, sin_addr:4,
sin_zero:8}` = 16 bytes with `sin_family` at offset 1; `sockaddr_in6` is
`{sin6_len:1, sin6_family:1, sin6_port:2, sin6_flowinfo:4, sin6_addr:16,
sin6_scope_id:4}` = 28 bytes with `sin6_addr` at offset 8; `sockaddr_un` is
`{sun_len:1, sun_family:1, sun_path:SUNPATHLEN}` with `SUNPATHLEN=104`
(not glibc's 108), total 106 bytes, `sun_path` at offset 2;
`sockaddr_storage` follows the same `_SS_PAD1SIZE`/`_SS_PAD2SIZE` algebra
as Darwin/glibc (128 bytes total) but starts with `ss_len`/`ss_family`
instead of just `ss_family`. All sizes/offsets were verified with the
`_Static_assert(... == 0xDEAD0000 + __LINE__, ...)` sentinel trick against
both oracle sysroots, not derived by inspection alone.

`AF_INET6` is a three-way divergence across every libc profile this repo
supports: **28** on FreeBSD, `10` on Linux, `30` on Darwin — none of the
three values may be reused for another. `SOL_SOCKET` is `0xffff` on
FreeBSD, not `1` like Linux/Darwin — a option-level constant with the same
kind of "looks portable, isn't" trap as `AF_INET6`. `SO_*` option values
also live on a different numbering scheme: boolean flags are low bitmask
values (`SO_REUSEADDR=0x4`) but `SO_SNDBUF`/`SO_RCVBUF`/`SO_ERROR`/etc. are
non-contiguous values starting at `0x1001`, unlike Linux's flat small
integers.

`struct msghdr`/`struct cmsghdr`/`struct linger` are structurally
identical to the generic shim (no BSD-specific padding quirks at LP64), so
only the CMSG macros needed reimplementing against FreeBSD's real
`_ALIGN`-based alignment (word-aligned to `sizeof(long)`, matching the
oracle's `sys/socket.h` macros verbatim) rather than glibc's
`size_t`-based `CMSG_ALIGN`.

`netdb.h`'s `struct addrinfo` has a **different field order** than the
shared shim's Linux-shaped version: FreeBSD puts `ai_canonname` before
`ai_addr` (`{flags, family, socktype, protocol, addrlen, canonname, addr,
next}`), not after — oracle-verified via `offsetof`, since silently
copying the Linux field order would have produced an ABI-incompatible
struct despite identical member types. `EAI_*` codes are **positive** on
FreeBSD (`EAI_FAMILY=5`), not negative like the shared shim's glibc-shaped
values. `NI_MAXHOST=1025` (not 255). `hostent`/`servent`/`protoent`/
`netent` have identical layouts to the shared shim, so only their
constants needed FreeBSD values, not new struct definitions.

`ifaddrs.h`: FreeBSD's `struct ifaddrs` has a **separate `ifa_dstaddr`
field**, not a union of `ifa_broadaddr`/`ifa_dstaddr` like the shared
shim's Linux-shaped version — `ifa_broadaddr` is a `#define` alias for the
same field instead. `arpa/inet.h` needed no FreeBSD overlay at all: its
function signatures are identical and it only depends on types
(`in_addr_t`, `socklen_t`) that already dispatch correctly through
`netinet/in.h`'s and `bits/types.h`'s existing FreeBSD branches.

Kept deliberately narrow per the ticket's design note ("exclude
routing-socket internals and kernel networking"): only the commonly-used
subset of `SO_*`/`IP_*`/`IPV6_*`/`PF_*`/`AF_*` constants is defined (core
socket lifecycle, `AF_UNIX`/`AF_INET`/`AF_INET6`/`AF_ROUTE`/`AF_LINK`,
`IPPROTO_{IP,ICMP,IGMP,TCP,UDP,IPV6,ICMPV6,RAW}`, basic multicast/option
constants) rather than exhaustively porting FreeBSD's full option surface.

Verified end-to-end via `slate translate` + `cargo check --target
{x86_64,aarch64}-unknown-freebsd` against the generated Rust using the real
`libc` crate, for a fixture that creates a TCP socket, sets `SO_REUSEADDR`,
binds/listens on loopback, reads back the bound address via
`getsockname`/`sockaddr_storage`, builds a `sockaddr_un` and an
`addrinfo` hints struct, and calls `getifaddrs`/`freeifaddrs` — both
architectures check clean with zero warnings.

## Process, terminal, sysctl, and kqueue ABI (`slate-sfzn.10.8`)

`libc-shim/freebsd-process-headers.txt` covers `unistd.h`, `spawn.h`,
`sys/wait.h`, `poll.h`, `sys/select.h`, `termios.h`, `pwd.h`, `grp.h`,
`dlfcn.h`, `regex.h`, `glob.h`, `fnmatch.h`, plus two brand-new
FreeBSD-only headers: `sys/sysctl.h` and `sys/event.h` (kqueue/kevent).
`sys/select.h` and `grp.h` needed **no overlay at all** — `fd_set`
(1024-bit, `unsigned long` word array) and `struct group` are
byte-for-byte identical to the shared shim's Linux-shaped versions,
oracle-confirmed rather than assumed.

`struct termios` is a from-scratch overlay: FreeBSD's flag bit
assignments (`IGNBRK=0x1`, `ECHO=0x8`, `ISIG=0x80`, etc.) share no
numbering scheme with glibc's octal-style bits, `NCCS=20` not 32, and the
struct itself carries `c_ispeed`/`c_ospeed` as plain trailing `speed_t`
fields (44 bytes total, `c_cc` at offset 16, `c_ispeed` at offset 36) —
oracle-verified via the `_Static_assert` sentinel trick, not the
`__c_ispeed`/`__c_ospeed`-behind-accessor-functions style glibc uses. The
`V*` control-character indices, baud rate constants, and
`TCSANOW`/`TCIFLUSH`/`TCOOFF`-family constants all use FreeBSD-specific
numbering distinct from glibc's.

`struct passwd` gained three FreeBSD-only fields absent from the shared
shim's POSIX-minimal version — `pw_change`, `pw_class`, `pw_expire`,
`pw_fields` — interleaved with the common fields in a different order
(80 bytes total, `pw_change` at offset 24, `pw_gecos` at offset 40,
oracle-verified). `posix_spawnattr_t`/`posix_spawn_file_actions_t`
follow the same **opaque-pointer-to-incomplete-struct** pattern
discovered for pthread types in 10.6 (`typedef struct __posix_spawnattr
*posix_spawnattr_t;`, 8 bytes) rather than the shared shim's fixed-size
inline struct — the kernel owns the storage. `POSIX_SPAWN_*` flag values
use a different bit assignment than the shared shim
(`SETSIGDEF=0x10`/`SETSIGMASK=0x20` vs. the shim's `4`/`8`), and
`POSIX_SPAWN_DISABLE_ASLR_NP` replaces the shim's
`USEVFORK`/`SETSID` (neither exists on FreeBSD).

`sys/wait.h`'s status-decoding macros use a completely different
algorithm than glibc's bit-shift scheme — FreeBSD's `_WSTATUS`/`_W_INT`
approach with `_WSTOPPED=0177` and a magic `0x13` (`SIGCONT`) sentinel
for `WIFCONTINUED`, ported verbatim from the oracle rather than
reimplemented. `idtype_t` is a 16-member enum in Solaris-inherited order
(`P_PID=0, P_PPID=1, ..., P_ALL=7, ...`), unrelated to the shared shim's
4-member `P_ALL=0`-first enum. `WCONTINUED=4` (not 8).

`poll.h`: `nfds_t` is `unsigned int` on FreeBSD, not the shared shim's
`unsigned long` — a real width divergence, not just a value one.
`POLLWRNORM` aliases `POLLOUT` (`0x0004`) rather than having its own bit.

`dlfcn.h`: `RTLD_DEFAULT` is `(void *)-2` and `RTLD_SELF` is
`(void *)-3` (both absent/different from the shared shim's NULL-based
`RTLD_DEFAULT`), `RTLD_NOLOAD=0x02000` (not `4`), and `RTLD_TRACE`/
`RTLD_DEEPBIND` are FreeBSD-specific additions.

`regex.h`: `regex_t` is a real 4-field struct (`re_magic`, `re_nsub`,
`re_endp`, `re_g`; 32 bytes) rather than the shared shim's synthetic
padded struct, and `REG_*` compile-flag bit assignments are swapped
relative to the shim (`REG_NOSUB=0004`/`REG_NEWLINE=0010` vs. the shim's
`8`/`4`). The glibc-only `_GNU_SOURCE` `re_*`/`RE_*` extension API has no
FreeBSD equivalent and is correctly absent from the FreeBSD branch.

`glob.h`: `glob_t` is structurally very different — FreeBSD's version
carries `gl_matchc`, an error-callback function pointer, and a full set
of pluggable `gl_opendir`/`gl_readdir`/`gl_closedir`/`gl_stat`/`gl_lstat`
function pointers absent from the shared shim's minimal struct.
`GLOB_*` flag bit assignments are entirely different
(`GLOB_APPEND=0x0001` vs. the shim's `0x20`), and error return codes are
negative (`GLOB_NOSPACE=-1`) rather than the shim's positive `1`.

`fnmatch.h`: `FNM_PATHNAME` and `FNM_NOESCAPE` are **swapped** relative
to the shared shim's values (`0x02`/`0x01` vs. the shim's `0x01`/`0x02`)
— easy to miss since both flags exist in both profiles with plausible-
looking values, caught only by checking against the oracle directly.

`unistd.h`'s `_PC_*`/`_SC_*` sysconf/pathconf identifiers are a full
renumbering relative to glibc (`_SC_ARG_MAX=1` not `0`, `_PC_LINK_MAX=1`
not `0`, etc.) — these are real runtime-ABI values since a translated
program calls the real `sysconf`/`fpathconf` on the target. Kept
deliberately narrow (~120 of FreeBSD's ~125 `_SC_*` names covered; the
`_CS_*` confstr identifiers are omitted entirely under the FreeBSD
branch rather than guessed) per the epic's "keep narrow" precedent from
10.7. Omitted names simply don't compile if used, which is safer than a
silently wrong value.

`sys/sysctl.h` and `sys/event.h` are new, FreeBSD-only public headers —
first time this shim has had headers with no cross-libc existence at
all. Both use `#if defined(__SLATE_LIBC_FREEBSD) ... #endif` with an
**empty body** (not an `#error`) on other profiles: an unconditional
`#error` as the file's literal first line is how this codebase already
marks headers as wholesale-unsupported-everywhere (see
`libc-shim/include/sys/pidfd.h`), and `tests/libc_shim_suite.rs`'s
`discover_public_headers` blindly text-matches that pattern to exclude
such stub files from the Linux/glibc "compile every public header"
sweep. A conditional `#error` guarded by `__SLATE_LIBC_FREEBSD` doesn't
match that text-prefix check, so it would have broken the Linux sweep
outright (confirmed by an initial failed nextest run) rather than being
silently skipped — hence the empty-body-elsewhere fix instead of a
hard error.

`sys/sysctl.h` covers `sysctl`/`sysctlbyname`/`sysctlnametomib` plus a
narrow `CTL_*`/`KERN_*`/`HW_*` MIB-name subset (`CTL_KERN=1`, `HW_NCPU=3`,
etc.) rather than FreeBSD's full MIB tree. `sys/event.h` covers
`kqueue`/`kevent`/`EV_SET` and `struct kevent`
(`{ident: uintptr_t, filter: short, flags: u16, fflags: u32, data: i64,
udata: *void, ext: [u64; 4]}`, 64 bytes, oracle-verified field offsets)
plus the `EVFILT_*`/`EV_*` constant sets — the design note explicitly
calls kqueue "FreeBSD-idiomatic ... represent as first-class FreeBSD-only
surface rather than trying to unify with a Linux shape" (epoll is not a
substitute), so no attempt was made to give it a cross-libc shape.

Verified end-to-end via `slate translate` + `cargo check --target
{x86_64,aarch64}-unknown-freebsd` for a fixture doing
`posix_spawn`/`waitpid`/`WIFEXITED`, `tcgetattr`/termios flag
manipulation, `getpwuid`, `sysctl(CTL_HW, HW_NCPU)`, `kqueue`/`EV_SET`,
and `fnmatch` — both architectures check clean modulo the same benign
opaque-struct `improper_ctypes` warnings already accepted for the
`posix_spawnattr_t`/`posix_spawn_file_actions_t` opaque pointers. While
building this fixture, hit and worked around an unrelated pre-existing
lowerer bug (filed as `slate-vd43`, fixed separately): on aarch64, `char *`
**struct fields** lower to `*mut i8` regardless of target, while `char *`
**local variables** correctly lower to `*mut u8` per AAPCS64's
unsigned-default — a target-awareness gap in the struct-field type path,
not anything specific to FreeBSD or to `struct passwd`.

## String, memory, and randomness extensions (`slate-sfzn.10.9`)

This ticket's original design assumed FreeBSD's `qsort_r` diverges from
glibc's — the context-pointer argument sitting in a different position,
with the comparator taking it first instead of last — and planned a
FreeBSD-specific function-pointer typedef to model that. **The oracle
disproves this for the 15.1 baseline.** FreeBSD 14 changed `qsort_r`'s
prototype to comply with POSIX: `void qsort_r(void *, size_t, size_t, int
(*)(const void *, const void *, void *), void *);` — byte-for-byte the
same shape already declared in the shared shim (comparator `(a, b, arg)`,
`arg` last). The historical BSD order (`thunk` before `compar`; comparator
takes `thunk` first) survives only as a symbol-versioned compat shim
(`qsort_r@FBSD_1.0` / `__qsort_r_compat`) for binaries linked against the
pre-14 ABI; any new translation gets the POSIX-shaped declaration, which
this repo already had. No FreeBSD-specific overlay was correct to add
here — doing so would have modeled an ABI FreeBSD itself no longer
exposes to new code.

`strlcpy`/`strlcat` (`string.h`) and `reallocarray`/`arc4random`/
`arc4random_buf`/`arc4random_uniform` (`stdlib.h`) all matched the shared
shim's existing declarations exactly against both oracle arches — same
signatures, same `__BSD_VISIBLE` gating that lines up with the shim's
existing `_GNU_SOURCE || _BSD_SOURCE` guard (on by default for FreeBSD,
same as every other libc profile here). `string.h`/`stdlib.h` were
already in `freebsd-basic-headers.txt` from `slate-sfzn.10.2` and already
pass the header-compile suite.

`qsort_s` (C11 Annex K) does exist on FreeBSD, but pulling it in would
require modeling `rsize_t`/`errno_t`/`constraint_handler_t` — machinery
this shim has never implemented for any libc profile — for a function
neither the ticket's acceptance criteria nor its core ask require. Left
out of scope, consistent with the epic's "keep narrow" precedent
(`slate-sfzn.10.7`/`.10.8`).

Net result: **zero `libc-shim` code changes.** Verified end-to-end via
`slate translate` + `cargo check --target {x86_64,aarch64}-unknown-freebsd`
for a fixture exercising `strlcpy`/`strlcat`, `reallocarray`, `qsort_r`
with a context-carrying comparator, and
`arc4random`/`arc4random_buf`/`arc4random_uniform` — both architectures
check clean with zero warnings.

## Feature selection and symbol versions (`slate-sfzn.10.10`)

`bits/freebsd/features.h` mirrors the 15.1 `sys/_visible.h` state machine.
The default namespace exposes BSD extensions, POSIX.1-2024, XSI Issue 8,
and C23; `_POSIX_C_SOURCE`, `_XOPEN_SOURCE`, and the strict FreeBSD C-source
macros narrow that surface without synthesizing glibc's `_BSD_SOURCE` or
`_GNU_SOURCE`. `sys/param.h` exposes `__FreeBSD_version=1501000`, and the
feature profile rejects any other release number so future declarations
cannot silently enter the modeled surface.

FreeBSD compatibility redirects use file-scope `.symver` assembly rather
than Clang `AsmLabelAttr`. The macro provenance plugin now collects those
directives before visiting calls. A call fact retains the source macro
spelling, the resolved base ELF symbol, and a separate `FBSD_1.x` version.
The historical-form `qsort_r` fixture therefore records source `qsort_r`,
foreign symbol `qsort_r`, version `FBSD_1.0`; the POSIX-form call remains on
the unversioned source declaration. Carrying that fact into emitted Rust is
left to `slate-sfzn.10.12`.
