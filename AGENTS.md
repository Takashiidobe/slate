# Instructions for AI Agents

This is a Cargo workspace. **Run every cargo, nextest, bd, llog, and tool
command from the workspace root.** `.cargo/config.toml` sends all builds to
`target/test-cache/`, so binaries live at `target/test-cache/release/slate` and
`target/test-cache/release/slate-parser`. `target/release/` is not used.

| Crate                        | What it is                                           | Crate instructions                     |
| ---------------------------- | ---------------------------------------------------- | -------------------------------------- |
| `crates/slate`               | C -> Rust translator; lowers slate-parser IR to Rust | `crates/slate/CLAUDE.md`               |
| `crates/slate-parser`        | C preprocessor, parser, sema, and typed IR           | `crates/slate-parser/AGENTS.md`        |
| `crates/slate-intrinsic-gen` | Generates the frontend intrinsic catalog from LLVM   | `crates/slate-intrinsic-gen/README.md` |
| `crates/slate-sysroots`      | Installs per-target C headers for slate-parser       | `crates/slate-sysroots/README.md`      |

## Toolchain

Always rust nightly, pinned in `rust-toolchain.toml` and bumped from time to
time.

## Routing work

Route by what the problem is, not by the bead ID (a `slate-*` bead can be a
parser bug, and the title prefix is not reliable):

1. If it looks like it is in the parser (preprocessing, `#include`/`#embed`,
   lexing, parsing, sema, the typed IR, or a parser diagnostic), read
   `crates/slate-parser/AGENTS.md`.
2. Otherwise it is a backend issue (lowering IR to Rust, output, the CLI,
   differential suites): read `crates/slate/CLAUDE.md`.

Reproduce first and read the error text. If it comes from `crates/slate-parser`,
it is a parser bug even if the failing test lives in `crates/slate`. Make the
fix where it belongs, follow that crate's instructions for the part you changed,
and run both crates' test profiles.

## Testing

Always test with release nextest profiles from the root:

```bash
cargo nextest r --release --profile slate    # crates/slate: slate-frontend differential suites
cargo nextest r --release --profile parser   # crates/slate-parser: FileCheck fixtures
```

Run the profile for each crate you changed. The crate instructions describe
single-fixture runs.

While working on one flavor's behavior (compiler options, for example the cl.exe
options in the `slate-3amj.5` epic), iterate with that flavor's profile. Each
runs both crates' fixtures for one flavor: slate-parser's `<flavor>/`,
`error/<flavor>/` and `suites/*/<flavor>/`, and slate's `generated_differential`
plus unsupported ratchet over that flavor's fixtures (the root and arch
directories for clang, `gcc/` or `msvc/`). `clang` and `gcc` run x86_64, i686
and aarch64; `msvc` runs x86_64 and i686 against cl.exe, and builds aarch64
without running it (wine cannot run ARM64 binaries here). They skip slate's
corpus suites, so run the full `slate` and `parser` profiles before committing:

```bash
cargo nextest r --release --profile clang
cargo nextest r --release --profile gcc
cargo nextest r --release --profile msvc
```

## Beads Issue Tracker

This project uses **bd (beads)** for issue tracking. Run `bd prime` to see full
workflow context and commands.

```bash
bd ready              # Find available work
bd show <id>          # View issue details
bd update <id> --claim  # Claim work
bd close <id>         # Complete work
```

- Use `bd` for ALL task tracking — do NOT use TodoWrite, TaskCreate, or markdown
  TODO lists
- Run `bd prime` for detailed command reference and session close protocol
- Use `bd remember` for persistent knowledge — do NOT use MEMORY.md files
- Every new bead needs a parent (`bd create --parent <id>`): one of the
  top-level epics below, or a sub-epic under one. Never make a top-level bead.

| Epic         | Area                                                   |
| ------------ | ------------------------------------------------------ |
| `slate-3amj` | Compiler flags that change semantics (all flavors)     |
| `slate-e36k` | Clang flavor compatibility                             |
| `slate-78pa` | GCC flavor compatibility                               |
| `slate-j3by` | MSVC flavor and Windows ABI compatibility              |
| `slate-5exy` | Standard C, preprocessor, and sema                     |
| `slate-yar4` | Rust lowering, ABI, and runtime correctness            |
| `slate-gbgp` | Rewrite correctness and Rust idioms                    |
| `slate-f2zu` | Real-world corpus: translate, build and run C projects |
| `slate-3of2` | gcc-torture and gcc.dg suites                          |
| `slate-xw0x` | Test suites and the differential harness               |
| `slate-5q63` | Tooling, performance, and project translation          |

## Navigating code

Search in this order before reading or re-deriving anything:

1. `bd search "<query>"` for existing or closed beads on the subsystem.
2. `llog search "<query>"` for past decisions and the history of a test or fix.
3. For a bead that names a test, check `git log -S"<test name>"`: the test may
   have been removed, which makes the bead stale.

Then pick the tool by what you are searching:

- Rust code you have edited this session, or a file `status` lists as changed:
  the LSP (rust-analyzer), since the index's lines there are stale. `hover`,
  `goToDefinition`, `findReferences`, `workspaceSymbol`, and `documentSymbol`
  for an enum's variants or a file's outline. Put the cursor on the identifier
  itself (for example the variant name line, not its `#[error]` attribute).
- Everything else (wiki, fixtures, C sources, `.beads`): grep. Quote globs; the
  shell is fish and an unquoted `--include=*.rs` errors.

rust-analyzer returns nothing or "no references" while it is still indexing, and
the first call after startup often fails. Wait about 5 seconds and retry before
you fall back to grep.

## Shell

The shell is fish: write Python scripts instead of bash scripts.

## Wiki

`wiki/` is shared by both crates: `wiki/concepts/` holds durable design and
decision docs, and `wiki/log/` holds point-in-time entries. Use
`llog search "<query>"` before re-deriving a decision that may already be
recorded.

## Session Completion

1. **File issues for remaining work** - Create beads for anything that needs
   follow-up.
2. **Run quality gates** (only if Rust changed):
   - `cargo clippy -p <crate> --allow-dirty --fix` for each changed crate, to
     apply what can be fixed automatically
   - `cargo fmt`
   - the nextest profile for each changed crate
3. **Update issue status** - Close finished work, update in-progress items.
4. **Log every change** - `llog new "<title>" "<entry>"`, summarizing the change
   in no more than 30 lines. Required before closing a bead. No need for
   `llog --help`:

   ```bash
   llog new "Name missing #embed resources" "slate-rpkp: #embed of a missing resource reported 'header not found'. Added PPErrorKind::EmbedResourceNotFound (pp/error.rs), used by expand_embed (pp/embed.rs), message matches clang. Fixture: error/clang/linux/x86_64/embed-missing-resource.c."
   ```

5. **Commit** - `git commit -m ...` with a one-line message summarizing the
   task.
6. **Hand off** - Summarize changes, validation, issue status, and the commit.

**Critical rules:**

- Explicit user or orchestrator instructions override this block.
- Do not push without clear authority from the user.
- If a required sync or push is blocked, stop and report the exact command and
  error.
