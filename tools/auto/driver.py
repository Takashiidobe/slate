#!/usr/bin/env python3
import argparse
import json
import os
import subprocess
import signal
import sys
import threading
import time
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
LEASE_PY = ROOT / "tools" / "auto" / "lease.py"
LOGS = ROOT / ".auto" / "logs"
TRANSCRIPT = ROOT / ".auto" / "transcript.md"
LIMITS = ROOT / ".auto" / "limits.json"


def sh(*args, check=False):
    return subprocess.run(args, cwd=ROOT, capture_output=True, text=True, check=check)


def lease(*args):
    return sh(sys.executable, str(LEASE_PY), *args)


def read_lease():
    return json.loads((ROOT / ".auto" / "lease.json").read_text())


def tree_dirty():
    lines = sh("git", "status", "--porcelain", "--untracked-files=normal").stdout.splitlines()
    return any(not line.endswith(".beads/issues.jsonl") for line in lines)


def rescue(ticket, dry_run, limited):
    note = "[auto] session ended while holding this ticket"
    if limited:
        note += " (5-hour usage limit reached; returned to the queue, not a judgment call)"
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
    status = "open" if dry_run or limited else "needs_input"
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
    if args.compact_window:
        env["CLAUDE_CODE_AUTO_COMPACT_WINDOW"] = str(args.compact_window)
    print(f"session {n}: log {log}; transcript {TRANSCRIPT}", flush=True)
    limited = False
    with log.open("w") as raw, TRANSCRIPT.open("a") as tx:
        tx.write(f"\n\n# session {n} · {time.strftime('%Y-%m-%d %H:%M:%S')}\n")
        tx.flush()
        proc = subprocess.Popen(cmd, cwd=ROOT, env=env, stdout=subprocess.PIPE, stderr=subprocess.STDOUT, text=True)
        timer = threading.Timer(remaining_s, proc.kill)
        timer.start()
        for line in proc.stdout:
            raw.write(line)
            try:
                event = json.loads(line)
            except json.JSONDecodeError:
                tx.write(line)
                continue
            if event.get("type") == "rate_limit_event" and record_limit(event["rate_limit_info"], args.limit_stop):
                limited = True
                tx.write(f"\n[{time.strftime('%H:%M:%S')}] 5-hour limit reached: killing session\n")
                proc.kill()
            if event.get("type") == "result" and event.get("is_error") and "limit" in str(event.get("result", "")).lower():
                limited = True
            text = render(event)
            if text:
                tx.write(text)
                tx.flush()
        code = proc.wait()
        timed_out = not timer.is_alive()
        timer.cancel()
    return ("timeout" if timed_out else code), limited


def record_limit(info, stop_at):
    window = info.get("unifiedWindows", {}).get("five_hour", {})
    utilization = window.get("utilization", info.get("utilization"))
    LIMITS.write_text(json.dumps({
        "status": info.get("status"),
        "utilization": utilization,
        "resets_at": window.get("resetsAt", info.get("resetsAt")),
        "stop_at": stop_at,
    }))
    return info.get("status") == "rejected"


def near_limit():
    try:
        limits = json.loads(LIMITS.read_text())
    except (FileNotFoundError, json.JSONDecodeError):
        return False
    if limits["resets_at"] and limits["resets_at"] < time.time():
        return False
    return limits["status"] == "rejected" or (limits["utilization"] or 0) >= limits["stop_at"]


def clip(text, limit):
    text = text.strip()
    return text if len(text) <= limit else text[:limit] + f" … [{len(text) - limit} more chars]"


def render(event):
    stamp = time.strftime("%H:%M:%S")
    kind = event.get("type")
    if kind == "assistant":
        out = []
        for part in event["message"].get("content", []):
            if part.get("type") == "text" and part["text"].strip():
                out.append(f"\n[{stamp}] {part['text'].strip()}\n")
            elif part.get("type") == "tool_use":
                args = part.get("input", {})
                shown = args.get("command") or args.get("file_path") or args.get("pattern") or json.dumps(args)
                out.append(f"[{stamp}] → {part['name']}: {clip(shown, 400)}\n")
        return "".join(out)
    if kind == "user":
        content = event["message"].get("content")
        out = []
        for part in content if isinstance(content, list) else []:
            if part.get("type") == "tool_result":
                body = part.get("content")
                if isinstance(body, list):
                    body = "\n".join(p.get("text", "") for p in body if isinstance(p, dict))
                mark = "✗" if part.get("is_error") else "←"
                out.append("    " + clip(str(body or ""), 300).replace("\n", "\n    ") + f"  {mark}\n")
        return "".join(out)
    if kind == "result":
        return f"\n[{stamp}] session result: {event.get('subtype')} · ${event.get('total_cost_usd', 0):.2f}\n"
    return ""


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("--hours", type=float, default=5)
    parser.add_argument("--per-session", type=int, default=4)
    parser.add_argument("--max-sessions", type=int, default=0)
    parser.add_argument("--dry-run", action="store_true")
    parser.add_argument("--model")
    parser.add_argument("--compact-window", type=int, default=250000)
    parser.add_argument("--limit-stop", type=float, default=0.9)
    args = parser.parse_args()

    if sh("git", "branch", "--show-current").stdout.strip() != "main":
        sys.exit("run from main")
    if tree_dirty():
        sys.exit("working tree is dirty; commit or stash first")

    start_args = ["start-run", "--hours", str(args.hours), "--per-session", str(args.per_session)]
    if args.dry_run:
        start_args.append("--dry-run")
    LIMITS.unlink(missing_ok=True)
    start_commit = lease(*start_args).stdout.strip()
    deadline = time.time() + args.hours * 3600
    print(f"run start commit {start_commit}; review with: git log {start_commit[:12]}..main", flush=True)
    awake = subprocess.Popen(
        ["fish", "-c", f"caffeinate {int(args.hours * 3600) + 600}s"],
        stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL, start_new_session=True,
    )
    try:
        loop(args, deadline)
    finally:
        os.killpg(awake.pid, signal.SIGTERM)
    history = read_lease()["history"]
    for row in history:
        print(f"  {row['ticket']}\t{row['outcome']}\t{row['minutes']}m")
    print(f"done: {len(history)} tickets; git log {start_commit[:12]}..main", flush=True)


def loop(args, deadline):
    n = 0
    while time.time() < deadline - 15 * 60:
        if args.max_sessions and n >= args.max_sessions:
            break
        if near_limit():
            print("5-hour usage limit reached or nearly used; stopping", flush=True)
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
            rescue(held, args.dry_run, limited)
        print(f"session {n} exit {code}{' (usage limit)' if limited else ''}", flush=True)
        if limited:
            break


if __name__ == "__main__":
    main()
