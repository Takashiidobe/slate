import { ConsoleStdout, Directory, File, OpenFile, PreopenDirectory, WASI } from './vendor/browser_wasi_shim/index.js';

const manifest = fetch('manifest.json').then((response) => response.ok ? response.json() : Promise.reject(new Error(`manifest.json: ${response.status}`)));
const slate = manifest.then(async ({ wasm }) => WebAssembly.compile(await fetchBytes(wasm)));
const archives = new Map();

async function fetchBytes(path, gzip = false) {
  const response = await fetch(path);
  if (!response.ok) throw new Error(`${path}: ${response.status}`);
  const body = gzip ? response.body.pipeThrough(new DecompressionStream('gzip')) : response.body;
  return new Uint8Array(await new Response(body).arrayBuffer());
}

function megabytes(bytes) {
  return `${(bytes / 1e6).toFixed(1)} MB`;
}

function archive({ path, bytes }, label) {
  if (!archives.has(path)) {
    postMessage({ type: 'status', text: `Downloading ${label} headers (${megabytes(bytes)})…` });
    archives.set(path, fetchBytes(path, true).then(untar).catch((error) => { archives.delete(path); throw error; }));
  }
  return archives.get(path);
}

function untar(bytes) {
  const root = new Map();
  const decoder = new TextDecoder();
  const field = (header, start, length) => decoder.decode(header.subarray(start, start + length)).replace(/\0[\s\S]*$/, '');
  for (let offset = 0; offset + 512 <= bytes.length;) {
    const header = bytes.subarray(offset, offset + 512);
    if (header[0] === 0) break;
    const size = parseInt(field(header, 124, 12).trim() || '0', 8);
    const prefix = field(header, 345, 155);
    const name = (prefix ? `${prefix}/` : '') + field(header, 0, 100);
    const type = field(header, 156, 1);
    offset += 512;
    if (type === '0' || type === '') insert(root, name, bytes.slice(offset, offset + size));
    offset += Math.ceil(size / 512) * 512;
  }
  return toDirectory(root);
}

function insert(root, path, data) {
  const parts = path.split('/').filter((part) => part && part !== '.');
  const name = parts.pop();
  let dir = root;
  for (const part of parts) {
    if (!dir.has(part)) dir.set(part, new Map());
    dir = dir.get(part);
  }
  dir.set(name, data);
}

function toDirectory(tree) {
  return new Directory(new Map(Array.from(tree, ([name, entry]) => [name, entry instanceof Map ? toDirectory(entry) : new File(entry, { readonly: true })])));
}

function splitFlags(flags) {
  return Array.from(flags.matchAll(/(?:[^\s"']+|"[^"]*"|'[^']*')+/g), ([flag]) => flag.replace(/"([^"]*)"|'([^']*)'/g, '$1$2'));
}

const empty = () => Promise.resolve(new Directory(new Map()));

async function translate({ source, targets, compiler, flags }) {
  const { compilerHeaders, targets: available } = await manifest;
  for (const target of targets) if (!(target in available)) throw new Error(`unsupported target: ${target}`);
  if (!archives.size) postMessage({ type: 'status', text: 'Loading Slate…' });
  const [module, headers, ...sysroots] = await Promise.all([
    slate,
    compilerHeaders[compiler] ? archive(compilerHeaders[compiler], compiler) : empty(),
    ...targets.map((target) => available[target] ? archive(available[target], target) : empty()),
  ]);
  postMessage({ type: 'status', text: 'Translating…' });
  const stdout = new TextDecoder();
  const stderr = new TextDecoder();
  let out = '';
  let err = '';
  const fds = [
    new OpenFile(new File([])),
    new ConsoleStdout((bytes) => { out += stdout.decode(bytes, { stream: true }); }),
    new ConsoleStdout((bytes) => { err += stderr.decode(bytes, { stream: true }); }),
    new PreopenDirectory('/work', new Map([['input.c', new File(new TextEncoder().encode(source))]])),
    new PreopenDirectory('/sysroots', new Map(targets.map((target, index) => [target, sysroots[index]]))),
    new PreopenDirectory('/compiler-headers', headers.contents),
  ];
  const wasi = new WASI(
    ['slate', 'translate', '--diagnostic-format=json', `--targets=${targets.join(',')}`, `--flavor=${compiler}`, ...splitFlags(flags), '/work/input.c'],
    ['SLATE_SYSROOTS=/sysroots', 'SLATE_COMPILER_HEADERS=/compiler-headers'],
    fds,
  );
  const instance = await WebAssembly.instantiate(module, { wasi_snapshot_preview1: wasi.wasiImport });
  const started = performance.now();
  let code;
  try {
    code = wasi.start(instance);
  } catch (error) {
    throw new Error(tidy(`${err}${error instanceof Error ? error.message : error}`));
  }
  out += stdout.decode();
  err += stderr.decode();
  let diagnostics = [];
  try {
    const report = JSON.parse(err);
    if (report.version === 1 && Array.isArray(report.diagnostics)) diagnostics = report.diagnostics;
  } catch {}
  if (code !== 0) {
    const message = diagnostics.find((diagnostic) => diagnostic.severity === 'error')?.message;
    throw Object.assign(new Error(message || tidy(err) || `slate exited with status ${code}`), { diagnostics });
  }
  return { rust: out, diagnostics, ms: performance.now() - started };
}

function tidy(text) {
  return text.replaceAll('/work/input.c', 'input.c').trim();
}

onmessage = async ({ data: { id, ...request } }) => {
  try {
    postMessage({ type: 'result', id, ok: true, ...await translate(request) });
  } catch (error) {
    postMessage({ type: 'result', id, ok: false, error: error instanceof Error ? error.message : String(error), diagnostics: error.diagnostics ?? [] });
  }
};
