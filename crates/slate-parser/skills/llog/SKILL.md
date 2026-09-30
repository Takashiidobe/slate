---
name: llog
description: Use the `llog` CLI to read, search, or write to a local markdown work-log wiki. Triggers include "llog", "search the wiki", "log this decision", "add a concept page", "start a wiki for this project", "review the wiki/logs", or any request to record or look up project decisions/context that isn't in the code or git history.
---

# llog

`llog` is a local, offline work-log wiki with full-text search, built for
recording decisions and context that don't live in the code itself (design
rationale, dead ends, why a tradeoff was made a certain way). It has no web
clipper, image handling, or sync — just markdown files plus a local search
index.

Binary: `llog` (this repo). Build with `cargo build --release`, or use
`cargo run --` during development of the tool itself.

## Layout

All commands operate relative to the current working directory:

```
wiki/
  index.md          # auto-maintained links to every concept page
  concepts/*.md      # one file per durable concept/decision
  log/*.md           # one file per log entry, named by timestamp
  .reviewed          # marker: last log entry processed by `llog review`
.llog/               # local search index + mtime cache — gitignored, derived
```

Concept pages are for durable, reusable knowledge ("how X works", "why we
chose Y"). Log entries are for point-in-time notes (what happened, what was
decided, in the moment). If unsure which one fits, prefer a log entry — it's
cheaper and searchable just the same.

## Commands

**Start a new project's wiki:**

```
llog init
```

Scaffolds `wiki/{concepts,log}/` and `wiki/index.md`, and builds the (empty)
`.llog/` search index. Run this once per project, from the project root,
before using any other command. Fails loudly if `wiki/` already exists
rather than touching it — safe to run speculatively to check.

**Create a concept page:**

```
llog new "<title>"
```

Scaffolds `wiki/concepts/<slug>.md` with a `# <title>` heading and links it
from `wiki/index.md` automatically. Fails if the slug already exists — edit
the existing file directly instead of creating a near-duplicate.

**Add a log entry:**

```
llog log "<title>" "<entry text>" [--date "<when>"]
```

Creates a new file `wiki/log/YYYY-MM-DD-HH-MM.md` (collision-suffixed if two
entries land in the same minute) with the title as an `# ` heading, a
timestamp line, and the entry body. Every call creates a new file — never
edit or append to past log entries.

`--date` backdates the entry (both the filename and the timestamp line) to a
specific moment instead of now — accepts `"YYYY-MM-DD"`, `"YYYY-MM-DD HH:MM"`,
or `"YYYY-MM-DD HH:MM:SS"`. This is the mechanism for backfilling history:
when mining old commits for decisions worth recording, pass the commit's
actual date/time so entries stay in true chronological order in `wiki/log/`
rather than clustering under today's date.

**Search:**

```
llog search "<query>" [--limit N] [--plain]     # default limit: 3
```

Lazily reindexes any changed/new/deleted `.md` file under `wiki/` (no
separate index step needed) and prints the matched file paths in ranked
order. `index.md` itself is excluded from results since it's pure
navigation, not content.

What happens after the path list depends on where stdout is going:

- **Real terminal (you, interactively):** `llog search` also opens all
  matched files in `nvim` as an arglist. Each file opens with the cursor
  already jumped to the first match of the query terms (case-insensitive,
  whole terms OR'd together); `:n` / `:N` step through the arglist and the
  cursor re-jumps to the first match in each new file automatically.
- **Agent / pipe / non-tty (an LLM running this command):** nvim never
  launches — there's nothing for it to attach to. You get exactly the plain
  ranked path list and nothing else; read the files with your normal file
  tools from there. Pass `--plain` to force this behavior even on a real
  terminal (useful for scripting).

Ranking: titles are boosted over body text, so a query matching a concept's
heading usually outranks one that only matches somewhere in the body. Log
entries (`log/*.md`) additionally decay with age (90-day half-life, based on
the entry's own timestamp — not filesystem mtime, so a `--date`-backdated
entry decays as if it were actually written then). Concept pages
(`concepts/*.md`) are **not** decayed — they're meant to be evergreen, kept
current by editing in place, so their rank should reflect relevance, not
recency.

**Review the log for concept-worthy material:**

```
llog review [--mark "<path>"]
```

`llog review` (no args) lists log entries created since the last review,
oldest first — the raw material for deciding whether something should be
promoted into a concept page. It's read-only: it doesn't change anything.
`llog review --mark "wiki/log/<file>.md"` advances the "reviewed" marker
(`wiki/.reviewed`) through that entry, so it won't be listed again. Only mark
after actually acting on the batch (creating/updating concept pages as
needed) — if that work gets interrupted, leaving the marker unmoved means
the same entries surface again next time instead of being silently skipped.

**Check for dangling links:**

```
llog check
```

Walks every `.md` file under `wiki/`, extracts markdown links (`[text](target)`),
and verifies each local target resolves to a real file. Targets are resolved
relative to the linking file's directory, same as a markdown renderer would
(a leading `/` resolves relative to `wiki/` instead). External links
(`http(s)://`, `mailto:`) and same-page anchors (`#section`) are skipped.
Prints each dangling link as `<file>: <target>` and exits non-zero if any are
found; prints `no dangling links` and exits 0 otherwise.

## When to reach for it

- Before starting work on something that feels like it's been decided before,
  `llog search` for it — don't rediscover a decision that's already recorded.
- When you land on a non-obvious decision, root cause, or rationale during a
  task, record it (see the `llog-wiki-maintain` skill for judgment on what's
  worth recording and how).
- Periodically (or when asked to "review the wiki"/"promote logs"), run
  `llog review` and use its output to decide what belongs in a concept page —
  see `llog-wiki-maintain` for the promotion workflow.
- After renaming or deleting a concept/log file, or editing links by hand, run
  `llog check` to catch dangling links before they rot.
