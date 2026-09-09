# Linux libc header-matrix handoff

_created 2026-09-09_

## Purpose

The Linux declaration matrix compares each selected public header as seen by
the configured real glibc or musl oracle with the same header as seen through
`libc-shim`. It is not a host-header smoke test. The eight descriptors are
glibc and musl on x86_64, i386, ARM, and AArch64. Every descriptor passes the
Slate target/libc defines to both sides; never replace an unavailable oracle
with the host's headers or use musl as a stand-in for glibc.

The active per-header tickets are children of `slate-b66b.14.2`. Take the
first open child by title, claim it with `bd update <id> --claim`, and leave
the ticket in progress until its generated probe, all applicable descriptors,
and the full `libc` profile are green.

## What the matrix checks

`tests/libc_declaration_matrix_suite.rs` reads each descriptor's
`tests/fixtures.libc-static-test/<target>/headers.txt`. For every listed
header it:

1. extracts the direct/transitive public-header file closure from the real
   oracle and the shim;
2. fails on either an oracle file missing from the shim closure or a shim file
   leaked beyond the oracle closure;
3. compiles and links the checked-in shim probe generated from the oracle's
   functions, objects, type surface, and selected macros.

The fixture is stored at
`tests/fixtures.libc-static-test/<target>/<header-with-slashes-and-dots-as-underscores>/shim-header-matrix.c`.
Oracle and failed-run artifacts belong under `target/libc-declaration-*`; they
are diagnostic output, not checked-in source.

The normal matrix is intentionally one-way: it proves that the shim accepts
the oracle-derived public surface. Do not treat it as permission to add every
shim extension to every profile. Bidirectional checking is currently limited
to `arpa/nameser.h`; only add a header there when its public surface is
expected to be exact in both directions.

`crypt.h` is supplied by standalone libxcrypt on current GNU systems, not by
the glibc core headers alone. Every glibc target needs a matched libxcrypt
development header and library in its oracle sysroot. A program that references
its functions must link `-lcrypt`; a declaration-only header probe does not.
The configured ARM and AArch64 GNU sysroots lack that package, which is an
oracle provisioning gap rather than an ABI or header-visibility distinction.

## Required workflow for one header

1. Read the ticket and inspect the real oracle for every supported descriptor.
   Identify whether the header exists and is public in each libc before adding
   it to a manifest. A glibc-only header must not be inserted into musl's
   manifest, and the converse holds too.
2. Add the header only to the manifests whose real oracle exposes it. Preserve
   profile-specific manifests rather than making a union manifest.
3. Generate the selected fixture for each descriptor that gained the header:

   ```bash
   SLATE_LIBC_DECL_TARGET=glibc-x86_64 cargo nextest r --release --profile libc \
     --test libc_declaration_matrix_suite \
     -E 'test(generate_declaration_matrix_fixtures)' --run-ignored ignored-only
   ```

   Repeat for each glibc/musl and architecture descriptor. Generation derives
   the probe from the real selected oracle; do not hand-edit it to hide a
   failure.

4. Run the selected matrix and inspect its generated source and diagnostics:

   ```bash
   SLATE_LIBC_DECL_TARGET=glibc-x86_64 cargo nextest r --release --profile libc \
     --test libc_declaration_matrix_suite -E 'test(declaration_matrices)'
   ```

5. Reconcile the shim using the narrowest public-header ownership and feature
   gate that matches the oracle -- for a pure missing-macro gap, steps 2-4 and
   most of this step can be done by `tools/reconcile_header.py` instead (see
   below). Then run the focused matrix for all affected descriptors and
   `cargo nextest r --release --profile libc`. Only run `cargo fmt` and
   `cargo clippy --all-targets` if the change touched a `.rs` file; a header
   port that only edits `libc-shim/include/*.h`, `headers.txt` manifests, or
   `tools/*.py` has nothing for either to check.

The C-parsing prerequisite still applies: verify the CIR Clang, matching
`cir-opt`, and macro-dump plugin before a test that invokes Slate parsing.

## Automating steps 2-4: `tools/reconcile_header.py`

For a header whose gap is purely missing `#define` macros, run:

```bash
python3 tools/reconcile_header.py <header> [<header> ...]
```

Per header it does steps 2-4 itself and most of step 5:

1. probes all eight oracles via the existing `emit_oracle_header_macros`
   ignored test (`SLATE_LIBC_DECL_LIBC`/`SLATE_LIBC_DECL_ARCH`/
   `SLATE_LIBC_DECL_HEADER`) to learn which descriptors actually expose the
   header, and each macro's real value there;
2. adds the header only to the manifests whose oracle exposes it;
3. generates the fixture and runs the real matrix per descriptor to get the
   compiler-verified missing-macro set (never guesses from a header diff);
4. groups missing macros by `(descriptor-presence pattern, value)` and picks
   the narrowest `__SLATE_LIBC_*`/`__SLATE_ARCH_*`/`__SLATE_WORDSIZE_*` guard
   for that pattern from a small lattice, falling back to an explicit
   `(libc && arch) || (libc && arch) || ...` expression for any pattern that
   isn't a clean libc/arch/wordsize split;
5. anchors each group's insertion after the nearest preceding macro that
   already exists in the shim header, ordered using the oracle's own
   `definition_file` (real source order -- both the generated fixture and
   `oracle-macros.json` list macros alphabetically, which is not usable for
   placement);
6. writes the merged header in place.

A macro whose value differs across descriptors that share a missing pattern
is a real conflict (needs an `#if` branch inside the guard, not a flat
block) and is never auto-inserted; it prints under `CONFLICTS` instead.
Anything that isn't a plain single-line `#define NAME VALUE` -- multi-line
macros, enums, structs, decls, and glibc's `#define X X` self-referential
feature-marker idiom (paired with an enum of the same name, e.g.
`FTW_STOP`) -- is out of its scope and is either skipped or reported under
`NON-MACRO ERRORS`/`CONFLICTS` for manual reconciliation. A header whose real
oracle content is entirely non-macro (structs, enums, function-pointer
typedefs -- `gconv.h` was this) needs a hand-written transliteration same as
before; the tool only reports "not yet reconciled" for it.

It also never silently treats a non-macro test failure (a header-visibility
mismatch, a type-surface mismatch) as success -- any matrix failure it can't
fully attribute to `#error "...macro is missing..."` lines is surfaced
verbatim under `NON-MACRO ERRORS` rather than swallowed.

Always rerun the tool after any manual fix (include-closure gate, hand-written
struct) -- it re-probes and re-runs the real matrix rather than trusting its
own prior output, so it will pick up whatever macro gaps the manual fix
exposed next. Finish every header the same way regardless of how it got
there: rerun the full matrix per descriptor, review the diff, then `cargo
nextest r --release --profile libc`.

For a single flat oracle header where the whole gap needs hand placement
(elf.h's scale made per-macro placement worth doing directly), the smaller
building block `tools/extract_missing_macros.py <missing-names-file>
<oracle-header> <out-file>` pulls just the `#define` blocks for a list of
missing names out of a real header file in that file's own source order.

## Include-closure rule

Header visibility is part of compliance. A top-level shim header may include a
dependency only when the real header includes the corresponding public or
allowed internal header under the same target/libc/profile. Do not include a
nearby convenience header merely to obtain a typedef, macro, or declaration.
Move a shared primitive into the matching `bits/` leaf or repeat the minimal
definition when the oracle does. Internal shim leaves must remain guarded from
direct inclusion.

For example, the last commit removed unconditional `<sys/types.h>` from
`fcntl.h` and moved common `O_*`/`F_*` constants into `bits/generic/fcntl.h`.
That preserves the oracle's ownership while still making types available
through `bits/types.h`. A header-closure failure is a real failure even if its
shim probe compiles.

## Profile-gating rules

Use the macro family established by `features.h`; do not infer libc from an
architecture or a feature-test macro.

| Situation                   | Required gate/ownership                                                                               |
| --------------------------- | ----------------------------------------------------------------------------------------------------- |
| Linux libc difference       | `__SLATE_LIBC_GLIBC` or `__SLATE_LIBC_MUSL`                                                           |
| 32/64-bit ABI difference    | `__SLATE_WORDSIZE_32` or `__SLATE_WORDSIZE_64`                                                        |
| CPU-specific ABI difference | the relevant `__SLATE_ARCH_*` macro, in addition to the libc gate when needed                         |
| GNU extension               | the real feature-test gate, usually `_GNU_SOURCE`, plus the owning libc gate if musl and glibc differ |
| C23/versioned glibc API     | libc gate plus the exact language/version condition                                                   |

Never expose an API in musl merely because glibc has it, and never hide a
musl API by wrapping it in a glibc condition. Feature-test macros select APIs
within a libc; they do not select the libc. Keep macros, declarations,
typedefs, records, and includes under the same profile condition whenever
their visibility is coupled in the oracle.

## Last-commit facts to preserve

Commit `1e1b6b68` reconciled `fcntl.h`, `stdlib.h`, `string.h`, `sys/types.h`,
and `unistd.h`. Treat these facts as regression constraints for later headers:

- `fcntl.h`: common Linux constants moved to `bits/generic/fcntl.h`; musl alone
  exposes `F_GETOWNER_UIDS`; glibc's `F_GETLK64` family is distinct on 32-bit;
  `AT_EXECVE_CHECK`, handle constants, delegation constants, lock constants,
  `F_SEAL_EXEC`, and `RWH_WRITE_LIFE_NOT_SET` are glibc-only, with several
  additionally CPU-gated. `openat2` takes `const struct open_how *`.
- `stdlib.h`: `<sys/types.h>` and `locale_t` are glibc-owned where GNU locale
  conversions require them. Wait-status macros are shared by glibc and musl,
  but `WNOHANG`, `WUNTRACED`, and `WCOREDUMP` are musl-only in this shim.
  The `_l` conversion family and C23 `once_flag`/`call_once` surface remain
  glibc- and feature/version-gated.
- `string.h`: musl must not acquire `<alloca.h>` or the glibc-only `strndupa`
  helper by transitive inclusion. `strdupa` uses `__builtin_alloca` so it does
  not require that extra visible header.
- `sys/types.h`: musl owns its `<endian.h>` and `<sys/select.h>` transitive
  inclusions and the BSD-style `ulong`/`u_int*_t` names added here. Do not
  promote those includes or names to glibc without an oracle result.
- `unistd.h`: `TEMP_FAILURE_RETRY` is glibc-only.

The exact values and available names must continue to come from the selected
oracle probe. This list describes current ownership; it is not a license to
copy values across libc or architecture profiles.

## Completion and handoff

Review generated fixture diffs for every added header: an empty or truncated
probe often means the oracle collector saw the wrong include closure or a
feature gate, not that the header has no API. Record intentional exceptions in
the ticket with their libc, architectures, feature profile, and oracle reason.
Write one `llog new` entry for the change, close only the verified ticket, and
leave the next alphabetical ticket available for another agent.
