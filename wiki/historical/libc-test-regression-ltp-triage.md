# Triage: musl libc-test `src/regression` and LTP, for vendoring value

> Filed under `slate-b66b.5.3`. This is a triage report, not a vendoring
> commitment -- see [cross-target-toolchains.md](cross-target-toolchains.md)
> and [libc-test-functional-harness.md](libc-test-functional-harness.md) for
> the already-vendored `functional`/`api` buckets this compares against.

## The right lens

Most of `src/regression`'s 69 cases pin **musl implementation bugs** (malloc
OOM paths, DNS resolver edge cases, regex engine corner cases). A libc
implementation bug is identical whether it's called from native C or from
Slate-generated Rust linked against the same libc/shim, so it doesn't exercise
what Slate's differential suite is actually for. The right filter isn't "is
this a famous historical bug" but **"does this exercise a C-ABI surface where
Slate's Rust codegen could diverge from clang's native C codegen even when
both link the identical libc"** -- variadic marshaling, struct-by-value
passing/return, signal-frame/register-save layouts, raw syscall argument
encoding, TLS/DTV, and float-representation edge cases (Slate's own `f80`
emulation in particular, see [long-double-f80.md](long-double-f80.md)).

Filtered that way, most of `src/regression` is out of scope. Five cases are in.

**Update (`slate-b66b.5.5`):** all five were actually attempted. Two don't
clear the existing native-admission gate on this host and were **not
vendored**:

- `sigaltstack.c` fails to *compile* against this host's glibc -- glibc
  2.34+ made `SIGSTKSZ` a `sysconf()` call, breaking the test's file-scope
  `static char stack[SIGSTKSZ]` declaration. An upstream libc-test/glibc-version
  incompatibility, not a Slate issue; not worth patching upstream test source
  to force through.
- `fpclassify-invalid-ld80.c` compiles, but its own `t_error` assertions fail
  against real glibc's `%La` formatting of denormal-boundary x87 bit
  patterns -- the expected strings were authored against musl's formatting,
  not glibc's.

The other three (`printf-fmt-n.c`, `syscall-sign-extend.c`,
`tls_get_new-dtv.c`) are vendored in
`tests/fixtures.libc-test/regression/supported/` and pass under
`tests/libc_test_regression_suite.rs`. Vendoring `tls_get_new-dtv.c` also
surfaced a real, unrelated libc-shim bug -- see `wiki/log/2026-09-08-10-49.md`
for the `pthread_barrier_t`/`pthread_barrierattr_t` ABI fix it drove.

## Recommended: vendor these five

| Case | Why it's ABI-relevant to Slate specifically | Fragility | Outcome |
| --- | --- | --- | --- |
| `printf-fmt-n.c` | `%n` writes back through a `va_list`-marshaled pointer argument -- stresses variadic pointer write-back, not just read, which the existing `snprintf.c`/`sscanf.c` functional cases don't cover | none: deterministic, single-threaded | vendored, passing |
| `syscall-sign-extend.c` | Calls `syscall(SYS_read, ...)` directly with a raw syscall number and pointer arg -- exactly the "raw syscall argument encoding" gap flagged when this epic was scoped | none: deterministic, no timing | vendored, passing |
| `sigaltstack.c` | Exercises real signal delivery via `raise()` on the *same* thread (no cross-thread race) and checks `stack_t`/`ss_flags`/`MINSIGSTKSZ` behavior -- a runtime signal-frame check static ABI probing can't do | low: synchronous, no cross-thread timing, but still a real signal handler | **not vendored** -- fails to compile against this host's glibc (`SIGSTKSZ` VLA break) |
| `fpclassify-invalid-ld80.c` | Directly probes invalid/pseudo-denormal x87 80-bit bit patterns through `fpclassify()` and `%La` formatting -- the sharpest available test of Slate's `rustc_apfloat`-backed `LongDouble` emulation against real hardware edge cases | none: `#if LDBL_MANT_DIG==64` guards it to a no-op off x86_64, deterministic otherwise | **not vendored** -- fails its own assertions against real glibc's `%La` formatting |
| `tls_get_new-dtv.c` (+ `tls_get_new-dtv_dso.c`/`.so`) | TLS/DTV growth after `dlopen()` from multiple threads -- genuinely new coverage (no TLS+dlopen interaction case exists today) | low: uses `pthread_barrier_wait`, which synchronizes rather than races -- deterministic outcome, just multi-threaded | vendored, passing (after fixing a real `pthread_barrier_t` ABI bug it exposed) |

None of these five need any `src/common/*` helper beyond `test.h`/`print.c`,
which `tools/filter-libc-test.sh` already merges into every vendored case --
so the existing single-file vendoring path handles four of them with no
harness changes. `tls_get_new-dtv.c` is the exception: its dlopen target
(`tls_get_new-dtv_dso.so`) isn't `_dlopen`-suffixed, so it falls outside the
naming convention `libc_test_functional_suite.rs` currently uses to find a
companion `.so` (documented in
[libc-test-functional-harness.md](libc-test-functional-harness.md)). Vendoring
it needs a small, explicit naming-convention extension on the Rust side, not
just a corpus copy -- call that out as its own small task if `.5.2` or a
follow-up picks it up.

Estimated cost: all five are single-process, no `setrlim`/OOM-injection,
sub-second cases -- in line with the existing 68-case functional corpus's
~7s total run. Adding them is noise against the current runtime.

## Explicitly not recommended from `src/regression`

The remaining ~64 cases fall into buckets that don't add signal for Slate's
purpose, or add real flakiness risk:

- **OOM/rlimit-injection tests** (`malloc-oom.c`, `malloc-brk-fail.c`,
  `daemon-failure.c`, `mkstemp-failure.c`, `mkdtemp-failure.c`,
  `setenv-oom.c`, `pthread_create-oom.c`, `rlimit-open-files.c`) -- depend on
  `setrlim`/overcommit behavior that varies by host/QEMU sandbox; exactly the
  kind of environment-sensitive flakiness this triage was asked to screen out.
- **Pure algorithmic/library-correctness cases** (`regex-*.c`,
  `regexec-nosub.c`, `strverscmp.c`, `wcsstr-false-negative.c`,
  `memmem-oob*.c`, `mbsrtowcs-overflow.c`, `iconv-roundtrips.c`) -- test the
  shim's own algorithm correctness, not a C-ABI surface Slate's codegen
  touches differently than clang's.
- **More pthread edge cases** (`pthread_cond-smasher.c`,
  `pthread_once-deadlock.c`, `pthread-robust-detach.c`,
  `pthread_rwlock-ebusy.c`, `pthread_atfork-errno-clobber.c`,
  `pthread_cancel-sem_wait.c`, `pthread_exit-cancel.c`,
  `pthread_exit-dtor.c`) -- real synchronization/cancellation races by
  design (the names say so: "smasher", "race", "deadlock"). `pthread_mutex.c`,
  `pthread_cond.c`, `pthread_robust.c`, `pthread_tsd.c`, and
  `pthread_cancel-points.c` are already vendored in `functional/` and cover
  the ABI-layout side; these add race-condition risk for very little
  incremental ABI signal.
- **Environment-dependent** (`getpwnam_r-crash.c`, `getpwnam_r-errno.c`,
  `statvfs.c`) -- need real `/etc/passwd`/mounted-filesystem state that
  differs across CI/dev hosts and QEMU chroots.
- **Everything else** (`iswspace-null.c`, `uselocale-0.c`,
  `fgetwc-buffering.c`, `fflush-exit.c`, `ftello-unflushed-append.c`,
  `fgets-eof.c`, `setvbuf-unget.c`, `scanf-*.c` besides the ones already
  implied by existing coverage, `inet_ntop-v4mapped.c`,
  `inet_pton-empty-last-field.c`, `lrand48-signextend.c`, `dn_expand-*.c`,
  `execle-env.c`, `sigreturn.c`, `sigprocmask-internal.c`,
  `flockfile-list.c`, `rewind-clear-error.c`) -- narrow historical musl bugs
  with no meaningful Slate-codegen angle, or (like `execle-env.c`'s
  process-replacing `execve`) awkward to fit the current single-binary
  differential harness for the value they'd add.

## LTP: not recommended, no vendoring work warranted

There's no local LTP checkout on this host, so this is a structural
judgment rather than a file-by-file survey -- flagging that limitation
explicitly rather than inventing specific LTP filenames. LTP's libc-facing
tests (`testcases/kernel/syscalls/`, the bundled Open POSIX Test Suite under
`testcases/open_posix_testsuite/`) are fundamentally **kernel-interface and
POSIX-conformance** tests: they validate that a given syscall/libc call
returns the right value and errno for a huge matrix of argument combinations.
That's orthogonal to Slate's translation-correctness question in the same way
most of `src/regression` was -- if the syscall/errno behavior is identical
between C and Slate-generated Rust (which it will be, since both marshal
through the same libc-shim call), LTP mostly re-proves libc/kernel behavior
Slate never touches, rather than exposing C-ABI divergence.

Combined with the integration cost -- LTP is an autoconf/make-based project
with its own build system and thousands of files, a categorically bigger lift
than libc-test's flat `.c`-per-case layout that `filter-libc-test.sh` already
handles -- the cost/benefit doesn't clear the bar. **Conclusion: do not vendor
LTP.** If a future need for POSIX-conformance-style testing arises, the Open
POSIX Test Suite standalone (not through LTP) would be the narrower way in,
but nothing in the current corpus gap justifies opening that work now.

## Bottom line

Done (`slate-b66b.5.5`): three of the five are vendored and passing under
`tests/libc_test_regression_suite.rs`, natively on this host. They'll run
against musl and cross-arch targets once `slate-b66b.5.2` extends target
coverage the same way it does for `functional/`. The other two are
permanently out (not blocked on anything -- they don't clear native admission
on this host's glibc at all). No LTP work is warranted.
