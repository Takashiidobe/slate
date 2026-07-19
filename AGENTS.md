# Agent Instructions

This project uses **bd** (beads) for issue tracking; the command quick reference
and sync model live in the beads integration block below. Run `bd prime` for full
context.

## Build, Test & Architecture

Slate transpiles C to Rust by lowering ClangIR; correctness is verified by
differential testing. **Build/test commands, the required CIR toolchain and its
`SLATE_*` overrides, the architecture, and the feature-adding conventions live in
[CLAUDE.md](CLAUDE.md).** The deeper playbook is in [docs/](docs/README.md) — in
particular [docs/adding-features.md](docs/adding-features.md) before adding any
feature, and [docs/writing-a-fixup.md](docs/writing-a-fixup.md) before writing or
modifying any fixup under `src/fixups/` (fixups are AST-to-AST only). For
raw-Rust-vs-fixuped-Rust semantic trace work, read
[docs/effects.md](docs/effects.md) before changing `src/effects/`. Do not
duplicate that content here; update CLAUDE.md/docs instead.

For parallel agent work, branch selection, and git worktree rules, follow
[WORKFLOW.md](WORKFLOW.md). Agents must check both ready and in-progress beads
and avoid starting tickets linked to active work.

## Fixup Traversal Helpers

When writing or modifying fixups, favor the existing traversal helpers over
private recursive walkers. Fact collectors should use `src/fixups/facts/walk.rs`;
rewrite passes should use `src/fixups/support/walk.rs`. If a traversal shape is
missing, extend the shared helper once instead of adding a pass-local `exprs`,
`stmt_exprs`, or body walker.

## Non-Interactive Shell Commands

File ops may be aliased to `-i` on this host and hang waiting for y/n. Always use
non-interactive flags: `cp -f`, `mv -f`, `rm -f`, `rm -rf`, `cp -rf`. For network
tools use batch modes (`ssh`/`scp -o BatchMode=yes`, `apt-get -y`).

<!-- BEGIN BEADS INTEGRATION v:1 profile:minimal hash:7510c1e2 -->

## Beads Issue Tracker

This project uses **bd (beads)** for issue tracking. Run `bd prime` to see full workflow context and commands.

### Quick Reference

```bash
bd ready              # Find available work
bd show <id>          # View issue details
bd update <id> --claim  # Claim work
bd close <id>         # Complete work
```

### Rules

- Use `bd` for ALL task tracking — do NOT use TodoWrite, TaskCreate, or markdown TODO lists
- Run `bd prime` for detailed command reference and session close protocol
- Use `bd remember` for persistent knowledge — do NOT use MEMORY.md files

**Architecture in one line:** issues live in a local Dolt DB; sync uses `refs/dolt/data` on your git remote; `.beads/issues.jsonl` is a passive export. See https://github.com/gastownhall/beads/blob/main/docs/SYNC_CONCEPTS.md for details and anti-patterns.

## Session Completion

**This project has no git remote** — beads data and code both live locally.
"Done" means the branch is merged into `main` in the coordination checkout, not
pushed anywhere. Do NOT run `git push` / `git pull --rebase`.

**When ending a work session**, complete ALL steps below. Work is NOT complete
until the branch is merged into `main`.

1. **File issues for remaining work** - Create issues for anything that needs follow-up
2. **Run quality gates** (if code changed) - `cargo fmt`, `cargo test`
3. **Update issue status** - Close finished work, update in-progress items
4. **INTEGRATE INTO MAIN** - from the coordination checkout, not the worktree:
   ```bash
   cd /home/takashi/Projects/slate
   git merge --no-ff work/<id>
   cargo fmt && cargo test
   bd close <id> --reason "..."
   ```
5. **Clean up** - `git worktree remove ../slate-<id>`, clear stashes
6. **Verify** - All changes committed and merged; `git status` clean
7. **Hand off** - Provide context for next session

**CRITICAL RULES:**

- Work is NOT complete until the branch is merged into `main`
- NEVER leave work stranded on an unmerged worktree branch
- There is no remote: do NOT `git push` or `git pull --rebase`
- Resolve merge conflicts in the coordination checkout and rerun tests
<!-- END BEADS INTEGRATION -->
