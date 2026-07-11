# Multi-Agent Worktree Workflow

Use this when several agents may work on Slate at the same time. Keep it focused
on coordination; general beads, build, and test rules live in `AGENTS.md` and
`CLAUDE.md`.

## Pick Work From Beads

The SessionStart hook already ran `bd prime`; don't rerun it. Before choosing
work, inspect both ready and active tickets:

```bash
bd ready
bd list --status=in_progress
bd list --status=open
```

**Prefer parallel-safe tickets.** The cleanest work to pick up cold is a ticket
that only adds isolated fixtures — new probes under `tests/stdlib/<header>/` (the
`slate-nk3.*` epic) or new C fixtures under `tests/fixtures/`. These don't touch
the lowerer, so they merge without conflicts even while other agents are active.
See [docs/adding-features.md](docs/adding-features.md) for the stdlib-probe
recipe. Reach for lowerer-heavy tickets only when no isolated-fixture work is
ready.

`bd ready` is not enough. A ready ticket can still overlap active work through
linked beads or shared implementation areas.

For each candidate, inspect its linked set:

```bash
bd show <id>
bd graph <id>
bd children <id>
```

If the ticket has a parent, inspect the parent and sibling children too.

Do not start when any linked item is already `in_progress`. Linked items include
the ticket, parent epics, child tickets, prerequisites, dependents, siblings, and
tickets that clearly touch the same core files or feature surface.

To judge file-level overlap fast, read each `in_progress` ticket's notes for its
recorded touched files (see [Claim Before Branching](#claim-before-branching));
fall back to the description only when notes are absent.

Do not implement against an epic directly; pick or create a concrete child
ticket.

## Claim Before Branching

From the coordination checkout:

```bash
bd update <id> --claim
git worktree add ../slate-<id> -b work/<id> main
cd ../slate-<id>
```

Use the bead id in the branch and directory name. Keep one bead per branch unless
the ticket explicitly says otherwise.

When you claim, record the files/hotspots you expect to touch so other agents can
detect overlap without reading prose:

```bash
bd update <id> --notes "touches: src/lower.rs, tests/stdlib/ctype/"
```

## Keep Beads Mutations Centralized

Avoid `.beads/` conflicts by keeping mutations in one place:

- coordination checkout: claim, close, reopen, defer, and create follow-up beads
- worktree branches: code, tests, docs, and read-only bead inspection

Create or update follow-up beads from the coordination checkout before
integrating the branch.

Note: a bd hook exports `.beads/issues.jsonl` on commit, so committing in a
worktree can pull a modified `.beads/issues.jsonl` into your branch — the exact
churn this section avoids. Stage only your fixture/code paths in worktree commits
(`git add tests/... src/...`, never `git add -A`) and let the coordination
checkout own `.beads/`.

## Avoid Parallel Hotspots

Run only one ticket at a time against these shared files unless a human explicitly
partitions the work:

- `src/lower.rs`
- `src/c_ast.rs`
- `src/cir/parse.rs`
- `src/cir/ir.rs`
- `tests/support/cgen.rs`
- `docs/README.md`
- `c.bnf`

Do not run broad language-feature tickets concurrently when they need the same
lowerer code: globals, aggregates, operators, prototypes, switch, goto.

## Worktree Rules

Start by checking scope:

```bash
git status --short --branch
bd show <id>
```

Use narrow tests while developing. Before handoff, run the ticket's acceptance
tests. For fixture work, usually run:

```bash
cargo test --test differential generated_differential
cargo test --test stdlib_coverage
```

Run `cargo fmt` before handoff. Leave no generated, temporary, or unrelated
files in the branch. Do not rewrite unrelated user changes.

## Integrate From Coordination Checkout

```bash
cd /home/takashi/Projects/slate
git status --short --branch
git merge --no-ff work/<id>
cargo fmt
cargo test
bd close <id> --reason "Implemented <summary>. Verified with <commands>."
bd ready
bd list --status=in_progress
```

Resolve conflicts in the coordination checkout and rerun relevant tests. Remove
finished worktrees only after their branches are merged:

```bash
git worktree remove ../slate-<id>
```

## If Active Work Overlaps

If a candidate is linked to active work, do not claim it. Pick an unrelated ready
ticket instead. If none is available, file a coordination note or ask for a human
decision.
