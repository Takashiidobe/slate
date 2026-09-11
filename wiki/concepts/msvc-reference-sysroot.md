# MSVC reference sysroot

Slate uses a pinned xwin splat as an external oracle for Microsoft CRT and
Windows UCRT headers. Review the [Microsoft license](https://go.microsoft.com/fwlink/?LinkId=2086102),
then bootstrap it from the repository root with:

```bash
tools/bootstrap-msvc-sysroot.sh --accept-license
```

`XWIN_ACCEPT_LICENSE=1 tools/bootstrap-msvc-sysroot.sh` is the equivalent
non-interactive form. Omitting explicit acceptance fails before Cargo or xwin
runs. The command installs no global tool and is safe to rerun.

The ignored output is entirely under `target/`:

| Path                                     | Contents                                                 |
| ---------------------------------------- | -------------------------------------------------------- |
| `target/msvc-tools`                      | pinned xwin executable and repository-local Cargo home   |
| `target/msvc-xwin-cache`                 | pinned manifest plus Microsoft download and unpack cache |
| `target/msvc-sysroot/crt/include`        | Microsoft CRT headers                                    |
| `target/msvc-sysroot/sdk/include/ucrt`   | Windows SDK UCRT headers                                 |
| `target/msvc-sysroot/include-roots.json` | machine-readable target and include roots                |

The script removes splatted libraries and the UM, WinRT, and C++/WinRT include
trees. It finishes by asking the configured CIR Clang to parse `vcruntime.h`
and `stdio.h` for `x86_64-pc-windows-msvc`. Set `SLATE_CLANG` when the compiler
is not at the repository default described in [slate overview](slate-overview.md).

The xwin release, immutable Visual Studio package-manifest URL and digest, CRT
version, SDK version, architecture, and target triple are constants at the top
of `tools/bootstrap-msvc-sysroot.sh`. To update the oracle, update those pins
together, remove `target/msvc-sysroot`, and rerun the setup command. The script
automatically replaces a sysroot whose version stamp no longer matches; keeping
`target/msvc-xwin-cache` allows unchanged downloads to be reused.

To remove everything produced by the bootstrap command:

```bash
rm -rf target/msvc-tools target/msvc-xwin-cache target/msvc-sysroot
```

Removing only `target/msvc-sysroot` preserves the pinned xwin installation and
download cache for the next run. None of these paths are committed because the
repository ignores all of `target/`.

## Header declaration matrix

Generate the checked-in probe for every header in the MSVC manifest:

```bash
SLATE_LIBC_DECL_TARGET=msvc-x86_64 cargo nextest r --release --profile libc \
  --test libc_declaration_matrix_suite \
  -E 'test(generate_declaration_matrix_fixtures)' \
  --run-ignored ignored-only --nocapture
```

Check the headers one by one against the pinned CRT and UCRT oracle:

```bash
SLATE_LIBC_DECL_TARGET=msvc-x86_64 cargo nextest r --release --profile libc \
  --test libc_declaration_matrix_suite -E 'test(declaration_matrices)' --nocapture
```

MSVC stays opt-in because an incomplete shim is expected to make this matrix
fail. The ordinary `libc` profile continues to run the hostable matrices.
