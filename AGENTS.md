# Instructions for AI Agents

This is a Cargo workspace. **Run every cargo, nextest, bd, llog, and tool
command from the workspace root.** `.cargo/config.toml` sends all builds to
`target/test-cache/`, so binaries live at `target/test-cache/release/slate`
and `target/test-cache/release/slate-parser`. `target/release/` is not used.

| Crate                  | What it is                                           | Crate instructions             |
| ---------------------- | ---------------------------------------------------- | ------------------------------ |
| `crates/slate`         | C -> Rust translator; lowers slate-parser IR to Rust | `crates/slate/CLAUDE.md`       |
| `crates/slate-parser`  | C preprocessor, parser, sema, and typed IR           | `crates/slate-parser/AGENTS.md` |
| `crates/slate-sysroots`| Installs per-target C headers for slate-parser       | `crates/slate-sysroots/README.md` |

## Routing work by bead ID

Every bead ID starts with `slate-`, so check the longer prefix first:

1. `slate-parser-*` -> read `crates/slate-parser/AGENTS.md`.
2. Any other `slate-*` -> read `crates/slate/CLAUDE.md`.

A bead may need changes in the other crate (for example, a Slate lowering bead
that exposes a parser bug). Make the fix where it belongs, follow that crate's
instructions for the part you changed, and run both crates' test profiles.

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

**Architecture in one line:** issues live in a local Dolt DB; sync uses `refs/dolt/data` on your git remote; `.beads/issues.jsonl` is a passive export. See https://github.com/gastownhall/beads/blob/main/docs/SYNC_CONCEPTS.md for details and anti-patterns.

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
4. **Log every change** - `llog new`, summarizing the change in no more than 30 lines.
5. **Commit** - `git commit -m ...` with a one-line message summarizing the task.
6. **Hand off** - Summarize changes, validation, issue status, and the commit.

**Critical rules:**

- Explicit user or orchestrator instructions override this block.
- Do not push without clear authority from the user.
- If a required sync or push is blocked, stop and report the exact command and error.
