---
name: llog-wiki-maintain
description: Keep the local `llog` work-log wiki (wiki/concepts, wiki/log, wiki/index.md) up to date as work happens, and periodically promote log entries into concept pages via `llog review`. Use proactively during any task where you make a non-obvious decision, find a root cause, rule out an approach, or change something a concept page already documents — not just when explicitly asked to "update the wiki" or "review the logs."
---

# Maintaining the llog wiki

This skill is about judgment: what's worth writing down, where it goes, and
when. It assumes the `llog` skill for the mechanical commands (`llog new`,
`llog log`, `llog search`).

The wiki exists to survive context loss between sessions. Optimize for what a
future session — with no memory of this conversation — would need to avoid
repeating work or re-litigating a settled decision.

## Before you start non-trivial work

Run `llog search "<topic>"` for the area you're about to touch. If a concept
page already exists, read it before making decisions that might contradict
it. Don't silently redo a decision that's already recorded — either follow
it, or if you're deliberately overturning it, say so explicitly and update
the concept page to reflect the new decision (see below).

## What to write, and where

**Log entry (`llog log "<title>" "<entry>"`)** — a point-in-time record. Use
it for:

- A decision made and why, including tradeoffs considered and rejected.
- A root cause found for a bug, especially if it wasn't obvious from the
  symptom.
- An approach that was tried and ruled out, and why — this is high-value and
  easy to lose, since it leaves no trace in the code.
- Anything a future session would otherwise have to re-derive by reading git
  blame or re-running the same investigation.

**Concept page (`llog new "<title>"`, then edit the file directly)** — a
durable reference. Use it for:

- How a subsystem or mechanism works, once it's stable enough to describe
  without hedging.
- A standing convention or invariant the codebase relies on.
- Something you expect to be asked "how does X work again?" about more than
  once.

If a concept page for the topic already exists, edit it in place rather than
creating a new one — check `llog search` first. Concept pages should reflect
current truth, not a changelog of how understanding evolved; that history
belongs in log entries, not in edits piling up in the concept page.

## What NOT to write

- Routine mechanical actions (ran the tests, fixed a typo, formatted a file)
  — these have no decision content and clutter search results.
- Anything already fully evident from reading the diff or commit message.
- Restating what the code does — write _why_, not _what_. If it needs no
  explanation beyond the diff, it doesn't need a log entry either.
- Speculative or unconfirmed claims. Only log what actually happened or was
  actually decided this session, not a plan or a guess.

## Writing style

- Log entry titles should be specific enough to be findable later: "target
  feature propagation via CIR op attrs", not "fixed bug".
- Write for a reader who has zero memory of this conversation — no "as
  discussed above" or "the fix we just made."
- Keep entries terse. A log entry is a pointer to a decision, not the full
  diff or transcript — link to the relevant file/commit instead of pasting
  code.

## When a concept page goes stale

If, during work, you discover a concept page describing something that's no
longer true (the mechanism it describes was replaced, the invariant no
longer holds), update it in the same session — don't leave it stale for a
future reader to be misled by. Note the change as a log entry too if the
reason for the change is itself worth recording.

## Periodic promotion pass: turning logs into concepts

Logging as-you-go (above) captures things in the moment, but log entries pile
up as isolated point-in-time notes. Periodically — or whenever asked to
"review the wiki" / "promote the logs" — run a batched pass to consolidate
them into durable concept pages:

1. `llog review` lists log entries created since the last pass, oldest first.
   If it says there's nothing pending, stop here.
2. Read through that batch. For each entry (or cluster of related entries),
   apply the same log-vs-concept judgment as above: does this describe
   something durable and reusable, not just a point-in-time note? If so:
   - `llog search "<topic>"` to check for an existing concept page on it.
   - If one exists, edit it in place to fold in the new information — don't
     leave the update to rot in the log entry.
   - If none exists and the material genuinely warrants a standing
     reference, `llog new "<title>"` and write it.
   - Related log entries across the batch often describe the same evolving
     concept from different angles — reading the whole batch before writing
     anything catches that; promoting one entry at a time misses it.
3. Most entries in a batch won't warrant promotion — a log entry recording a
   root cause or a rejected approach has already done its job just by
   existing and being searchable. Only promote what earns a durable page (see
   "What to write, and where" above). It's fine, and expected, for a batch to
   result in zero new/updated concept pages.
4. Once the batch has been fully considered (whether or not anything was
   promoted), `llog review --mark "wiki/log/<last-file>.md"` to advance the
   marker through the last entry you looked at. Only mark after finishing —
   if the pass gets interrupted partway, leave the marker where it was so the
   unfinished portion of the batch surfaces again next time instead of being
   silently skipped.
