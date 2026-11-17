# Kernel selftests

<!-- toc -->
- [Why kselftest](#why-kselftest)
- [Running it](#running-it)
- [How it works](#how-it-works)
  - [Building the tests](#building-the-tests)
  - [Generic userspace under TCG](#generic-userspace-under-tcg)
  - [The initramfs and /init](#the-initramfs-and-init)
  - [Comparison](#comparison)
- [Status](#status)
- [Adding collections](#adding-collections)
<!-- /toc -->

`tools/corpus/kernel_selftest.py` runs the kernel's userspace selftests
(`tools/testing/selftests`) on the native and translated defconfig bzImages
under QEMU and compares the per-test TAP results. It is the `kselftest` stage of
`e2e.py linux` ([Adding corpus tests](corpus-e2e.md#linux-kernel)), after
`boot`.

## Why kselftest

kselftest exercises the translated kernel through syscalls (futex, signals,
ptrace, pidfd, namespaces, mm, vDSO, x86 entry) without changing the kernel
config, so the kernel that already boots is the one tested: no retranslation,
no relink. KUnit and the in-kernel self-tests (testmgr, `TEST_XARRAY`,
`TEST_MAPLE_TREE`) need `CONFIG_KUNIT`, `CONFIG_CRYPTO_SELFTESTS` and friends,
which defconfig leaves off; a config change rewrites `autoconf.h` and forces a
full retranslation (slate-f2zu.14.36, which depends on this).

## Running it

```bash
# subset: comma-separated collections or collection:test entries
python3 tools/corpus/kernel_selftest.py ~/slate-translated/slate-linux \
    --only kcmp,futex,pidfd,signal,ptrace,clone3,core,pipe,mincore,sync

# every built test, as the e2e stage does
python3 tools/corpus/e2e.py linux --from kselftest
```

Output: `<out>/kselftest.md` (tables, regressions, flaky and kernel failure
lines), raw serial logs in `<out>/kselftest/{native-1,native-2,translated}.log`.
Use `--only` while iterating: the 10-collection subset above takes about 2m15s
wall time; the full list is much longer (`mm` allows 900 s per test).

Other options: `--native-runs` (default 2), `--timeout` (whole QEMU run, default
3600 s), `--cpus`, `--memory`, `--append`.

## How it works

### Building the tests

`build()` installs uapi headers (`make headers_install` into
`<out>/kselftest/usr`), then `make -C tools/testing/selftests TARGETS=...
install` with `O=` and `INSTALL_PATH=<out>/kselftest/install`. `TARGETS` lists
the collections that build on this host (35, 221 tests). Left out: `size`
(nolibc link fails), `fchmodat2` (needs `-lasan` with gcc), collections needing
hardware or extra configs.

Some collections (`prctl`, `uevent`) build their `TEST_PROGS` inside the source
tree despite `O=`, so the build first runs `git clean -fdX` on each selected
`tools/testing/selftests/<target>`; otherwise a stale in-tree binary is
installed. `stamp.json` (targets, kernel `HEAD`, compiler) decides when to
rebuild.

### Generic userspace under TCG

This host has no `/dev/kvm`, so QEMU runs TCG, which has no AVX-512. The host's
CachyOS glibc, bash and coreutils are x86_64_v4 builds:

- dynamic binaries linked against the host `crt1.o` carry
  `GNU_PROPERTY_X86_ISA_1_NEEDED: x86-64-v4`, and ld.so refuses them ("CPU ISA
  level is lower than required", exit 127);
- static binaries pull v4 code from the host `libc.a` and die with SIGILL.

So the harness takes userspace from Arch's generic packages:
`generic_package()` maps each needed host file to its package with
`pacman -Qoq`, downloads the `core`/`extra`/`multilib` build from the servers in
`/etc/pacman.d/mirrorlist`, and caches it under `<out>/kselftest/packages/`.
Tests are compiled with `CC="gcc -B<generic glibc>/usr/lib/"`; the root also
holds lib32-glibc's `usr/lib32` with a `usr/lib/32 -> ../lib32` link, because
gcc searches `<prefix>/32/` for `-m32` start files and libraries. `readelf -n`
then shows `ISA needed: x86-64-baseline` (the "ISA used" line lists glibc's
ifunc variants, which are selected by CPUID and are fine).

busybox is `<out>/boot/busybox`, the generic static build `run-qemu.py` uses.

### The initramfs and /init

`initramfs()` packs the install tree under `/kselftest`, busybox with its
applet links, generic `bash` and coreutils `timeout`, every shared library
`ldd` reports (from the generic packages), `/etc/passwd` and `/etc/group`, and
`/init`. The cpio is written by `kernel_boot.write_cpio`, which adds
`/dev/console`.

`/init` mounts proc, sysfs, devtmpfs, devpts, `/dev/shm`, mqueue, tmpfs, debugfs
and tracefs, brings up `lo`, and runs the list itself instead of
`run_kselftest.sh`. The upstream runner pipes each test through `sed`; a test
that fails an assertion can leave a child (for example
`pidfd_setns_test`'s `no_foul_play` child in `pause()`) holding that pipe, and
the runner waits forever even though `timeout` killed the test. The loop:

- reads the selection from `kselftest=` on the kernel command line (`--only`),
  so changing the subset does not rebuild the initramfs;
- runs each test with output to a file, under `timeout -k 10 <seconds>` from
  the collection's `settings` (default 45, `timeout=0` capped at 1200);
- runs `kill -KILL -1` after each test to remove orphans;
- prints runner-compatible TAP: `ok`, `# SKIP` (exit 4), `# TIMEOUT` (124/137),
  `# exit=N`, with test output prefixed `# `;
- prints the boot marker and powers off.

### Comparison

The native kernel boots `--native-runs` times and the translated once, all in
parallel. `parse()` reads top-level results keyed `collection:test` and
one-level subtests keyed `collection:test/name`. A result that differs between
native runs is flaky and not compared. A regression is a stable native result
that the translated run does not reproduce, unless native fails or times out
(listed separately). Kernel failure lines (`kernel_boot.FAILURE`) that native
lacks after normalization are reported too. The stage fails on a missing
marker or any test or subtest regression.

## Status

2026-11-17, `--only kcmp,futex,pidfd,signal,ptrace,clone3,core,pipe,mincore,sync`:
31 tests and 205 subtests compared, 0 regressions, 0 flaky, identical counts
(ok 21, fail 8, skip 1, timeout 1). Native-failing tests in that subset
(defconfig/TCG environment, not slate): `futex:run.sh`, `mincore`,
`pidfd_{file_handle,info,setattr,setns,xattr}_test`, `pidfd_test` (timeout),
`ptrace:vmaccess`; `sync_test` skips.

The full 221-test run has not been done yet.

## Adding collections

Add the collection to `TARGETS`, then run it with `--only <collection>`. If it
needs a library, `ldd` and `generic_package` pick it up as long as `pacman -Qo`
knows the host file and Arch ships it in `core`, `extra` or `multilib`. A test
that fails natively is not a slate failure; drop the collection only if it
cannot build, and note it in [Building the tests](#building-the-tests).
