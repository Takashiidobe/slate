# Instructions for AI Agents

This is a Cargo workspace. **Run every cargo, nextest, bd, llog, and tool
command from the workspace root.** `.cargo/config.toml` sends all builds to
`target/test-cache/`, so binaries live at `target/test-cache/release/slate`
and `target/test-cache/release/slate-parser`. `target/release/` is not used.

| Crate                  | What it is                                           | Crate instructions             |
| ---------------------- | ---------------------------------------------------- | ------------------------------ |
| `crates/slate`         | C -> Rust translator; lowers slate-parser IR to Rust | `crates/slate/CLAUDE.md`       |
| `crates/slate-parser`  | C preprocessor, parser, sema, and typed IR           | `crates/slate-parser/AGENTS.md` |
| `crates/slate-intrinsic-gen` | Generates the frontend intrinsic catalog from LLVM | `crates/slate-intrinsic-gen/README.md` |
| `crates/slate-sysroots`| Installs per-target C headers for slate-parser       | `crates/slate-sysroots/README.md` |

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
fix where it belongs, follow that crate's instructions for the part you
changed, and run both crates' test profiles.

## Testing

Always test with release nextest profiles from the root:

```bash
cargo nextest r --release --profile slate    # crates/slate: slate-frontend differential suites
cargo nextest r --release --profile parser   # crates/slate-parser: FileCheck fixtures
```

Run the profile for each crate you changed. The crate instructions describe
single-fixture runs.

## Beads Issue Tracker

This project uses **bd (beads)** for issue tracking. Run `bd prime` to see full workflow context and commands.

```bash
bd ready              # Find available work
bd show <id>          # View issue details
bd update <id> --claim  # Claim work
bd close <id>         # Complete work
```

- Use `bd` for ALL task tracking — do NOT use TodoWrite, TaskCreate, or markdown TODO lists
- Run `bd prime` for detailed command reference and session close protocol
- Use `bd remember` for persistent knowledge — do NOT use MEMORY.md files

## Navigating code

Search in this order before reading or re-deriving anything:

1. `bd search "<query>"` for existing or closed beads on the subsystem.
2. `llog search "<query>"` for past decisions and the history of a test or fix.
3. For a bead that names a test, check `git log -S"<test name>"`: the test may
   have been removed, which makes the bead stale.

Then pick the tool by what you are searching:

- Rust code you have not edited this session: the SCIP index,
  `python3 tools/scip/scip.py <command>`. It answers instantly, with no
  server to wait for:
  - `def <name>`: definition site and signature
  - `refs <name>`: every reference, with its enclosing function
  - `callers <name>`: the functions that reference it
  - `hover <name>`: signature and docs
  - `outline <file>`: a file's definitions, in order
  - `search <substring>`: defined symbols by name
  - `status`: the commit it was built at, and the `.rs` files changed since

  Qualify names to narrow them (`Preprocessor::reject_poisoned`,
  `PPErrorKind::PoisonedIdentifier`) or pass `--crate slate-parser`. Rebuild
  it with `python3 tools/scip/scip.py build` (about 90 s) when `status`
  says it is stale. `python3 tools/scip/site.py` writes a browsable,
  cross-crate view of it to `.scip/site/index.html`.
- Rust code you have edited this session, or a file `status` lists as
  changed: the LSP (rust-analyzer), since the index's lines there are
  stale. `hover`, `goToDefinition`, `findReferences`, `workspaceSymbol`,
  and `documentSymbol` for an enum's variants or a file's outline. Put the
  cursor on the identifier itself (for example the variant name line, not
  its `#[error]` attribute).
- Everything else (wiki, fixtures, C sources, `.beads`): grep. Quote globs;
  the shell is fish and an unquoted `--include=*.rs` errors.

rust-analyzer returns nothing or "no references" while it is still indexing,
and the first call after startup often fails. Wait about 5 seconds and retry
before you fall back to grep.

## Shell

The shell is fish: write Python scripts instead of bash scripts.

## Wiki

`wiki/` is shared by both crates: `wiki/concepts/` holds durable design and
decision docs, and `wiki/log/` holds point-in-time entries. Use
`llog search "<query>"` before re-deriving a decision that may already be
recorded.

## Session Completion

1. **File issues for remaining work** - Create beads for anything that needs follow-up.
2. **Run quality gates** (only if Rust changed):
   - `cargo clippy -p <crate> --allow-dirty --fix` for each changed crate, to apply what can be fixed automatically
   - `cargo fmt`
   - the nextest profile for each changed crate
3. **Update issue status** - Close finished work, update in-progress items.
4. **Log every change** - `llog new "<title>" "<entry>"`, summarizing the change in no more than 30 lines. Required before closing a bead. No need for `llog --help`:

   ```bash
   llog new "Name missing #embed resources" "slate-rpkp: #embed of a missing resource reported 'header not found'. Added PPErrorKind::EmbedResourceNotFound (pp/error.rs), used by expand_embed (pp/embed.rs), message matches clang. Fixture: error/clang/linux/x86_64/embed-missing-resource.c."
   ```
5. **Commit** - `git commit -m ...` with a one-line message summarizing the task.
6. **Hand off** - Summarize changes, validation, issue status, and the commit.

**Critical rules:**

- Explicit user or orchestrator instructions override this block.
- Do not push without clear authority from the user.
- If a required sync or push is blocked, stop and report the exact command and error.
