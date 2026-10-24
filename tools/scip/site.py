#!/usr/bin/env python3
import html
import json
import re
import sqlite3
import subprocess
import sys
from collections import Counter, defaultdict
from pathlib import Path

try:
    from pygments import lex
    from pygments.lexers import RustLexer
    from pygments.token import Comment, Keyword, Name, Number, String
except ImportError:
    lex = None

ROOT = Path(__file__).resolve().parents[2]
DB = ROOT / "target" / "scip" / "index.db"
OUT = ROOT / ".scip" / "site"

FUNCTION_KINDS = {"Function", "Method", "StaticMethod", "AbstractMethod", "TraitMethod"}


def connect():
    if not DB.exists():
        sys.exit(f"missing {DB}; run: python3 tools/scip/scip.py build")
    con = sqlite3.connect(DB)
    columns = {row[1] for row in con.execute("PRAGMA table_info(occurrences)")}
    if "end_col" not in columns:
        sys.exit(f"{DB} predates end_col; run: python3 tools/scip/scip.py build")
    return con


def page_name(path):
    return path.replace("/", "__") + ".html"


def crate_dir(path):
    parts = path.split("/")
    return "/".join(parts[:2]) if parts[0] == "crates" and len(parts) > 2 else parts[0]


IMPL_OWNER = re.compile(r"impl#\[`?([^\]`<]+)")


def label(symbol, display, is_function):
    if not is_function:
        return display
    descriptor = symbol.rsplit("/", 1)[-1]
    owner = IMPL_OWNER.match(descriptor)
    if owner:
        return f"{owner.group(1)}::{display}"
    if "#" in descriptor:
        return f"{descriptor.split('#', 1)[0]}::{display}"
    return display


class Index:
    def __init__(self, con):
        self.meta = dict(con.execute("SELECT key, value FROM meta"))
        self.files = [
            path for _, path in con.execute("SELECT id, path FROM files ORDER BY path")
            if (ROOT / path).exists()
        ]
        file_ids = {path: i for i, path in enumerate(self.files)}
        db_files = {fid: file_ids.get(path) for fid, path in con.execute("SELECT id, path FROM files")}

        self.syms, sym_ids = [], {}
        for sid, symbol, name, kind, fid, line in con.execute(
            "SELECT s.id, s.symbol, s.name, s.kind, o.file_id, MIN(o.line) FROM symbols s "
            "JOIN occurrences o ON o.symbol_id = s.id AND o.is_definition = 1 "
            "GROUP BY s.id ORDER BY s.id"
        ):
            if db_files.get(fid) is None:
                continue
            is_function = kind in FUNCTION_KINDS
            sym_ids[sid] = len(self.syms)
            self.syms.append([symbol, label(symbol, name, is_function), db_files[fid], line, int(is_function)])

        self.refs = [[] for _ in self.syms]
        self.occurrences = defaultdict(list)
        calls = set()
        for sid, fid, line, col, end_col, is_definition, enclosing in con.execute(
            "SELECT symbol_id, file_id, line, col, end_col, is_definition, enclosing_id "
            "FROM occurrences ORDER BY file_id, line, col"
        ):
            target, file = sym_ids.get(sid), db_files.get(fid)
            if target is None or file is None:
                continue
            if end_col is not None:
                self.occurrences[file].append((line, col - 1, end_col - 1, target, is_definition))
            if is_definition:
                continue
            caller = sym_ids.get(enclosing, -1)
            self.refs[target].append([file, line, caller])
            if caller >= 0 and self.syms[target][4]:
                calls.add((caller, target))
        self.calls = sorted(calls)

    def crate_of_sym(self, sym):
        return crate_dir(self.files[self.syms[sym][2]])

    def cross_crate(self):
        edges = defaultdict(Counter)
        for target, refs in enumerate(self.refs):
            to = self.crate_of_sym(target)
            for file, _, _ in refs:
                origin = crate_dir(self.files[file])
                if origin != to:
                    edges[(origin, to)][target] += 1
        return edges


def git(*args):
    return subprocess.run(["git", *args], cwd=ROOT, capture_output=True, text=True).stdout.strip()


KEYWORDS = {
    "as", "async", "await", "break", "const", "continue", "crate", "dyn", "else", "enum", "extern",
    "fn", "for", "if", "impl", "in", "let", "loop", "match", "mod", "move", "mut", "pub", "ref",
    "return", "self", "static", "struct", "super", "trait", "type", "unsafe", "use", "where", "while",
}
PRIMITIVES = {
    "i8", "i16", "i32", "i64", "i128", "isize", "u8", "u16", "u32", "u64", "u128", "usize",
    "f32", "f64", "bool", "char", "str",
}


def token_class(ttype, text):
    if text in PRIMITIVES:
        return "ty"
    if text in KEYWORDS:
        return "k"
    if ttype in Comment.Preproc:
        return "at"
    if ttype in Comment:
        return "cm"
    if ttype in String.Doc:
        return "cm"
    if ttype in String:
        return "st"
    if ttype in Number:
        return "nu"
    if ttype in Keyword.Type or ttype in Name.Builtin:
        return "ty"
    if ttype in Keyword.Pseudo:
        return None
    if ttype in Keyword:
        return "k"
    if ttype in Name.Function.Magic:
        return "mc"
    if ttype in Name.Attribute:
        return "lt"
    return None


def syntax_classes(source):
    classes = [None] * len(source)
    if lex is None:
        return classes
    tokens = list(lex(source, RustLexer(stripnl=False, ensurenl=False)))
    if sum(len(text) for _, text in tokens) != len(source):
        return classes
    pos = 0
    for i, (ttype, text) in enumerate(tokens):
        cls = token_class(ttype, text)
        if text == "'" and i + 1 < len(tokens) and tokens[i + 1][0] in Name.Attribute:
            cls = "lt"
        if cls:
            classes[pos : pos + len(text)] = [cls] * len(text)
        pos += len(text)
    return classes


def highlight(text, classes):
    out, i = [], 0
    while i < len(text):
        cls, j = classes[i], i
        while j < len(text) and classes[j] == cls:
            j += 1
        chunk = html.escape(text[i:j])
        out.append(f'<span class="t-{cls}">{chunk}</span>' if cls else chunk)
        i = j
    return "".join(out)


def render_file(source, occurrences):
    lines = source.split("\n")
    classes = syntax_classes(source)
    by_line = defaultdict(list)
    for line, start, end, sym, is_def in occurrences:
        by_line[line - 1].append((start, end, sym, is_def))

    rows = []
    offset = 0
    for number, text in enumerate(lines):
        line_classes = classes[offset : offset + len(text)]
        offset += len(text) + 1
        pieces, cursor = [], 0
        for start, end, sym, is_def in sorted(by_line.get(number, [])):
            if start < cursor or end > len(text):
                continue
            pieces.append(highlight(text[cursor:start], line_classes[cursor:start]))
            cls = "s d" if is_def else "s"
            pieces.append(
                f'<span class="{cls}" data-s="{sym}">'
                f"{highlight(text[start:end], line_classes[start:end])}</span>"
            )
            cursor = end
        pieces.append(highlight(text[cursor:], line_classes[cursor:]))
        rows.append(
            f'<tr id="L{number + 1}"><td class="n">{number + 1}</td><td class="c">{"".join(pieces)}</td></tr>'
        )
    return "\n".join(rows)


STYLE = """
:root { --bg:#fff; --fg:#1f2328; --mut:#6e7781; --acc:#0969da; --line:#d0d7de; --hl:#fff8c5; --pan:#f6f8fa;
        --k:#cf222e; --ty:#953800; --st:#0a3069; --nu:#0550ae; --cm:#6e7781; --at:#116329; --lt:#8250df; --mc:#8250df; --cross:#bc4c00; }
@media (prefers-color-scheme: dark) {
  :root { --bg:#0d1117; --fg:#e6edf3; --mut:#8b949e; --acc:#58a6ff; --line:#30363d; --hl:#3d3300; --pan:#161b22;
          --k:#ff7b72; --ty:#ffa657; --st:#a5d6ff; --nu:#79c0ff; --cm:#8b949e; --at:#7ee787; --lt:#d2a8ff; --mc:#d2a8ff; --cross:#f0883e; }
}
body { margin:0; background:var(--bg); color:var(--fg); font:14px/1.5 system-ui, sans-serif; }
a { color:var(--acc); text-decoration:none; } a:hover { text-decoration:underline; }
header { padding:8px 16px; border-bottom:1px solid var(--line); display:flex; gap:16px; align-items:baseline;
         position:sticky; top:0; z-index:10; background:var(--bg); }
tr { scroll-margin-top:var(--hh, 44px); }
main { display:flex; align-items:flex-start; }
main.closed aside { display:none; }
header button { margin-left:auto; font:inherit; padding:2px 10px; background:var(--pan); color:var(--fg);
                border:1px solid var(--line); border-radius:4px; cursor:pointer; }
.code { flex:1; min-width:0; overflow-x:auto; padding:8px 0; }
table { border-collapse:collapse; font:13px/1.45 ui-monospace, monospace; }
td.n { color:var(--mut); text-align:right; padding:0 12px; user-select:none; }
td.c { white-space:pre; padding-right:16px; }
tr.hl { background:var(--hl); }
.t-k { color:var(--k); } .t-ty { color:var(--ty); } .t-st { color:var(--st); } .t-nu { color:var(--nu); }
.t-cm { color:var(--cm); font-style:italic; } .t-at { color:var(--at); } .t-lt, .t-mc { color:var(--lt); }
.s { cursor:pointer; } .s:hover, .s.on { background:var(--hl); border-radius:2px; } .s.d { font-weight:600; }
aside { width:380px; flex:none; position:sticky; top:var(--hh, 44px); max-height:calc(100vh - var(--hh, 44px)); overflow:auto;
        padding:12px 16px; background:var(--pan); border-left:1px solid var(--line); }
aside h2 { font-size:13px; margin:12px 0 4px; } aside code { font-size:12px; word-break:break-all; }
aside ul { list-style:none; padding:0; margin:0; } aside li { padding:2px 0; font-size:13px; }
aside .caller { color:var(--mut); }
aside h3 { font-size:12px; margin:10px 0 2px; text-transform:uppercase; letter-spacing:.04em; color:var(--mut); }
aside h3.cross, .cross { color:var(--cross); }
.crates { padding:0 16px; } .crates table { font:13px/1.5 system-ui, sans-serif; } .crates td { padding:2px 12px 2px 0; vertical-align:top; }
h2.crate { font-size:15px; margin:20px 16px 0; }
.files { padding:16px; columns:2; } .files li { list-style:none; padding:2px 0; }
input { width:100%; box-sizing:border-box; padding:6px 8px; font-size:14px; background:var(--bg);
        color:var(--fg); border:1px solid var(--line); border-radius:4px; }
@media (max-width:800px) { main { flex-direction:column; } aside { width:auto; position:static; max-height:none; } }
"""

SCRIPT = """
const panel = document.getElementById('panel');
let current = null, depth = 2;
const callers = new Map(), callees = new Map();
for (const [a, b] of CALLS) {
  if (!callees.has(a)) callees.set(a, []);
  callees.get(a).push(b);
  if (!callers.has(b)) callers.set(b, []);
  callers.get(b).push(a);
}
function levels(start, adj, max) {
  const seen = new Set([start]);
  let frontier = [start];
  const out = [];
  for (let d = 1; d <= max && frontier.length; d++) {
    const next = [];
    for (const n of frontier)
      for (const m of adj.get(n) || []) if (!seen.has(m)) { seen.add(m); next.push(m); }
    if (next.length) out.push(next);
    frontier = next;
  }
  return out;
}
function callList(title, sym, adj) {
  const lv = levels(sym, adj, depth);
  let h = `<h2>${title}</h2>`;
  if (!lv.length) return h + '<p class="caller">none</p>';
  lv.forEach((nodes, i) => {
    h += `<div class="caller">depth ${i + 1}</div><ul>`;
    for (const n of nodes.sort((a, b) => NAMES[a].localeCompare(NAMES[b]))) {
      const d = DEFS[n];
      h += `<li><a href="${link(d[0], d[1])}">${esc(NAMES[n])}</a> <span class="caller">${esc(d[0])}</span></li>`;
    }
    h += '</ul>';
  });
  return h;
}
function callSection(sym) {
  const opts = [1, 2, 3, 4, 5].map(n => `<option${n === depth ? ' selected' : ''}>${n}</option>`).join('');
  return `<h2>Call graph <select id="depth">${opts}</select> ` +
    `<a href="graph.html?s=${encodeURIComponent(SYMS[sym][0])}&d=${depth}">open graph</a></h2>` +
    callList('Callers', sym, callers) + callList('Callees', sym, callees);
}
document.addEventListener('change', e => {
  if (e.target.id === 'depth') { depth = +e.target.value; show(current); }
});
const esc = s => s.replace(/[&<>"]/g, c => ({'&':'&amp;','<':'&lt;','>':'&gt;','"':'&quot;'}[c]));
const link = (p, l) => `${pageName(p)}#L${l}`;
function show(sym) {
  const d = DEFS[sym], r = REFS[sym], home = crateOf(d[0]);
  let h = `<h2>${esc(NAMES[sym])}</h2><code>${esc(SYMS[sym][0])}</code>`;
  h += `<h2>Definition</h2><ul><li><a href="${link(d[0], d[1])}">${esc(d[0])}:${d[1]}</a></li></ul>`;
  const byCrate = {};
  for (const [f, l, caller] of r) {
    const files = byCrate[crateOf(FILES[f])] ??= {};
    (files[FILES[f]] ??= []).push([l, caller]);
  }
  const outside = r.filter(([f]) => crateOf(FILES[f]) !== home).length;
  h += `<h2>References (${r.length}${outside ? `, ${outside} from other crates` : ''})</h2>`;
  const crates = Object.keys(byCrate).sort((a, b) => (a === home) - (b === home) || a.localeCompare(b));
  for (const c of crates) {
    const count = Object.values(byCrate[c]).reduce((n, v) => n + v.length, 0);
    h += `<h3 class="${c === home ? '' : 'cross'}">${esc(c)} (${count})</h3>`;
    for (const f of Object.keys(byCrate[c]).sort()) {
      h += `<div><b>${esc(f.slice(c.length + 1))}</b></div><ul>`;
      for (const [l, caller] of byCrate[c][f])
        h += `<li><a href="${link(f, l)}">line ${l}</a>${caller >= 0 ? ` <span class="caller">in ${esc(NAMES[caller])}</span>` : ''}</li>`;
      h += '</ul>';
    }
  }
  if (SYMS[sym][4]) h += callSection(sym);
  current = sym;
  panel.innerHTML = h;
  document.querySelectorAll('.s.on').forEach(e => e.classList.remove('on'));
  document.querySelectorAll('.s').forEach(e => { if (+e.dataset.s === sym) e.classList.add('on'); });
}
const main = document.querySelector('main');
document.documentElement.style.setProperty('--hh', document.querySelector('header').offsetHeight + 'px');
const setOpen = open => {
  main.classList.toggle('closed', !open);
  try { localStorage.setItem('panelOpen', open ? '1' : '0'); } catch (e) {}
};
document.getElementById('toggle').addEventListener('click', () => setOpen(main.classList.contains('closed')));
document.addEventListener('keydown', e => { if (e.key === 'Escape') setOpen(false); });
document.addEventListener('click', e => {
  const t = e.target.closest('.s');
  if (t) { show(+t.dataset.s); setOpen(true); }
});
try { if (localStorage.getItem('panelOpen') === '1') setOpen(true); } catch (e) {}
if (location.hash.startsWith('#L')) {
  const row = document.getElementById(location.hash.slice(1));
  if (row) { row.classList.add('hl'); row.scrollIntoView({block:'center'}); }
}
"""


GRAPH_HTML = """<!doctype html><meta charset="utf-8"><meta name="viewport" content="width=device-width,initial-scale=1">
<title>call graph</title><link rel="stylesheet" href="style.css">
<header><a href="index.html">index</a><b id="title"></b>
<label>depth <select id="d"><option>1</option><option>2</option><option>3</option><option>4</option></select></label></header>
<div id="msg" style="padding:16px"></div>
<div style="overflow:auto"><svg id="g" xmlns="http://www.w3.org/2000/svg"></svg></div>
<script src="data.js"></script><script src="graph.js"></script>"""

GRAPH_JS = """
const params = new URLSearchParams(location.search);
const root = SYMS.findIndex(s => s[0] === params.get('s'));
const maxDepth = +(params.get('d') || 2);
const NODE_W = 300, NODE_H = 24, COL_GAP = 90, ROW_GAP = 8;
const NS = 'http://www.w3.org/2000/svg';
const adjOut = new Map(), adjIn = new Map();
for (const [a, b] of CALLS) {
  if (!adjOut.has(a)) adjOut.set(a, []);
  adjOut.get(a).push(b);
  if (!adjIn.has(b)) adjIn.set(b, []);
  adjIn.get(b).push(a);
}
function column(adj, sign) {
  const col = new Map(), seen = new Set([root]);
  let frontier = [root];
  for (let d = 1; d <= maxDepth && frontier.length; d++) {
    const next = [];
    for (const n of frontier)
      for (const m of adj.get(n) || []) if (!seen.has(m)) { seen.add(m); col.set(m, sign * d); next.push(m); }
    frontier = next;
  }
  return col;
}
document.getElementById('d').value = String(maxDepth);
document.getElementById('d').addEventListener('change', e => {
  params.set('d', e.target.value);
  location.search = params.toString();
});
if (root < 0) {
  document.getElementById('msg').textContent = 'Pick a function from a code page (side panel, "open graph").';
} else {
  document.getElementById('title').textContent = NAMES[root];
  const cols = new Map([[root, 0], ...column(adjIn, -1), ...column(adjOut, 1)]);
  const byCol = new Map();
  for (const [n, c] of cols) (byCol.get(c) || byCol.set(c, []).get(c)).push(n);
  const keys = [...byCol.keys()].sort((a, b) => a - b);
  const minCol = keys[0];
  const rows = Math.max(...[...byCol.values()].map(v => v.length));
  const height = rows * (NODE_H + ROW_GAP) + 40;
  const width = keys.length * (NODE_W + COL_GAP) + 40;
  const pos = new Map();
  for (const c of keys) {
    const nodes = byCol.get(c).sort((a, b) => NAMES[a].localeCompare(NAMES[b]));
    const top = (height - nodes.length * (NODE_H + ROW_GAP)) / 2;
    nodes.forEach((n, i) => pos.set(n, { x: 20 + (c - minCol) * (NODE_W + COL_GAP), y: top + i * (NODE_H + ROW_GAP) }));
  }
  const svg = document.getElementById('g');
  svg.setAttribute('width', width);
  svg.setAttribute('height', height);
  const el = (tag, attrs, parent = svg) => {
    const e = document.createElementNS(NS, tag);
    for (const k in attrs) e.setAttribute(k, attrs[k]);
    parent.appendChild(e);
    return e;
  };
  const marker = el('marker', { id: 'arrow', viewBox: '0 0 10 10', refX: 9, refY: 5, markerWidth: 7, markerHeight: 7, orient: 'auto' });
  el('path', { d: 'M0 0L10 5L0 10z', fill: 'var(--mut)' }, marker);
  for (const [a, b] of CALLS) {
    if (!pos.has(a) || !pos.has(b) || a === b) continue;
    const p = pos.get(a), q = pos.get(b);
    if (q.x <= p.x) continue;
    const x1 = p.x + NODE_W, x2 = q.x;
    const y1 = p.y + NODE_H / 2, y2 = q.y + NODE_H / 2;
    const dx = Math.max(30, (x2 - x1) / 2);
    el('path', {
      d: `M${x1} ${y1}C${x1 + dx} ${y1} ${x2 - dx} ${y2} ${x2} ${y2}`,
      fill: 'none', stroke: 'var(--mut)', 'stroke-width': 1, 'marker-end': 'url(#arrow)',
    });
  }
  for (const [n, p] of pos) {
    const d = DEFS[n];
    const a = el('a', { href: `graph.html?s=${encodeURIComponent(SYMS[n][0])}&d=${maxDepth}` });
    const cross = crateOf(d[0]) !== crateOf(DEFS[root][0]);
    el('rect', { x: p.x, y: p.y, width: NODE_W, height: NODE_H, rx: 4,
      fill: n === root ? 'var(--hl)' : 'var(--pan)', stroke: cross ? 'var(--cross)' : 'var(--line)' }, a);
    const t = el('text', { x: p.x + 8, y: p.y + 16, 'font-size': 12, fill: 'var(--fg)',
      'font-family': 'ui-monospace, monospace' }, a);
    t.textContent = NAMES[n].length > 28 ? NAMES[n].slice(0, 27) + '…' : NAMES[n];
    const src = el('a', { href: `${pageName(d[0])}#L${d[1]}` });
    const s = el('text', { x: p.x + NODE_W - 8, y: p.y + 16, 'font-size': 11, 'text-anchor': 'end', fill: 'var(--acc)' }, src);
    s.textContent = d[0].split('/').pop().replace('.rs', '') + ':' + d[1];
    const tip = el('title', {}, a);
    tip.textContent = `${crateOf(d[0])}: ${SYMS[n][0]}`;
  }
}
"""




def cross_crate_html(index):
    rows = []
    for (origin, to), counts in sorted(index.cross_crate().items(), key=lambda e: -e[1].total()):
        top = ", ".join(
            f'<a href="{page_name(index.files[index.syms[sym][2]])}#L{index.syms[sym][3]}">'
            f"{html.escape(index.syms[sym][1])}</a> ({n})"
            for sym, n in counts.most_common(12)
        )
        rows.append(
            f"<tr><td>{html.escape(origin)} &rarr; {html.escape(to)}</td>"
            f"<td>{counts.total()} refs to {len(counts)} symbols</td><td>{top}</td></tr>"
        )
    if not rows:
        return ""
    return f'<h2 class="crate">Cross-crate references</h2><div class="crates"><table>{"".join(rows)}</table></div>'


def write_site(index):
    OUT.mkdir(parents=True, exist_ok=True)
    data = {"FILES": index.files, "SYMS": index.syms, "REFS": index.refs, "CALLS": index.calls}
    (OUT / "data.js").write_text(
        "".join(f"const {key} = {json.dumps(value, separators=(',', ':'))};\n" for key, value in data.items())
        + "const NAMES = SYMS.map(s => s[1]);\n"
        + "const DEFS = SYMS.map(s => [FILES[s[2]], s[3]]);\n"
        + "const pageName = p => p.replaceAll('/', '__') + '.html';\n"
        + "const crateOf = p => { const x = p.split('/'); return x[0] === 'crates' && x.length > 2 ? x[0] + '/' + x[1] : x[0]; };\n"
    )
    (OUT / "site.js").write_text(SCRIPT)
    (OUT / "graph.html").write_text(GRAPH_HTML)
    (OUT / "graph.js").write_text(GRAPH_JS)
    (OUT / "style.css").write_text(STYLE)

    for file, path in enumerate(index.files):
        body = render_file((ROOT / path).read_text(), index.occurrences[file])
        (OUT / page_name(path)).write_text(
            f"""<!doctype html><meta charset="utf-8"><meta name="viewport" content="width=device-width,initial-scale=1">
<title>{html.escape(path)}</title><link rel="stylesheet" href="style.css">
<header><a href="index.html">index</a><b>{html.escape(path)}</b>
<button id="toggle" title="Toggle the symbol panel (Esc closes)">panel</button></header>
<main class="closed"><div class="code"><table>{body}</table></div><aside id="panel"><p>Click an identifier.</p></aside></main>
<script src="data.js"></script><script src="site.js"></script>"""
        )

    by_crate = defaultdict(list)
    for path in index.files:
        by_crate[crate_dir(path)].append(path)
    sections = "".join(
        f'<h2 class="crate">{html.escape(crate)} ({len(paths)} files)</h2><ul class="files">'
        + "".join(
            f'<li><a href="{page_name(p)}">{html.escape(p[len(crate) + 1:])}</a></li>' for p in paths
        )
        + "</ul>"
        for crate, paths in sorted(by_crate.items())
    )
    commit = index.meta.get("commit", "")[:12]
    (OUT / "index.html").write_text(
        f"""<!doctype html><meta charset="utf-8"><meta name="viewport" content="width=device-width,initial-scale=1">
<title>slate code index</title><link rel="stylesheet" href="style.css">
<header><b>slate code index</b><span style="color:var(--mut)">{len(index.syms)} symbols, indexed at {commit}</span></header>
<div style="padding:16px 16px 0"><input id="q" placeholder="Search symbols (e.g. lower_binary, Preprocessor::reject_poisoned)" autofocus></div>
<ul id="hits" style="padding:0 16px"></ul>
{cross_crate_html(index)}
{sections}
<script src="data.js"></script>
<script>
const hits = document.getElementById('hits');
const esc = s => s.replace(/[&<>"]/g, c => ({{'&':'&amp;','<':'&lt;','>':'&gt;','"':'&quot;'}}[c]));
document.getElementById('q').addEventListener('input', e => {{
  const q = e.target.value.toLowerCase();
  if (!q) {{ hits.innerHTML = ''; return; }}
  const found = [];
  for (let i = 0; i < SYMS.length && found.length < 40; i++)
    if (NAMES[i].toLowerCase().includes(q)) found.push(i);
  hits.innerHTML = found.map(s => {{
    const [p, l] = DEFS[s];
    return `<li style="list-style:none"><a href="${{pageName(p)}}#L${{l}}">${{esc(NAMES[s])}}</a> ` +
      `<span style="color:var(--mut)">${{esc(p)}}:${{l}}</span></li>`;
  }}).join('');
}});
</script>"""
    )
    print(f"wrote {len(index.files)} pages to {OUT}")
    changed = git("diff", "--name-only", index.meta.get("commit", "HEAD"), "--", "*.rs")
    if changed:
        print(f"warning: {len(changed.split())} .rs files changed since the index; rebuild it for exact links")


if __name__ == "__main__":
    write_site(Index(connect()))
