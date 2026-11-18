#!/usr/bin/env python3
import argparse
import json
import os
import subprocess
import sys
import time
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
LEASE = ROOT / ".auto" / "lease.json"
TICKET_MINUTES = 15
DRY_LABEL = "auto-dry"


def load():
    try:
        return json.loads(LEASE.read_text())
    except (FileNotFoundError, json.JSONDecodeError):
        return None


def save(lease):
    LEASE.parent.mkdir(exist_ok=True)
    tmp = LEASE.with_suffix(".tmp")
    tmp.write_text(json.dumps(lease, indent=2))
    tmp.replace(LEASE)


def minutes_since(stamp):
    return (time.time() - stamp) / 60


def run_left(lease):
    return lease["run_minutes"] - minutes_since(lease["run_start"])


def bd(*args):
    return subprocess.run(["bd", *args], cwd=ROOT, capture_output=True, text=True)


def status_line(lease):
    parts = []
    if lease.get("ticket"):
        used = minutes_since(lease["ticket_start"])
        parts.append(f"ticket {lease['ticket']} {used:.0f}m/{TICKET_MINUTES}m")
    used_run = minutes_since(lease["run_start"])
    parts.append(f"run {used_run / 60:.1f}h/{lease['run_minutes'] / 60:.1f}h")
    parts.append(f"session ticket {lease['session_tickets']}/{lease['per_session']}")
    if lease["dry_run"]:
        parts.append("DRY RUN")
    return " · ".join(parts)


def cmd_start_run(args):
    head = subprocess.run(["git", "rev-parse", "HEAD"], cwd=ROOT, capture_output=True, text=True).stdout.strip()
    save({
        "run_start": time.time(),
        "run_minutes": args.hours * 60,
        "per_session": args.per_session,
        "dry_run": args.dry_run,
        "start_commit": head,
        "session_tickets": 0,
        "ticket": None,
        "ticket_start": None,
        "history": [],
    })
    print(head)


def cmd_new_session(_args):
    lease = load()
    lease["session_tickets"] = 0
    save(lease)


def cmd_next(_args):
    lease = load()
    if lease is None:
        sys.exit("no lease: the run was not started by tools/auto/driver.py")
    if lease.get("ticket"):
        print(f"still holding {lease['ticket']}: finish it with `lease.py done <outcome>` first")
        sys.exit(5)
    if lease["session_tickets"] >= lease["per_session"]:
        print("EXIT: session ticket quota reached")
        sys.exit(3)
    if run_left(lease) <= TICKET_MINUTES:
        print("EXIT: run budget exhausted")
        sys.exit(3)
    query = ["ready", "--label", "auto", "--exclude-type", "epic", "--claim", "--json"]
    if lease["dry_run"]:
        query += ["--exclude-label", DRY_LABEL]
    out = bd(*query)
    if out.returncode != 0:
        sys.exit(f"bd ready failed: {out.stderr.strip()}")
    claimed = json.loads(out.stdout or "null")
    if isinstance(claimed, list):
        claimed = claimed[0] if claimed else None
    if not claimed:
        print("EXIT: queue empty")
        sys.exit(4)
    lease["ticket"] = claimed["id"]
    lease["ticket_start"] = time.time()
    lease["session_tickets"] += 1
    save(lease)
    print(f"claimed {claimed['id']}: {claimed['title']}")
    print(status_line(lease))


def cmd_done(args):
    lease = load()
    ticket = lease.get("ticket")
    if not ticket:
        sys.exit("no ticket held")
    if lease["dry_run"]:
        bd("update", ticket, "--add-label", DRY_LABEL, "--status", "open", "--assignee", "")
    lease["history"].append({
        "ticket": ticket,
        "outcome": args.outcome,
        "minutes": round(minutes_since(lease["ticket_start"]), 1),
    })
    lease["ticket"] = None
    lease["ticket_start"] = None
    save(lease)
    stop = (
        args.outcome in ("needs_input", "wip")
        or lease["session_tickets"] >= lease["per_session"]
        or run_left(lease) <= TICKET_MINUTES
    )
    print("EXIT: end this session now" if stop else "CONTINUE: run `lease.py next`")


def cmd_check(_args):
    lease = load()
    if lease is None:
        return
    print(status_line(lease))
    if lease.get("ticket") and minutes_since(lease["ticket_start"]) > TICKET_MINUTES:
        sys.exit(2)


def cmd_hook(_args):
    if os.environ.get("SLATE_AUTO") != "1":
        return
    lease = load()
    if lease is None or not lease.get("ticket"):
        return
    line = status_line(lease)
    if minutes_since(lease["ticket_start"]) > TICKET_MINUTES:
        line += (
            " · OVER BUDGET: stop now. Save work to the <id>-wip branch, comment, set needs_input, "
            "`lease.py done wip`, and end the session (see auto-bead skill)."
        )
    elif minutes_since(lease["ticket_start"]) > TICKET_MINUTES - 3:
        line += " · under 3 minutes left: wrap up or prepare to file WIP."
    print(json.dumps({"hookSpecificOutput": {"hookEventName": "PostToolUse", "additionalContext": line}}))


def main():
    parser = argparse.ArgumentParser()
    sub = parser.add_subparsers(dest="cmd", required=True)
    start = sub.add_parser("start-run")
    start.add_argument("--hours", type=float, default=5)
    start.add_argument("--per-session", type=int, default=4)
    start.add_argument("--dry-run", action="store_true")
    start.set_defaults(func=cmd_start_run)
    sub.add_parser("new-session").set_defaults(func=cmd_new_session)
    sub.add_parser("next").set_defaults(func=cmd_next)
    done = sub.add_parser("done")
    done.add_argument("outcome", choices=["closed", "stale", "needs_input", "wip", "dry"])
    done.set_defaults(func=cmd_done)
    sub.add_parser("check").set_defaults(func=cmd_check)
    sub.add_parser("hook").set_defaults(func=cmd_hook)
    args = parser.parse_args()
    args.func(args)


if __name__ == "__main__":
    main()
