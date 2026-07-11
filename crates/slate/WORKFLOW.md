# Multi-Agent Worktree Workflow

This workflow is for running several agents against Slate without creating
unnecessary merge conflicts or duplicating beads work.

## Start From Beads, Not Git Branches

Before choosing work, every agent must inspect both ready and active tickets:

```bash
bd prime
bd ready
bd list --status=in_progress
bd list --status=open
```

Do not pick a ticket only because it appears in `bd ready`. A ticket can be
ready but still be linked to active work through a parent epic, child ticket,
dependency, dependent ticket, or obvious shared implementation area.

For each candidate, inspect its linked set:

```bash
bd show <id>
bd graph <id>
bd children <id>
```

If the ticket has a parent, inspect the parent and sibling children too:

```bash
bd show <parent-id>
bd children <parent-id>
```

Do not start a ticket when any item in its linked set is already
`in_progress`. The linked set includes:

- the ticket itself
- parent epics and child tickets
- dependency prerequisites
- dependent tickets blocked by this one
- sibling children under the same parent epic
- tickets that clearly touch the same core files or feature surface

Do not implement against an epic ticket directly. Pick or create a child ticket
with concrete acceptance criteria.

## Claim Before Creating A Worktree

Claim work from the coordination checkout, then create one branch and worktree
per claimed bead:

```bash
bd update <id> --claim
git worktree add ../slate-<id> -b work/<id> main
cd ../slate-<id>
```

Use the bead id in the branch and directory name. Keep one bead per branch unless
the ticket explicitly says otherwise.

## Keep Beads Mutations Centralized

Beads state is tracked in this repository through `.beads/issues.jsonl` and
`.beads/interactions.jsonl`. Multiple branches editing those files independently
will create noisy conflicts.

Use this rule:

- coordination checkout: claim, close, reopen, defer, and create follow-up beads
- worktree branches: code, tests, docs, and local read-only bead inspection

If a worker discovers required follow-up work, create or update the bead from the
coordination checkout before integrating the branch.

## Choose Parallel Work By Conflict Risk

Good parallel tickets usually add isolated fixtures or header coverage under
`tests/stdlib/...`.

Risky parallel tickets touch shared lowering or parser internals. Run only one
of these at a time unless a human explicitly partitions the files:

- `src/lower.rs`
- `src/c_ast.rs`
- `src/cir/parse.rs`
- `src/cir/ir.rs`
- `tests/support/cgen.rs`
- `docs/README.md`
- `c.bnf`

Do not run broad language-feature tickets concurrently when they all need the
same lowerer code. Examples: globals, aggregates, operators, prototypes,
switch, and goto.

## Worktree Rules

In each worktree:

```bash
git status --short --branch
bd show <id>
cargo fmt
cargo test
```

Use narrower tests while developing, but run the ticket's acceptance tests before
handoff. For fixture work, the usual checks are:

```bash
cargo test --test differential generated_differential
cargo test --test stdlib_coverage
```

Do not leave generated, temporary, or unrelated files in the branch. Do not
rewrite unrelated user changes.

## Integration

Integrate from the coordination checkout:

```bash
cd /home/takashi/Projects/slate
git status --short --branch
git merge --no-ff work/<id>
cargo fmt
cargo test
bd close <id> --reason "Implemented <summary>. Verified with <commands>."
```

If the merge conflicts, resolve it in the coordination checkout and rerun the
relevant tests. After a successful merge and close, check what became newly
available:

```bash
bd ready
bd list --status=in_progress
```

Remove finished worktrees only after their branches are merged:

```bash
git worktree remove ../slate-<id>
```

## When A Candidate Is Linked To Active Work

If `bd list --status=in_progress`, `bd graph`, or `bd children` shows linked
active work, do not claim the candidate. Pick an unrelated ready ticket instead.

If no unrelated ticket is available, file a coordination note or ask for a human
decision. Do not "just start" overlapping lowerer work; it causes semantic
conflicts even when Git can merge the text.
