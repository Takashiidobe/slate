# Multi-Agent Worktree Workflow

Use this when several agents may work on Slate at the same time. Keep it focused
on coordination; general beads, build, and test rules live in `AGENTS.md` and
`CLAUDE.md`.

## Pick Work From Beads

Before choosing work, inspect both ready and active tickets:

```bash
bd prime
bd ready
bd list --status=in_progress
bd list --status=open
```

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

## Keep Beads Mutations Centralized

Avoid `.beads/` conflicts by keeping mutations in one place:

- coordination checkout: claim, close, reopen, defer, and create follow-up beads
- worktree branches: code, tests, docs, and read-only bead inspection

Create or update follow-up beads from the coordination checkout before
integrating the branch.

## Avoid Parallel Hotspots

Good parallel tickets usually add isolated fixtures or header coverage under
`tests/stdlib/...`. Run only one ticket at a time for these shared files unless a
human explicitly partitions the work:

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
