#!/usr/bin/env python3
import argparse
import json
import os
import subprocess
import sys
import time
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
LEASE_PY = ROOT / "tools" / "auto" / "lease.py"
LOGS = ROOT / ".auto" / "logs"


def sh(*args, check=False):
    return subprocess.run(args, cwd=ROOT, capture_output=True, text=True, check=check)


def lease(*args):
    return sh(sys.executable, str(LEASE_PY), *args)


def read_lease():
    return json.loads((ROOT / ".auto" / "lease.json").read_text())


def tree_dirty():
    lines = sh("git", "status", "--porcelain", "--untracked-files=normal").stdout.splitlines()
    return any(not line.endswith(".beads/issues.jsonl") for line in lines)


def rescue(ticket, dry_run):
    note = "[auto] session ended while holding this ticket"
    if tree_dirty() and not dry_run:
        branch = f"{ticket}-wip"
        base = sh("git", "rev-parse", "--short", "HEAD").stdout.strip()
        sh("git", "switch", "-c", branch)
        sh("git", "add", "-A")
        sh("git", "commit", "-m", f"WIP {ticket}: rescued by driver after session ended")
        sh("git", "switch", "main")
        note += f"; uncommitted work saved to branch {branch} (base {base}), unreviewed"
    elif tree_dirty():
        sh("git", "stash", "push", "-u", "-m", f"auto dry-run leftovers {ticket}")
    sh("bd", "comments", "add", ticket, note)
    status = "open" if dry_run else "needs_input"
    sh("bd", "update", ticket, "--status", status, "--assignee", "")
    lease("done", "dry" if dry_run else "wip")


def run_session(n, args, remaining_s):
    LOGS.mkdir(parents=True, exist_ok=True)
    log = LOGS / f"{time.strftime('%Y%m%d-%H%M%S')}-s{n}.jsonl"
    prompt = "/auto-bead dry-run" if args.dry_run else "/auto-bead"
    cmd = [
        "claude", "-p", prompt,
        "--permission-mode", "bypassPermissions",
        "--output-format", "stream-json", "--verbose",
    ]
    if args.model:
        cmd += ["--model", args.model]
    env = dict(os.environ, SLATE_AUTO="1")
    print(f"session {n}: log {log}", flush=True)
    with log.open("w") as out:
        try:
            proc = subprocess.run(cmd, cwd=ROOT, env=env, stdout=out, stderr=subprocess.STDOUT, timeout=remaining_s)
            code = proc.returncode
        except subprocess.TimeoutExpired:
            code = "timeout"
    tail = log.read_text()[-4000:].lower()
    limited = "usage limit" in tail or "rate limit" in tail or "limit reached" in tail
    return code, limited


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("--hours", type=float, default=5)
    parser.add_argument("--per-session", type=int, default=4)
    parser.add_argument("--max-sessions", type=int, default=0)
    parser.add_argument("--dry-run", action="store_true")
    parser.add_argument("--model")
    args = parser.parse_args()

    if sh("git", "branch", "--show-current").stdout.strip() != "main":
        sys.exit("run from main")
    if tree_dirty():
        sys.exit("working tree is dirty; commit or stash first")

    start_args = ["start-run", "--hours", str(args.hours), "--per-session", str(args.per_session)]
    if args.dry_run:
        start_args.append("--dry-run")
    start_commit = lease(*start_args).stdout.strip()
    deadline = time.time() + args.hours * 3600
    print(f"run start commit {start_commit}; review with: git log {start_commit[:12]}..main", flush=True)

    n = 0
    while time.time() < deadline - 15 * 60:
        if args.max_sessions and n >= args.max_sessions:
            break
        query = ["bd", "ready", "--label", "auto", "--exclude-type", "epic", "--json", "-n", "1"]
        if args.dry_run:
            query += ["--exclude-label", "auto-dry"]
        if not json.loads(sh(*query).stdout or "[]"):
            print("queue empty", flush=True)
            break
        n += 1
        lease("new-session")
        code, limited = run_session(n, args, deadline - time.time())
        held = read_lease().get("ticket")
        if held:
            rescue(held, args.dry_run)
        print(f"session {n} exit {code}{' (usage limit)' if limited else ''}", flush=True)
        if limited:
            break

    history = read_lease()["history"]
    for row in history:
        print(f"  {row['ticket']}\t{row['outcome']}\t{row['minutes']}m")
    print(f"done: {len(history)} tickets in {n} sessions; git log {start_commit[:12]}..main", flush=True)


if __name__ == "__main__":
    main()
